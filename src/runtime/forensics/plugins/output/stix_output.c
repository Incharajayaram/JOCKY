#include "../../forensic_types.h"
/**
 * STIX 2.1 Output Plugin
 * 
 * Generates STIX 2.1 bundle from analysis results.
 */

#include "../../forensic_types.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* ============================================================================
 * STIX Output Implementation
 * ============================================================================ */

static int stix_output_init(void* config) {
    (void)config;
    printf("[stix_output] Initialized\n");
    return 0;
}

static const char* map_artifact_to_stix_type(const char* artifact_type) {
    if (strcmp(artifact_type, "process") == 0) return "process";
    if (strcmp(artifact_type, "file") == 0) return "file";
    if (strcmp(artifact_type, "network_connection") == 0) return "network-traffic";
    if (strcmp(artifact_type, "registry") == 0) return "windows-registry-key";
    return "observed-data";
}

static int stix_output_generate(const forensic_timeline_t* timeline,
                                 const forensic_ioc_list_t* iocs,
                                 const forensic_metadata_t* provenance,
                                 void* config) {
    (void)provenance;
    
    const char* output_dir = config ? (const char*)config : ".";
    char output_path[512];
    snprintf(output_path, sizeof(output_path), "%s/forensic_report.stix.json", output_dir);
    FILE* f = fopen(output_path, "w");
    if (!f) {
        fprintf(stderr, "[stix_output] Failed to open output file: %s\n", output_path);
        return -1;
    }
    
    fprintf(f, "{\n");
    fprintf(f, "  \"type\": \"bundle\",\n");
    fprintf(f, "  \"id\": \"bundle--%ld\",\n", (long)time(NULL));
    fprintf(f, "  \"spec_version\": \"2.1\",\n");
    fprintf(f, "  \"objects\": [\n");
    
    bool first = true;
    
    for (size_t i = 0; i < timeline->count; i++) {
        const forensic_event_t* evt = &timeline->items[i];
        if (!first) fprintf(f, ",\n");
        first = false;
        
        fprintf(f, "    {\n");
        fprintf(f, "      \"type\": \"observed-data\",\n");
        fprintf(f, "      \"id\": \"observed-data--%ld-%zu\",\n", (long)time(NULL), i);
        fprintf(f, "      \"created\": \"%s\",\n", evt->timestamp ? evt->timestamp : "");
        fprintf(f, "      \"modified\": \"%s\",\n", evt->timestamp ? evt->timestamp : "");
        fprintf(f, "      \"first_observed\": \"%s\",\n", evt->timestamp ? evt->timestamp : "");
        fprintf(f, "      \"last_observed\": \"%s\",\n", evt->timestamp ? evt->timestamp : "");
        fprintf(f, "      \"number_observed\": 1,\n");
        fprintf(f, "      \"objects\": {\n");
        fprintf(f, "        \"0\": {\n");
        fprintf(f, "          \"type\": \"%s\",\n", map_artifact_to_stix_type(evt->event_type));
        fprintf(f, "          \"name\": \"%s\",\n", evt->description ? evt->description : "");
        fprintf(f, "          \"description\": \"%s\"\n", evt->source_plugin ? evt->source_plugin : "");
        fprintf(f, "        }\n");
        fprintf(f, "      }\n");
        fprintf(f, "    }");
    }
    
    for (size_t i = 0; i < iocs->count; i++) {
        const forensic_ioc_t* ioc = &iocs->items[i];
        if (!first) fprintf(f, ",\n");
        first = false;
        
        fprintf(f, "    {\n");
        fprintf(f, "      \"type\": \"indicator\",\n");
        fprintf(f, "      \"id\": \"indicator--%ld-%zu\",\n", (long)time(NULL), i);
        fprintf(f, "      \"created\": \"%ld\",\n", (long)time(NULL));
        fprintf(f, "      \"modified\": \"%ld\",\n", (long)time(NULL));
        fprintf(f, "      \"pattern_type\": \"stix\",\n");
        fprintf(f, "      \"pattern\": \"[%s:value = '%s']\",\n", 
                ioc->type ? ioc->type : "file", ioc->value ? ioc->value : "");
        fprintf(f, "      \"valid_from\": \"%ld\"\n", (long)time(NULL));
        fprintf(f, "    }");
    }
    
    fprintf(f, "\n  ]\n");
    fprintf(f, "}\n");
    fclose(f);
    
    printf("[stix_output] STIX bundle written to %s\n", output_path);
    return 0;
}

static void stix_output_cleanup(void) {
    printf("[stix_output] Cleanup\n");
}

/* ============================================================================
 * Plugin Definition
 * ============================================================================ */

static const char* stix_capabilities[] = {
    "write_report"
};

forensic_output_plugin_t stix_output_plugin = {
    .name = "stix_output",
    .version = "1.0.0",
    .description = "Generates STIX 2.1 bundle from timeline and IOCs",
    .init = stix_output_init,
    .generate = stix_output_generate,
    .cleanup = stix_output_cleanup,
    .required_capabilities = stix_capabilities,
    .capability_count = sizeof(stix_capabilities) / sizeof(stix_capabilities[0])
};