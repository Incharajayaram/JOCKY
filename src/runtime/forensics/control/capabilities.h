/**
 * Capability Permissions System
 * 
 * Static table in each plugin's manifest defines required capabilities.
 * Enforcement checks plugin manifest against operator's role before execution.
 * 
 * All type definitions are in forensic_types.h - this header only provides
 * the extern declaration for FORENSIC_ROLE_PERMISSIONS.
 */

#pragma once

#include "forensic_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* 
 * FORENSIC_ROLE_PERMISSIONS is defined in forensic_types.h with constructor.
 * This extern declaration is for compatibility.
 */

extern const forensic_role_permissions_t FORENSIC_ROLE_PERMISSIONS[ROLE_COUNT];

/* ============================================================================
 * Permission Check API (declared in forensic_types.h)
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