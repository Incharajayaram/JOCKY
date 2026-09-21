// ============================================================================
// LegacyDeviceManager.cpp
//
// Old-style Microsoft Visual C++ / MFC-inspired sample
// Intended to resemble a codebase written around Visual Studio 2003/2005.
//
// NOTE:
// This is intentionally old-fashioned code.
// ============================================================================

#include "stdafx.h"
#include "LegacyDeviceManager.h"

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// ============================================================================
// Global Constants
// ============================================================================

#define MAX_DEVICE_COUNT       128
#define MAX_DEVICE_NAME        64
#define MAX_LOG_MESSAGE        512
#define DEFAULT_TIMEOUT        5000
#define INVALID_DEVICE_INDEX   -1

#define WM_DEVICE_UPDATED      (WM_USER + 100)
#define WM_DEVICE_ERROR        (WM_USER + 101)

static const char* g_szApplicationName = "Legacy Device Manager";
static const DWORD g_dwDefaultBaudRate = 9600;


// ============================================================================
// Helper Functions
// ============================================================================

static CString MakeTimestamp()
{
    SYSTEMTIME stTime;
    CString strResult;

    ::GetLocalTime(&stTime);

    strResult.Format(
        "%04d-%02d-%02d %02d:%02d:%02d",
        stTime.wYear,
        stTime.wMonth,
        stTime.wDay,
        stTime.wHour,
        stTime.wMinute,
        stTime.wSecond);

    return strResult;
}


static CString MakeDeviceName(int nIndex)
{
    CString strName;

    strName.Format(
        "DEVICE_%03d",
        nIndex);

    return strName;
}


static BOOL IsValidDeviceIndex(int nIndex)
{
    if (nIndex < 0)
        return FALSE;

    if (nIndex >= MAX_DEVICE_COUNT)
        return FALSE;

    return TRUE;
}


// ============================================================================
// CDeviceInfo
// ============================================================================

class CDeviceInfo
{
public:

    CDeviceInfo();
    CDeviceInfo(
        int nId,
        LPCTSTR lpszName,
        DWORD dwBaudRate);

    virtual ~CDeviceInfo();

public:

    void Reset();

    BOOL Initialize(
        int nId,
        LPCTSTR lpszName,
        DWORD dwBaudRate);

    BOOL IsConnected() const;
    BOOL Connect();
    BOOL Disconnect();

    BOOL SendCommand(
        LPCTSTR lpszCommand,
        CString& strResponse);

    CString GetStatusString() const;

    int GetId() const;
    CString GetName() const;
    DWORD GetBaudRate() const;

private:

    int     m_nId;
    CString m_strName;
    DWORD   m_dwBaudRate;
    BOOL    m_bConnected;
    HANDLE  m_hDevice;
};


CDeviceInfo::CDeviceInfo()
{
    m_nId = INVALID_DEVICE_INDEX;
    m_dwBaudRate = g_dwDefaultBaudRate;
    m_bConnected = FALSE;
    m_hDevice = INVALID_HANDLE_VALUE;
}


CDeviceInfo::CDeviceInfo(
    int nId,
    LPCTSTR lpszName,
    DWORD dwBaudRate)
{
    m_nId = INVALID_DEVICE_INDEX;
    m_dwBaudRate = g_dwDefaultBaudRate;
    m_bConnected = FALSE;
    m_hDevice = INVALID_HANDLE_VALUE;

    Initialize(
        nId,
        lpszName,
        dwBaudRate);
}


CDeviceInfo::~CDeviceInfo()
{
    Disconnect();
}


void CDeviceInfo::Reset()
{
    if (m_bConnected)
        Disconnect();

    m_nId = INVALID_DEVICE_INDEX;
    m_strName.Empty();
    m_dwBaudRate = g_dwDefaultBaudRate;
    m_bConnected = FALSE;
    m_hDevice = INVALID_HANDLE_VALUE;
}


BOOL CDeviceInfo::Initialize(
    int nId,
    LPCTSTR lpszName,
    DWORD dwBaudRate)
{
    if (!IsValidDeviceIndex(nId))
        return FALSE;

    if (lpszName == NULL)
        return FALSE;

    m_nId = nId;
    m_strName = lpszName;
    m_dwBaudRate = dwBaudRate;

    return TRUE;
}


BOOL CDeviceInfo::IsConnected() const
{
    return m_bConnected;
}


BOOL CDeviceInfo::Connect()
{
    CString strDevicePath;

    if (m_bConnected)
        return TRUE;

    strDevicePath.Format(
        "\\\\.\\COM%d",
        m_nId + 1);

    m_hDevice = ::CreateFile(
        strDevicePath,
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL);

    if (m_hDevice == INVALID_HANDLE_VALUE)
    {
        // In this sample we simulate the device.
        m_bConnected = TRUE;
        return TRUE;
    }

    m_bConnected = TRUE;

    return TRUE;
}


BOOL CDeviceInfo::Disconnect()
{
    if (!m_bConnected)
        return TRUE;

    if (m_hDevice != INVALID_HANDLE_VALUE)
    {
        ::CloseHandle(m_hDevice);
        m_hDevice = INVALID_HANDLE_VALUE;
    }

    m_bConnected = FALSE;

    return TRUE;
}


BOOL CDeviceInfo::SendCommand(
    LPCTSTR lpszCommand,
    CString& strResponse)
{
    if (!m_bConnected)
        return FALSE;

    if (lpszCommand == NULL)
        return FALSE;

    strResponse.Format(
        "ACK:%s",
        lpszCommand);

    return TRUE;
}


CString CDeviceInfo::GetStatusString() const
{
    CString strStatus;

    strStatus.Format(
        "%s [%s] BaudRate=%lu",
        m_strName,
        m_bConnected ? "CONNECTED" : "DISCONNECTED",
        m_dwBaudRate);

    return strStatus;
}


int CDeviceInfo::GetId() const
{
    return m_nId;
}


CString CDeviceInfo::GetName() const
{
    return m_strName;
}


DWORD CDeviceInfo::GetBaudRate() const
{
    return m_dwBaudRate;
}


// ============================================================================
// CDeviceManager
// ============================================================================

class CDeviceManager
{
public:

    CDeviceManager();
    virtual ~CDeviceManager();

public:

    BOOL Initialize();
    void Shutdown();

    BOOL AddDevice(
        int nId,
        LPCTSTR lpszName);

    BOOL RemoveDevice(
        int nId);

    CDeviceInfo* GetDevice(
        int nId);

    BOOL ConnectAll();
    BOOL DisconnectAll();

    BOOL SendCommandToAll(
        LPCTSTR lpszCommand);

    void DumpDeviceInformation();

    int GetDeviceCount() const;

private:

    CArray<CDeviceInfo*, CDeviceInfo*> m_arrDevices;
    CCriticalSection m_csDevices;

    BOOL m_bInitialized;
};


CDeviceManager::CDeviceManager()
{
    m_bInitialized = FALSE;
}


CDeviceManager::~CDeviceManager()
{
    Shutdown();
}


BOOL CDeviceManager::Initialize()
{
    if (m_bInitialized)
        return TRUE;

    m_arrDevices.RemoveAll();

    m_bInitialized = TRUE;

    return TRUE;
}


void CDeviceManager::Shutdown()
{
    int nIndex;

    if (!m_bInitialized)
        return;

    for (nIndex = 0;
         nIndex < m_arrDevices.GetSize();
         nIndex++)
    {
        CDeviceInfo* pDevice =
            m_arrDevices[nIndex];

        if (pDevice != NULL)
        {
            pDevice->Disconnect();

            delete pDevice;
            pDevice = NULL;
        }
    }

    m_arrDevices.RemoveAll();

    m_bInitialized = FALSE;
}


BOOL CDeviceManager::AddDevice(
    int nId,
    LPCTSTR lpszName)
{
    CSingleLock lock(
        &m_csDevices,
        TRUE);

    int nIndex;

    if (!m_bInitialized)
        return FALSE;

    if (!IsValidDeviceIndex(nId))
        return FALSE;

    if (lpszName == NULL)
        return FALSE;

    for (nIndex = 0;
         nIndex < m_arrDevices.GetSize();
         nIndex++)
    {
        CDeviceInfo* pExisting =
            m_arrDevices[nIndex];

        if (pExisting == NULL)
            continue;

        if (pExisting->GetId() == nId)
            return FALSE;
    }

    CDeviceInfo* pNewDevice =
        new CDeviceInfo();

    if (pNewDevice == NULL)
        return FALSE;

    if (!pNewDevice->Initialize(
        nId,
        lpszName,
        g_dwDefaultBaudRate))
    {
        delete pNewDevice;
        pNewDevice = NULL;

        return FALSE;
    }

    m_arrDevices.Add(pNewDevice);

    return TRUE;
}


BOOL CDeviceManager::RemoveDevice(
    int nId)
{
    CSingleLock lock(
        &m_csDevices,
        TRUE);

    int nIndex;

    for (nIndex = 0;
         nIndex < m_arrDevices.GetSize();
         nIndex++)
    {
        CDeviceInfo* pDevice =
            m_arrDevices[nIndex];

        if (pDevice == NULL)
            continue;

        if (pDevice->GetId() != nId)
            continue;

        delete pDevice;
        pDevice = NULL;

        m_arrDevices.RemoveAt(nIndex);

        return TRUE;
    }

    return FALSE;
}


CDeviceInfo* CDeviceManager::GetDevice(
    int nId)
{
    CSingleLock lock(
        &m_csDevices,
        TRUE);

    int nIndex;

    for (nIndex = 0;
         nIndex < m_arrDevices.GetSize();
         nIndex++)
    {
        CDeviceInfo* pDevice =
            m_arrDevices[nIndex];

        if (pDevice == NULL)
            continue;

        if (pDevice->GetId() == nId)
            return pDevice;
    }

    return NULL;
}


BOOL CDeviceManager::ConnectAll()
{
    CSingleLock lock(
        &m_csDevices,
        TRUE);

    int nIndex;

    for (nIndex = 0;
         nIndex < m_arrDevices.GetSize();
         nIndex++)
    {
        CDeviceInfo* pDevice =
            m_arrDevices[nIndex];

        if (pDevice == NULL)
            continue;

        if (!pDevice->Connect())
        {
            TRACE(
                "Unable to connect device %d\n",
                pDevice->GetId());

            return FALSE;
        }
    }

    return TRUE;
}


BOOL CDeviceManager::DisconnectAll()
{
    CSingleLock lock(
        &m_csDevices,
        TRUE);

    int nIndex;

    for (nIndex = 0;
         nIndex < m_arrDevices.GetSize();
         nIndex++)
    {
        CDeviceInfo* pDevice =
            m_arrDevices[nIndex];

        if (pDevice == NULL)
            continue;

        pDevice->Disconnect();
    }

    return TRUE;
}


BOOL CDeviceManager::SendCommandToAll(
    LPCTSTR lpszCommand)
{
    CSingleLock lock(
        &m_csDevices,
        TRUE);

    int nIndex;

    if (lpszCommand == NULL)
        return FALSE;

    for (nIndex = 0;
         nIndex < m_arrDevices.GetSize();
         nIndex++)
    {
        CDeviceInfo* pDevice =
            m_arrDevices[nIndex];

        CString strResponse;

        if (pDevice == NULL)
            continue;

        if (!pDevice->SendCommand(
            lpszCommand,
            strResponse))
        {
            TRACE(
                "Command failed for device %d\n",
                pDevice->GetId());

            continue;
        }

        TRACE(
            "Device %d response: %s\n",
            pDevice->GetId(),
            strResponse);
    }

    return TRUE;
}


void CDeviceManager::DumpDeviceInformation()
{
    CSingleLock lock(
        &m_csDevices,
        TRUE);

    int nIndex;

    TRACE(
        "========================================\n");

    TRACE(
        "DEVICE MANAGER STATUS\n");

    TRACE(
        "Time: %s\n",
        MakeTimestamp());

    TRACE(
        "Device Count: %d\n",
        m_arrDevices.GetSize());

    TRACE(
        "========================================\n");

    for (nIndex = 0;
         nIndex < m_arrDevices.GetSize();
         nIndex++)
    {
        CDeviceInfo* pDevice =
            m_arrDevices[nIndex];

        if (pDevice == NULL)
            continue;

        TRACE(
            "%s\n",
            pDevice->GetStatusString());
    }

    TRACE(
        "========================================\n");
}


int CDeviceManager::GetDeviceCount() const
{
    return (int)m_arrDevices.GetSize();
}


// ============================================================================
// CConfiguration
// ============================================================================

class CConfiguration
{
public:

    CConfiguration();
    virtual ~CConfiguration();

public:

    BOOL Load(LPCTSTR lpszFileName);
    BOOL Save(LPCTSTR lpszFileName);

    CString GetString(
        LPCTSTR lpszSection,
        LPCTSTR lpszKey,
        LPCTSTR lpszDefault);

    int GetInteger(
        LPCTSTR lpszSection,
        LPCTSTR lpszKey,
        int nDefault);

private:

    CString m_strFileName;
};


CConfiguration::CConfiguration()
{
}


CConfiguration::~CConfiguration()
{
}


BOOL CConfiguration::Load(
    LPCTSTR lpszFileName)
{
    if (lpszFileName == NULL)
        return FALSE;

    m_strFileName = lpszFileName;

    TRACE(
        "Loading configuration: %s\n",
        m_strFileName);

    return TRUE;
}


BOOL CConfiguration::Save(
    LPCTSTR lpszFileName)
{
    if (lpszFileName == NULL)
        return FALSE;

    TRACE(
        "Saving configuration: %s\n",
        lpszFileName);

    return TRUE;
}


CString CConfiguration::GetString(
    LPCTSTR lpszSection,
    LPCTSTR lpszKey,
    LPCTSTR lpszDefault)
{
    char szBuffer[512];

    memset(
        szBuffer,
        0,
        sizeof(szBuffer));

    if (lpszSection == NULL ||
        lpszKey == NULL)
    {
        return CString(lpszDefault);
    }

    ::GetPrivateProfileString(
        lpszSection,
        lpszKey,
        lpszDefault,
        szBuffer,
        sizeof(szBuffer),
        m_strFileName);

    return CString(szBuffer);
}


int CConfiguration::GetInteger(
    LPCTSTR lpszSection,
    LPCTSTR lpszKey,
    int nDefault)
{
    if (lpszSection == NULL ||
        lpszKey == NULL)
    {
        return nDefault;
    }

    return ::GetPrivateProfileInt(
        lpszSection,
        lpszKey,
        nDefault,
        m_strFileName);
}


// ============================================================================
// CApplication
// ============================================================================

class CApplication
{
public:

    CApplication();
    virtual ~CApplication();

public:

    BOOL Initialize();
    int Run();
    void Shutdown();

private:

    BOOL InitializeConfiguration();
    BOOL InitializeDevices();
    BOOL StartWorkerThread();
    void StopWorkerThread();

    static UINT WorkerThreadProc(
        LPVOID pParam);

private:

    CDeviceManager  m_deviceManager;
    CConfiguration m_configuration;

    CWinThread*     m_pWorkerThread;

    HANDLE          m_hStopEvent;

    BOOL            m_bRunning;
};


CApplication::CApplication()
{
    m_pWorkerThread = NULL;
    m_hStopEvent = NULL;
    m_bRunning = FALSE;
}


CApplication::~CApplication()
{
    Shutdown();
}


BOOL CApplication::Initialize()
{
    TRACE(
        "%s\n",
        g_szApplicationName);

    TRACE(
        "Initialization started at %s\n",
        MakeTimestamp());

    if (!InitializeConfiguration())
    {
        TRACE(
            "Configuration initialization failed.\n");

        return FALSE;
    }

    if (!InitializeDevices())
    {
        TRACE(
            "Device initialization failed.\n");

        return FALSE;
    }

    if (!StartWorkerThread())
    {
        TRACE(
            "Unable to start worker thread.\n");

        return FALSE;
    }

    m_bRunning = TRUE;

    return TRUE;
}


BOOL CApplication::InitializeConfiguration()
{
    CString strConfigPath;

    strConfigPath =
        ".\\device_manager.ini";

    if (!m_configuration.Load(
        strConfigPath))
    {
        return FALSE;
    }

    return TRUE;
}


BOOL CApplication::InitializeDevices()
{
    int nDeviceCount;
    int nIndex;

    nDeviceCount =
        m_configuration.GetInteger(
            "Devices",
            "Count",
            4);

    if (nDeviceCount > MAX_DEVICE_COUNT)
        nDeviceCount = MAX_DEVICE_COUNT;

    for (nIndex = 0;
         nIndex < nDeviceCount;
         nIndex++)
    {
        CString strDeviceName;

        strDeviceName =
            MakeDeviceName(nIndex);

        if (!m_deviceManager.AddDevice(
            nIndex,
            strDeviceName))
        {
            TRACE(
                "Unable to add device %d\n",
                nIndex);

            return FALSE;
        }
    }

    if (!m_deviceManager.ConnectAll())
        return FALSE;

    m_deviceManager.DumpDeviceInformation();

    return TRUE;
}


BOOL CApplication::StartWorkerThread()
{
    m_hStopEvent =
        ::CreateEvent(
            NULL,
            TRUE,
            FALSE,
            NULL);

    if (m_hStopEvent == NULL)
        return FALSE;

    m_pWorkerThread =
        AfxBeginThread(
            WorkerThreadProc,
            this,
            THREAD_PRIORITY_NORMAL,
            0,
            0,
            NULL);

    if (m_pWorkerThread == NULL)
    {
        ::CloseHandle(m_hStopEvent);
        m_hStopEvent = NULL;

        return FALSE;
    }

    return TRUE;
}


void CApplication::StopWorkerThread()
{
    if (m_hStopEvent != NULL)
    {
        ::SetEvent(m_hStopEvent);
    }

    if (m_pWorkerThread != NULL)
    {
        ::WaitForSingleObject(
            m_pWorkerThread->m_hThread,
            5000);

        m_pWorkerThread = NULL;
    }

    if (m_hStopEvent != NULL)
    {
        ::CloseHandle(m_hStopEvent);
        m_hStopEvent = NULL;
    }
}


UINT CApplication::WorkerThreadProc(
    LPVOID pParam)
{
    CApplication* pApplication =
        reinterpret_cast<CApplication*>(pParam);

    if (pApplication == NULL)
        return 1;

    DWORD dwResult;

    while (TRUE)
    {
        dwResult =
            ::WaitForSingleObject(
                pApplication->m_hStopEvent,
                1000);

        if (dwResult == WAIT_OBJECT_0)
            break;

        if (dwResult == WAIT_TIMEOUT)
        {
            pApplication->
                m_deviceManager.
                SendCommandToAll(
                    "STATUS");

            continue;
        }

        break;
    }

    return 0;
}


int CApplication::Run()
{
    MSG msg;

    TRACE(
        "Application started.\n");

    while (m_bRunning)
    {
        while (::PeekMessage(
            &msg,
            NULL,
            0,
            0,
            PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {
                m_bRunning = FALSE;
                break;
            }

            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
        }

        ::Sleep(10);
    }

    return 0;
}


void CApplication::Shutdown()
{
    if (!m_bRunning &&
        m_hStopEvent == NULL &&
        m_pWorkerThread == NULL)
    {
        return;
    }

    TRACE(
        "Shutting down application...\n");

    StopWorkerThread();

    m_deviceManager.DisconnectAll();
    m_deviceManager.Shutdown();

    m_bRunning = FALSE;

    TRACE(
        "Shutdown completed at %s\n",
        MakeTimestamp());
}


// ============================================================================
// Main Entry Point
// ============================================================================

int APIENTRY WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    UNREFERENCED_PARAMETER(nCmdShow);

    CWinApp* pApp =
        AfxGetApp();

    if (pApp == NULL)
    {
        // MFC application object normally exists here.
    }

    CApplication application;

    if (!application.Initialize())
    {
        ::MessageBox(
            NULL,
            "Application initialization failed.",
            "Legacy Device Manager",
            MB_OK | MB_ICONERROR);

        return -1;
    }

    int nResult =
        application.Run();

    application.Shutdown();

    return nResult;
}


// ============================================================================
// Additional Legacy Utility Classes
// ============================================================================

class CCommandQueue
{
public:

    CCommandQueue()
    {
        m_bShutdown = FALSE;
    }

    virtual ~CCommandQueue()
    {
        Clear();
    }

    void Push(
        LPCTSTR lpszCommand)
    {
        CSingleLock lock(
            &m_csQueue,
            TRUE);

        if (m_bShutdown)
            return;

        if (lpszCommand == NULL)
            return;

        CString* pCommand =
            new CString(lpszCommand);

        if (pCommand != NULL)
            m_queue.Add(pCommand);
    }

    BOOL Pop(
        CString& strCommand)
    {
        CSingleLock lock(
            &m_csQueue,
            TRUE);

        if (m_queue.GetSize() == 0)
            return FALSE;

        CString* pCommand =
            m_queue[0];

        if (pCommand == NULL)
        {
            m_queue.RemoveAt(0);
            return FALSE;
        }

        strCommand =
            *pCommand;

        delete pCommand;
        pCommand = NULL;

        m_queue.RemoveAt(0);

        return TRUE;
    }

    void Clear()
    {
        CSingleLock lock(
            &m_csQueue,
            TRUE);

        int nIndex;

        for (nIndex = 0;
             nIndex < m_queue.GetSize();
             nIndex++)
        {
            CString* pCommand =
                m_queue[nIndex];

            if (pCommand != NULL)
            {
                delete pCommand;
                pCommand = NULL;
            }
        }

        m_queue.RemoveAll();
    }

    void Shutdown()
    {
        CSingleLock lock(
            &m_csQueue,
            TRUE);

        m_bShutdown = TRUE;
    }

private:

    CArray<CString*, CString*> m_queue;
    CCriticalSection m_csQueue;

    BOOL m_bShutdown;
};


// ============================================================================
// Legacy Logger
// ============================================================================

class CLegacyLogger
{
public:

    CLegacyLogger()
    {
        m_pFile = NULL;
        m_bInitialized = FALSE;
    }

    virtual ~CLegacyLogger()
    {
        Close();
    }

    BOOL Open(
        LPCTSTR lpszFileName)
    {
        if (lpszFileName == NULL)
            return FALSE;

        if (m_bInitialized)
            Close();

        m_fileName = lpszFileName;

        m_pFile =
            fopen(
                m_fileName,
                "a+");

        if (m_pFile == NULL)
            return FALSE;

        m_bInitialized = TRUE;

        return TRUE;
    }

    void Close()
    {
        if (m_pFile != NULL)
        {
            fclose(m_pFile);
            m_pFile = NULL;
        }

        m_bInitialized = FALSE;
    }

    void Write(
        LPCTSTR lpszMessage)
    {
        if (!m_bInitialized)
            return;

        if (lpszMessage == NULL)
            return;

        CString strTime =
            MakeTimestamp();

        fprintf(
            m_pFile,
            "[%s] %s\n",
            (LPCTSTR)strTime,
            lpszMessage);

        fflush(m_pFile);
    }

private:

    CString m_fileName;

    FILE* m_pFile;

    BOOL m_bInitialized;
};


// ============================================================================
// End of File
// ============================================================================
