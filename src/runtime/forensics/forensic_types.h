/**
 * Forensic Core Types
 * 
 * Unified header with all common types for the forensic subsystem.
 */

#define _GNU_SOURCE
#define _DEFAULT_SOURCE
#define _POSIX_C_SOURCE 200809L

#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#ifndef _WIN32
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <pwd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================================
 * Common Types
 * ============================================================================ */

typedef struct forensic_bytes_s {
    char* data;
    size_t len;
} forensic_bytes_t;

typedef struct forensic_kv_s {
    char* key;
    char* value;
} forensic_kv_t;

typedef struct forensic_metadata_s {
    forensic_kv_t* items;
    size_t count;
    size_t capacity;
} forensic_metadata_t;

typedef struct forensic_artifact_s {
    const char* plugin_name;
    const char* artifact_type;
    const char* timestamp;
    forensic_bytes_t raw;
    forensic_metadata_t metadata;
} forensic_artifact_t;

typedef struct forensic_artifact_list_s {
    forensic_artifact_t* items;
    size_t count;
    size_t capacity;
} forensic_artifact_list_t;

typedef struct forensic_ioc_s {
    const char* type;
    const char* value;
    const char* source;
    double confidence;
} forensic_ioc_t;

typedef struct forensic_ioc_list_s {
    forensic_ioc_t* items;
    size_t count;
} forensic_ioc_list_t;

typedef struct forensic_event_s {
    const char* event_type;
    const char* timestamp;
    const char* source_plugin;
    const char* description;
    forensic_metadata_t details;
} forensic_event_t;

typedef struct forensic_timeline_s {
    forensic_event_t* items;
    size_t count;
} forensic_timeline_t;

typedef struct forensic_parsed_artifact_s {
    const char* artifact_type;
    const char* parser_name;
    forensic_metadata_t parsed_data;
    forensic_ioc_list_t iocs;
    forensic_event_t* timeline_events;
    size_t timeline_count;
} forensic_parsed_artifact_t;

typedef struct forensic_parsed_artifact_list_s {
    forensic_parsed_artifact_t* items;
    size_t count;
} forensic_parsed_artifact_list_t;

/* ============================================================================
 * Plugin Interfaces
 * ============================================================================ */

typedef struct forensic_data_source_plugin_s {
    const char* name;
    const char* version;
    const char* description;
    int (*init)(void* config);
    forensic_artifact_list_t* (*collect)(const char* target, void* config);
    void (*cleanup)(void);
    const char** required_capabilities;
    size_t capability_count;
} forensic_data_source_plugin_t;

typedef struct forensic_artifact_parser_plugin_s {
    const char* name;
    const char* version;
    const char* description;
    const char* artifact_type;
    int (*init)(void* config);
    struct forensic_parsed_artifact_s* (*parse)(const struct forensic_bytes_s* data, void* config);
    void (*cleanup)(void);
    const char** required_capabilities;
    size_t capability_count;
    bool requires_sandbox;
    uint32_t max_cpu_time_ms;
    uint64_t max_memory_bytes;
} forensic_artifact_parser_plugin_t;

typedef struct forensic_output_plugin_s {
    const char* name;
    const char* version;
    const char* description;
    int (*init)(void* config);
    int (*generate)(const forensic_timeline_t* timeline,
                    const forensic_ioc_list_t* iocs,
                    const forensic_metadata_t* provenance,
                    void* config);
    void (*cleanup)(void);
    const char** required_capabilities;
    size_t capability_count;
} forensic_output_plugin_t;

typedef struct forensic_plugin_registry_s {
    struct forensic_data_source_plugin_s** collectors;
    size_t collector_count;
    struct forensic_artifact_parser_plugin_s** parsers;
    size_t parser_count;
    struct forensic_output_plugin_s** outputs;
    size_t output_count;
} forensic_plugin_registry_t;

forensic_plugin_registry_t* forensic_plugin_registry_create(void);
void forensic_plugin_registry_destroy(forensic_plugin_registry_t* reg);
int forensic_plugin_registry_register_collector(forensic_plugin_registry_t* reg,
                                                 struct forensic_data_source_plugin_s* plugin);
int forensic_plugin_registry_register_parser(forensic_plugin_registry_t* reg,
                                              struct forensic_artifact_parser_plugin_s* plugin);
int forensic_plugin_registry_register_output(forensic_plugin_registry_t* reg,
                                              struct forensic_output_plugin_s* plugin);
struct forensic_data_source_plugin_s* forensic_plugin_registry_find_collector(
    forensic_plugin_registry_t* reg, const char* name);
struct forensic_artifact_parser_plugin_s* forensic_plugin_registry_find_parser(
    forensic_plugin_registry_t* reg, const char* artifact_type);
struct forensic_output_plugin_s* forensic_plugin_registry_find_output(
    forensic_plugin_registry_t* reg, const char* name);

/* ============================================================================
 * Utility Functions
 * ============================================================================ */

forensic_artifact_list_t* forensic_artifact_list_create(size_t capacity);
void forensic_artifact_list_destroy(forensic_artifact_list_t* list);
int forensic_artifact_list_add(forensic_artifact_list_t* list,
                                const forensic_artifact_t* artifact);
int forensic_artifact_list_add_deep(forensic_artifact_list_t* list,
                                     const forensic_artifact_t* artifact);

forensic_bytes_t forensic_bytes_create(const void* data, size_t len);
void forensic_bytes_destroy(forensic_bytes_t* bytes);

forensic_metadata_t forensic_metadata_create(size_t capacity);
void forensic_metadata_destroy(forensic_metadata_t* meta);
int forensic_metadata_add(forensic_metadata_t* meta, const char* key, const char* value);
const char* forensic_metadata_get(const forensic_metadata_t* meta, const char* key);

struct forensic_parsed_artifact_s* forensic_parsed_artifact_create(const char* artifact_type,
                                                             const char* parser_name);
void forensic_parsed_artifact_destroy(struct forensic_parsed_artifact_s* artifact);

struct forensic_ioc_s* forensic_ioc_create(const char* type, const char* value,
                                     const char* source, double confidence);
void forensic_ioc_destroy(struct forensic_ioc_s* ioc);

struct forensic_event_s* forensic_event_create(const char* event_type,
                                         const char* timestamp,
                                         const char* source_plugin,
                                         const char* description);
void forensic_event_destroy(struct forensic_event_s* event);

/* ============================================================================
 * Capability System
 * ============================================================================ */

typedef enum {
    CAP_READ_PROCESS_INFO = 0,
    CAP_ENUMERATE_PROCESSES,
    CAP_READ_FILESYSTEM,
    CAP_ENUMERATE_FILES,
    CAP_ACCESS_TEMP_DIRS,
    CAP_READ_NETWORK_STATE,
    CAP_ENUMERATE_CONNECTIONS,
    CAP_READ_DNS_CACHE,
    CAP_READ_SYSTEM_CONFIG,
    CAP_ENUMERATE_SERVICES,
    CAP_READ_CRON_JOBS,
    CAP_READ_PROCESS_MEMORY,
    CAP_ENUMERATE_MEMORY_REGIONS,
    CAP_ACCESS_PROCFS,
    CAP_READ_ARTIFACT,
    CAP_PARSE_PE,
    CAP_PARSE_REGISTRY,
    CAP_PARSE_EVTX,
    CAP_PARSE_PREFETCH,
    CAP_PARSE_MFT,
    CAP_PARSE_NETWORK,
    CAP_WRITE_REPORT,
    CAP_NETWORK_ACCESS,
    CAP_COUNT
} forensic_capability_t;

typedef enum {
    ROLE_OPERATOR = 0,
    ROLE_ANALYST = 1,
    ROLE_COUNT
} forensic_role_t;

typedef struct {
    forensic_capability_t capabilities[CAP_COUNT];
    size_t count;
} forensic_capability_set_t;

typedef struct {
    forensic_role_t role;
    forensic_capability_set_t capabilities;
} forensic_role_permissions_t;

extern const forensic_role_permissions_t FORENSIC_ROLE_PERMISSIONS[ROLE_COUNT];

/* ============================================================================
 * Capability Information (defined in capabilities.c, exposed for name lookup)
 * ============================================================================ */

typedef struct forensic_capability_info_s {
    forensic_capability_t capability;
    const char* name;
    const char* description;
} forensic_capability_info_t;

extern const forensic_capability_info_t FORENSIC_CAPABILITIES[CAP_COUNT];

/* ============================================================================
 * Permission Check API
 * ============================================================================ */

bool forensic_permission_check_role(forensic_role_t role, forensic_capability_t cap);
bool forensic_permission_check_set(const forensic_capability_set_t* set, forensic_capability_t cap);
bool forensic_permission_check_plugin(const char* plugin_name, forensic_role_t operator_role);
const char* forensic_capability_name(forensic_capability_t cap);
const char* forensic_role_name(forensic_role_t role);
bool forensic_permission_validate_manifest(const char* plugin_name,
                                            const char** required_caps,
                                            size_t cap_count,
                                            forensic_role_t operator_role);

#ifdef __cplusplus
}
#endif