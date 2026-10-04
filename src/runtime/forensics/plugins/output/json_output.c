#include "../../forensic_types.h"
/**
 * JSON Output Plugin
 * 
 * Generates human-readable JSON report from analysis results.
 */

#include "../../forensic_types.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* ============================================================================
 * JSON Output Implementation
 * ============================================================================ */

static int json_output_init(void* config) {
    (void)config;
    printf("[json_output] Initialized\n");
    return 0;
}

static void write_metadata_json(FILE* f, const forensic_metadata_t* meta) {
    for (size_t i = 0; i < meta->count; i++) {
        fprintf(f, "    \"%s\": \"%s\"%s\n", 
                meta->items[i].key, meta->items[i].value,
                (i < meta->count - 1) ? "," : "");
    }
}

static int json_output_generate(const forensic_timeline_t* timeline,
                                 const forensic_ioc_list_t* iocs,
                                 const forensic_metadata_t* provenance,
                                 void* config) {
    const char* output_dir = config ? (const char*)config : ".";
    char output_path[512];
    snprintf(output_path, sizeof(output_path), "%s/forensic_report.json", output_dir);
    FILE* f = fopen(output_path, "w");
    if (!f) {
        fprintf(stderr, "[json_output] Failed to open output file: %s\n", output_path);
        return -1;
    }
    
    fprintf(f, "{\n");
    fprintf(f, "  \"generated\": \"%ld\",\n", (long)time(NULL));
    fprintf(f, "  \"version\": \"1.0\",\n");
    
    if (provenance) {
        fprintf(f, "  \"provenance\": {\n");
        write_metadata_json(f, provenance);
        fprintf(f, "  },\n");
    }
    
    fprintf(f, "  \"timeline\": [\n");
    for (size_t i = 0; i < timeline->count; i++) {
        const forensic_event_t* evt = &timeline->items[i];
        fprintf(f, "    {\n");
        fprintf(f, "      \"event_type\": \"%s\",\n", evt->event_type ? evt->event_type : "");
        fprintf(f, "      \"timestamp\": \"%s\",\n", evt->timestamp ? evt->timestamp : "");
        fprintf(f, "      \"source_plugin\": \"%s\",\n", evt->source_plugin ? evt->source_plugin : "");
        fprintf(f, "      \"description\": \"%s\",\n", evt->description ? evt->description : "");
        fprintf(f, "      \"details\": {\n");
        write_metadata_json(f, &evt->details);
        fprintf(f, "      }\n");
        fprintf(f, "    }%s\n", (i < timeline->count - 1) ? "," : "");
    }
    fprintf(f, "  ],\n");
    
    fprintf(f, "  \"iocs\": [\n");
    for (size_t i = 0; i < iocs->count; i++) {
        const forensic_ioc_t* ioc = &iocs->items[i];
        fprintf(f, "    {\n");
        fprintf(f, "      \"type\": \"%s\",\n", ioc->type ? ioc->type : "");
        fprintf(f, "      \"value\": \"%s\",\n", ioc->value ? ioc->value : "");
        fprintf(f, "      \"source\": \"%s\",\n", ioc->source ? ioc->source : "");
        fprintf(f, "      \"confidence\": %.2f\n", ioc->confidence);
        fprintf(f, "    }%s\n", (i < iocs->count - 1) ? "," : "");
    }
    fprintf(f, "  ]\n");
    
    fprintf(f, "}\n");
    fclose(f);
    
    printf("[json_output] Report written to %s\n", output_path);
    return 0;
}

static void json_output_cleanup(void) {
    printf("[json_output] Cleanup\n");
}

/* ============================================================================
 * Plugin Definition
 * ============================================================================ */

static const char* json_capabilities[] = {
    "write_report"
};

forensic_output_plugin_t json_output_plugin = {
    .name = "json_output",
    .version = "1.0.0",
    .description = "Generates JSON report from timeline, IOCs, and provenance",
    .init = json_output_init,
    .generate = json_output_generate,
    .cleanup = json_output_cleanup,
    .required_capabilities = json_capabilities,
    .capability_count = sizeof(json_capabilities) / sizeof(json_capabilities[0])
};