/**
 * Forensic Utility Functions Implementation
 * 
 * Core utility functions for artifact lists, metadata, bytes, etc.
 * Plugin registry functions.
 */

#include "forensic_types.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* ============================================================================
 * Deep Copy Helpers (static - internal only)
 * ============================================================================ */

static forensic_metadata_t forensic_metadata_deep_copy(const forensic_metadata_t* src) {
    forensic_metadata_t dst = forensic_metadata_create(src->capacity);
    for (size_t i = 0; i < src->count; i++) {
        forensic_metadata_add(&dst, src->items[i].key, src->items[i].value);
    }
    return dst;
}

static forensic_bytes_t forensic_bytes_deep_copy(const forensic_bytes_t* src) {
    return forensic_bytes_create(src->data, src->len);
}

static forensic_artifact_t forensic_artifact_deep_copy(const forensic_artifact_t* src) {
    forensic_artifact_t dst = {0};
    dst.plugin_name = src->plugin_name ? strdup(src->plugin_name) : NULL;
    dst.artifact_type = src->artifact_type ? strdup(src->artifact_type) : NULL;
    dst.timestamp = src->timestamp ? strdup(src->timestamp) : NULL;
    dst.raw = forensic_bytes_deep_copy(&src->raw);
    dst.metadata = forensic_metadata_deep_copy(&src->metadata);
    return dst;
}

/* ============================================================================
 * Artifact List
 * ============================================================================ */

forensic_artifact_list_t* forensic_artifact_list_create(size_t capacity) {
    forensic_artifact_list_t* list = calloc(1, sizeof(forensic_artifact_list_t));
    if (!list) return NULL;
    
    list->capacity = capacity > 0 ? capacity : 16;
    list->items = calloc(list->capacity, sizeof(forensic_artifact_t));
    if (!list->items) {
        free(list);
        return NULL;
    }
    return list;
}

void forensic_artifact_list_destroy(forensic_artifact_list_t* list) {
    if (!list) return;
    for (size_t i = 0; i < list->count; i++) {
        forensic_bytes_destroy(&list->items[i].raw);
        forensic_metadata_destroy(&list->items[i].metadata);
        free((char*)list->items[i].plugin_name);
        free((char*)list->items[i].artifact_type);
        free((char*)list->items[i].timestamp);
    }
    free(list->items);
    free(list);
}

int forensic_artifact_list_add_deep(forensic_artifact_list_t* list,
                                     const forensic_artifact_t* artifact) {
    if (!list || !artifact) return -1;
    
    if (list->count >= list->capacity) {
        size_t new_cap = list->capacity * 2;
        forensic_artifact_t* new_items = realloc(list->items, 
            new_cap * sizeof(forensic_artifact_t));
        if (!new_items) return -1;
        list->items = new_items;
        list->capacity = new_cap;
    }
    
    list->items[list->count] = forensic_artifact_deep_copy(artifact);
    list->count++;
    return 0;
}

int forensic_artifact_list_add(forensic_artifact_list_t* list,
                                const forensic_artifact_t* artifact) {
    return forensic_artifact_list_add_deep(list, artifact);
}

int forensic_artifact_list_append_list(forensic_artifact_list_t* dst,
                                        const forensic_artifact_list_t* src) {
    if (!dst || !src) return -1;
    
    for (size_t i = 0; i < src->count; i++) {
        int ret = forensic_artifact_list_add_deep(dst, &src->items[i]);
        if (ret != 0) return ret;
    }
    return 0;
}

/* ============================================================================
 * Bytes
 * ============================================================================ */

forensic_bytes_t forensic_bytes_create(const void* data, size_t len) {
    forensic_bytes_t bytes = {0};
    if (len > 0 && data) {
        bytes.data = malloc(len);
        if (bytes.data) {
            memcpy(bytes.data, data, len);
            bytes.len = len;
        }
    }
    return bytes;
}


void forensic_bytes_destroy(forensic_bytes_t* bytes) {
    if (bytes && bytes->data) {
        free(bytes->data);
        bytes->data = NULL;
        bytes->len = 0;
    }
}

/* ============================================================================
 * Metadata
 * ============================================================================ */

forensic_metadata_t forensic_metadata_create(size_t capacity) {
    forensic_metadata_t meta = {0};
    meta.capacity = capacity > 0 ? capacity : 8;
    meta.items = calloc(meta.capacity, sizeof(forensic_kv_t));
    return meta;
}

void forensic_metadata_destroy(forensic_metadata_t* meta) {
    if (!meta || !meta->items) return;
    for (size_t i = 0; i < meta->count; i++) {
        free(meta->items[i].key);
        free(meta->items[i].value);
    }
    free(meta->items);
    meta->items = NULL;
    meta->count = 0;
    meta->capacity = 0;
}

int forensic_metadata_add(forensic_metadata_t* meta, const char* key, const char* value) {
    if (!meta || !key || !value) return -1;
    
    if (meta->count >= meta->capacity) {
        size_t new_cap = meta->capacity * 2;
        forensic_kv_t* new_items = realloc(meta->items, new_cap * sizeof(forensic_kv_t));
        if (!new_items) return -1;
        meta->items = new_items;
        meta->capacity = new_cap;
    }
    
    meta->items[meta->count].key = strdup(key);
    meta->items[meta->count].value = strdup(value);
    if (!meta->items[meta->count].key || !meta->items[meta->count].value) {
        free(meta->items[meta->count].key);
        free(meta->items[meta->count].value);
        return -1;
    }
    meta->count++;
    return 0;
}

const char* forensic_metadata_get(const forensic_metadata_t* meta, const char* key) {
    if (!meta || !key) return NULL;
    for (size_t i = 0; i < meta->count; i++) {
        if (strcmp(meta->items[i].key, key) == 0) {
            return meta->items[i].value;
        }
    }
    return NULL;
}

/* ============================================================================
 * Parsed Artifact
 * ============================================================================ */

struct forensic_parsed_artifact_s* forensic_parsed_artifact_create(const char* artifact_type,
                                                             const char* parser_name) {
    struct forensic_parsed_artifact_s* art = calloc(1, sizeof(struct forensic_parsed_artifact_s));
    if (!art) return NULL;
    
    art->artifact_type = artifact_type ? strdup(artifact_type) : NULL;
    art->parser_name = parser_name ? strdup(parser_name) : NULL;
    art->parsed_data = forensic_metadata_create(16);
    art->iocs.items = NULL;
    art->iocs.count = 0;
    art->timeline_events = NULL;
    art->timeline_count = 0;
    return art;
}

void forensic_parsed_artifact_destroy(struct forensic_parsed_artifact_s* artifact) {
    if (!artifact) return;
    free((char*)artifact->artifact_type);
    free((char*)artifact->parser_name);
    forensic_metadata_destroy(&artifact->parsed_data);
    for (size_t i = 0; i < artifact->iocs.count; i++) {
        forensic_ioc_destroy(&artifact->iocs.items[i]);
    }
    free(artifact->iocs.items);
    for (size_t i = 0; i < artifact->timeline_count; i++) {
        forensic_event_destroy(&artifact->timeline_events[i]);
    }
    free(artifact->timeline_events);
    free(artifact);
}

/* ============================================================================
 * IOC
 * ============================================================================ */

struct forensic_ioc_s* forensic_ioc_create(const char* type, const char* value,
                                     const char* source, double confidence) {
    struct forensic_ioc_s* ioc = calloc(1, sizeof(struct forensic_ioc_s));
    if (!ioc) return NULL;
    ioc->type = type ? strdup(type) : NULL;
    ioc->value = value ? strdup(value) : NULL;
    ioc->source = source ? strdup(source) : NULL;
    ioc->confidence = confidence;
    return ioc;
}

void forensic_ioc_destroy(struct forensic_ioc_s* ioc) {
    if (!ioc) return;
    free((char*)ioc->type);
    free((char*)ioc->value);
    free((char*)ioc->source);
}

/* ============================================================================
 * Event
 * ============================================================================ */

struct forensic_event_s* forensic_event_create(const char* event_type,
                                         const char* timestamp,
                                         const char* source_plugin,
                                         const char* description) {
    struct forensic_event_s* evt = calloc(1, sizeof(struct forensic_event_s));
    if (!evt) return NULL;
    evt->event_type = event_type ? strdup(event_type) : NULL;
    evt->timestamp = timestamp ? strdup(timestamp) : NULL;
    evt->source_plugin = source_plugin ? strdup(source_plugin) : NULL;
    evt->description = description ? strdup(description) : NULL;
    evt->details = forensic_metadata_create(8);
    return evt;
}

void forensic_event_destroy(struct forensic_event_s* evt) {
    if (!evt) return;
    free((char*)evt->event_type);
    free((char*)evt->timestamp);
    free((char*)evt->source_plugin);
    free((char*)evt->description);
    forensic_metadata_destroy(&evt->details);
}

/* ============================================================================
 * Plugin Registry
 * ============================================================================ */

forensic_plugin_registry_t* forensic_plugin_registry_create(void) {
    forensic_plugin_registry_t* reg = calloc(1, sizeof(forensic_plugin_registry_t));
    return reg;
}

void forensic_plugin_registry_destroy(forensic_plugin_registry_t* reg) {
    if (!reg) return;
    for (size_t i = 0; i < reg->collector_count; i++) {
        if (reg->collectors[i]->cleanup) reg->collectors[i]->cleanup();
    }
    for (size_t i = 0; i < reg->parser_count; i++) {
        if (reg->parsers[i]->cleanup) reg->parsers[i]->cleanup();
    }
    for (size_t i = 0; i < reg->output_count; i++) {
        if (reg->outputs[i]->cleanup) reg->outputs[i]->cleanup();
    }
    free(reg->collectors);
    free(reg->parsers);
    free(reg->outputs);
    free(reg);
}

int forensic_plugin_registry_register_collector(forensic_plugin_registry_t* reg,
                                                 struct forensic_data_source_plugin_s* plugin) {
    if (!reg || !plugin) return -1;
    
    size_t new_count = reg->collector_count + 1;
    struct forensic_data_source_plugin_s** new_items = realloc(reg->collectors,
        new_count * sizeof(struct forensic_data_source_plugin_s*));
    if (!new_items) return -1;
    
    new_items[reg->collector_count] = plugin;
    reg->collectors = new_items;
    reg->collector_count = new_count;
    return 0;
}

int forensic_plugin_registry_register_parser(forensic_plugin_registry_t* reg,
                                              struct forensic_artifact_parser_plugin_s* plugin) {
    if (!reg || !plugin) return -1;
    
    size_t new_count = reg->parser_count + 1;
    struct forensic_artifact_parser_plugin_s** new_items = realloc(reg->parsers,
        new_count * sizeof(struct forensic_artifact_parser_plugin_s*));
    if (!new_items) return -1;
    
    new_items[reg->parser_count] = plugin;
    reg->parsers = new_items;
    reg->parser_count = new_count;
    return 0;
}

int forensic_plugin_registry_register_output(forensic_plugin_registry_t* reg,
                                              struct forensic_output_plugin_s* plugin) {
    if (!reg || !plugin) return -1;
    
    size_t new_count = reg->output_count + 1;
    struct forensic_output_plugin_s** new_items = realloc(reg->outputs,
        new_count * sizeof(struct forensic_output_plugin_s*));
    if (!new_items) return -1;
    
    new_items[reg->output_count] = plugin;
    reg->outputs = new_items;
    reg->output_count = new_count;
    return 0;
}

struct forensic_data_source_plugin_s* forensic_plugin_registry_find_collector(
    forensic_plugin_registry_t* reg, const char* name) {
    if (!reg || !name) return NULL;
    for (size_t i = 0; i < reg->collector_count; i++) {
        if (strcmp(reg->collectors[i]->name, name) == 0) {
            return reg->collectors[i];
        }
    }
    return NULL;
}

struct forensic_artifact_parser_plugin_s* forensic_plugin_registry_find_parser(
    forensic_plugin_registry_t* reg, const char* artifact_type) {
    if (!reg || !artifact_type) return NULL;
    for (size_t i = 0; i < reg->parser_count; i++) {
        if (strcmp(reg->parsers[i]->artifact_type, artifact_type) == 0) {
            return reg->parsers[i];
        }
    }
    return NULL;
}

struct forensic_output_plugin_s* forensic_plugin_registry_find_output(
    forensic_plugin_registry_t* reg, const char* name) {
    if (!reg || !name) return NULL;
    for (size_t i = 0; i < reg->output_count; i++) {
        if (strcmp(reg->outputs[i]->name, name) == 0) {
            return reg->outputs[i];
        }
    }
    return NULL;
}