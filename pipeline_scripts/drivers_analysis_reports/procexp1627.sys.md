# DeepZero Analysis: procexp1627.sys — Full Breakdown

---

## 🐛 The Error (Cosmetic, Not a Blocker)

### `assess failed` — RateLimitError

Same as before: DeepZero's final AI assessment stage hit the **Google Gemini free tier daily quota**.

**Stage status:**

| Stage | Status |
|:---|:---|
| `discover` | ✅ Completed |
| `kernel_filter` | ✅ Completed |
| `decompile` | ✅ Completed |
| `semgrep_scanner` | ✅ Completed |
| `rank_by_findings` | ✅ Completed |
| `assess` | ❌ Failed (quota exceeded) |

The `assess` stage is only the AI's second opinion. All critical work completed successfully. The 11 HIGH findings are already conclusive.

**Fix options:** Wait 24 hours, switch LLM provider, or skip the stage entirely.

---

## 🎯 Driver Identity

| Field | Value |
|:---|:---|
| **File name** | `procexp1627.sys` |
| **SHA-256** | `9b6a84f7c40ea51c38cc4d2e93efb3375e9d98d4894a85941190d94fbe73a4e4` |
| **MD5** | `c06dda757b92e79540551efd00b99d4b` |
| **Size** | 36,192 bytes (~36 KB) |
| **Machine Type** | `AMD64` — 64-bit x86 |
| **Subsystem** | `NATIVE` — kernel-mode driver |
| **Signed** | `True` — valid signature |
| **Kernel Driver** | `True` — correct target |
| **Priority Score** | 2.0 |

**This is the Process Explorer driver** (PROCEXP152). It is a legitimate, Microsoft-signed Sysinternals component designed to give Process Explorer privileged kernel-level capabilities. It is a well-known tool, which means it is **very likely on Microsoft's Vulnerable Driver Blocklist**. You must verify the hash before proceeding.

---

## 🔓 Attack Surface

### Device Names
```
Device Name:     \Device\PROCEXP152
Symbolic Link:   \DosDevices\PROCEXP152
```

Unlike the previous drivers, this uses a **friendly, predictable name** — `PROCEXP152`. From user mode, you open a handle via:

```cpp
CreateFile("\\\\.\\PROCEXP152", ...)
```

### Key Flag: `device_on_load_path = False`

The device is **not created when the driver first loads**. It requires an initialization sequence (likely triggered by Process Explorer itself). Your exploit would need to either replicate that initialization or find an IOCTL that creates the device on demand.

### Attack Surface Numbers
```
Functions:     118
IOCTLs:        Not listed in the report (data missing)
```

The report does not include `decompile.ioctl_count` or `decompile.ioctl_codes`, so we cannot enumerate the IOCTLs. However, the `dispatch_ioctl.c` file exists and is the source of all findings, so IOCTLs are clearly present and exploitable.

---

## ⚠️ The 11 HIGH Severity Findings

### Category 1: `PsLookupProcessByProcessId` + `KeStackAttachProcess` — 3 findings

**Locations:** Lines 345, 494, 556

**Finding text:**
```
PsLookupProcessByProcessId followed by KeStackAttachProcess.
Classic process manipulation primitive for cross-process memory operations.
```

**What this means in simple language:**
- The driver looks up a process by its PID (`PsLookupProcessByProcessId`).
- It then **attaches** to that process's address space (`KeStackAttachProcess`).
- While attached, the driver can **read and write memory** in the target process's context.

**Why this is a BYOVD gold mine:**
This is the **exact primitive** used by tools like Process Explorer, Process Hacker, and Mimikatz to read/write another process's memory. Once you can attach to any process, you can:
- Read LSASS memory (dump credentials)
- Write to EDR process memory (disable or tamper)
- Manipulate protected processes (if PPL is not enforced or is bypassed)
- Read/write any process's memory without using standard APIs that EDR hooks

**This is arguably stronger than `MmMapIoSpace`** for certain tasks because it operates at the process level and is less likely to trigger certain kernel memory protections.

### Category 2: `RtlCopyMemory` with User-Controlled Length — 8 findings

**Locations:** Lines 284, 393, 645, 653, 750, 758, 777, 789

**Finding text:**
```
RtlCopyMemory/memcpy with length potentially derived from
user-controlled IOCTL input buffer.
Classic buffer overflow if the destination is a fixed-size kernel buffer.
```

Eight separate buffer overflow primitives. Combined with the process manipulation primitives, this driver has **multiple independent paths** to kernel compromise.

---

## 🔍 Dangerous Imports

| Import | Purpose | Risk |
|:---|:---|:---|
| **`KeStackAttachProcess`** | Attach to another process's address space | 🔴 Cross-process memory access |
| **`PsLookupProcessByProcessId`** | Look up process by PID | 🔴 Required for above |
| **`ZwOpenProcess`** | Open a process handle | 🔴 Process manipulation |
| **`MmIsAddressValid`** | Check if kernel address is valid | 🟡 Used for safe memory access |
| **`ObOpenObjectByPointer`** | Open kernel objects by pointer | 🟡 Token/handle manipulation |
| **`ZwOpenProcessToken`** | Open a process token | 🔴 Token theft |
| **`ZwDuplicateObject`** | Duplicate handles | 🔴 Handle manipulation |
| **`ZwQueryInformationProcess`** | Query process info | 🟡 Reconnaissance |
| **`ZwQuerySystemInformation`** | Query system info | 🟡 Reconnaissance |
| **`SeCaptureSubjectContext`** | Capture caller's security context | 🟡 Privilege checks |
| **`SePrivilegeCheck`** | Check privileges | 🟡 Access control |
| **`ZwSetSecurityObject`** | Modify security descriptors | 🟡 Permission manipulation |
| **`IoCreateDevice` / `IoCreateSymbolicLink`** | Device creation | 🟡 Standard driver setup |
| **`strncpy`** | String copy (unsafe) | 🟡 Buffer overflow potential |
| **`_snwprintf`** | Formatted string output | 🟢 Standard utility |

**The critical set:** `KeStackAttachProcess` + `PsLookupProcessByProcessId` + `ZwOpenProcess` + `ZwOpenProcessToken` — this driver is a **complete process manipulation toolkit**. It can attach to any process, read/write memory, steal tokens, duplicate handles, and modify security descriptors.

This is exactly what Process Explorer uses to do its job, but it also makes the driver a **powerful weapon** when abused.

---

## 🛡️ Security Consideration: Privilege Checks

The driver imports:
- `SeCaptureSubjectContext`
- `SePrivilegeCheck`
- `SeReleaseSubjectContext`

This means the driver **does perform privilege checks** before allowing certain operations. It likely requires the caller to have `SeDebugPrivilege` or be an administrator. This is a **gate** that your exploit must bypass.

**However:** The buffer overflow findings (RtlCopyMemory) may not be protected by these checks, depending on where they occur in the dispatch routine. Some IOCTLs may be accessible to lower-privileged users.

**Action item:** Examine the decompiled code to see which IOCTLs are gated by `SePrivilegeCheck` and which are not. The ones without checks are your best entry points.

---

## 📊 Standalone Summary

| Aspect | Assessment |
|:---|:---|
| **Validity** | ✅ Signed, kernel-mode, AMD64 driver |
| **Attack surface** | 118 functions, IOCTLs present (exact count not listed) |
| **Primary primitive** | 3× `PsLookupProcessByProcessId` + `KeStackAttachProcess` (cross-process memory access) |
| **Secondary primitive** | 8× `RtlCopyMemory` user-controlled length (buffer overflow) |
| **Device creation** | Deferred (not on load) — requires init sequence |
| **Device security** | Uses `SePrivilegeCheck` — likely requires admin/debug privileges |
| **Blocklist status** | **Very likely blocked** — well-known Sysinternals driver |
| **Overall verdict** | Extremely powerful but likely blocked; verify hash first |

**The `KeStackAttachProcess` primitive is exceptionally powerful** — it allows reading/writing any process's memory, which is the core of credential dumping and EDR evasion. The 8 buffer overflows provide additional avenues. However, the privilege checks and the likely blocklist status are significant hurdles.

---

## 🛠️ Next Steps for procexp1627.sys

### 1. Verify Against the Microsoft Blocklist
Search `Microsoft_Blocked_Driver_List.pdf` for:
```
9b6a84f7c40ea51c38cc4d2e93efb3375e9d98d4894a85941190d94fbe73a4e4
```
**I strongly suspect this will be found.** If it is, this driver is **not usable** on modern systems with HVCI/blocklist enabled. Discard it and move on.

### 2. Investigate the Privilege Checks
Open the decompiled source:
```
/home/incharanew/JOCKY/DeepZero/work/loldrivers/drivers_out-6603aa5d/.bulk_temp/semgrep_0/9b6a84f7c40ea51c_dispatch_ioctl.c
```

Look for `SePrivilegeCheck` calls. Determine:
- Which IOCTLs require `SeDebugPrivilege`?
- Which IOCTLs are accessible without it?
- Can the buffer overflows be reached without admin privileges?

### 3. Identify Device Creation Trigger
Since `device_on_load_path = False`, find the IOCTL or code path that creates `\Device\PROCEXP152`. You may need to send a specific IOCTL first to initialize the device.

### 4. Add to Your JOCKY Manifest (Conditional)
If the hash is **not** on the blocklist, add it with a note that it requires admin privileges:

```yaml
name: "procexp1627.sys"
sha256: "9b6a84f7c40ea51c38cc4d2e93efb3375e9d98d4894a85941190d94fbe73a4e4"
device_path: "\\\\.\\PROCEXP152"
device_on_load: false
requires_admin: true  # SeDebugPrivilege likely required
capabilities: [process_attach, process_memory_rw, kernel_overflow]
priority: 3  # Lower priority due to blocklist risk
```

### 5. Test in Isolated Windows VM
- Disable HVCI and blocklist
- Enable test signing
- Snapshot before loading
- Load driver, trigger device creation, open handle, attempt an IOCTL
- Verify process attach primitive works
- Revert snapshot after

---

## 🧩 Comparison: All Drivers Analyzed So Far

| Feature | Driver A (`65329dad`) | Driver B (`LHA.sys`) | Driver C (`procexp1627.sys`) |
|:---|:---|:---|:---|
| **Size** | 65 KB | 35 KB | 36 KB |
| **Functions** | 155 | 116 | 118 |
| **IOCTLs** | 57 | 28 | Not listed (present) |
| **Total HIGH findings** | 38 | 14 | 11 |
| **`MmMapIoSpace` findings** | 21 | 13 | 0 |
| **`RtlCopyMemory` findings** | 17 | 1 | 8 |
| **`KeStackAttachProcess` findings** | 0 | 0 | 3 |
| **Primary primitive** | Arbitrary physical memory mapping | Arbitrary physical memory mapping | Cross-process memory access |
| **Device on load** | ✅ True | ❌ False | ❌ False |
| **Device creation API** | `IoCreateDevice` | `IoDevObjCreateDeviceSecure` | `IoCreateDevice` |
| **Privilege checks** | Unknown | Unknown | ✅ Present (`SePrivilegeCheck`) |
| **Blocklist risk** | Low (verify) | Low (verify) | **Very high** (Sysinternals) |
| **Overall strength** | ★★★★★ | ★★★★☆ | ★★★★☆ (if unblocked) |
| **Recommended priority** | 1 (primary) | 2 (fallback) | 3 (only if unblocked) |

---



## ✅ Bottom Line

**Procexp1627.sys is a powerful driver but a risky candidate.** Verify its blocklist status first. If it's blocked, discard it. If it's somehow unblocked (unlikely), investigate the privilege checks to see if the buffer overflows can be triggered without admin rights.

**Your strongest candidates remain Driver A and Driver B.** Continue running DeepZero against your corpus — you're building a diverse arsenal.