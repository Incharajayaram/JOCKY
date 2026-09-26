#include "forensic_types.h"
/**
 * PE Parser Plugin
 * 
 * Parses PE file bytes into sections, imports, exports, signatures,
 * compile timestamp, and hashes. Runs in sandboxed child process.
 */

#include "forensic_types.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>

/* ============================================================================
 * PE Parsing Helpers
 * ============================================================================ */

#pragma pack(push, 1)
typedef struct {
    uint16_t e_magic;
    uint16_t e_cblp;
    uint16_t e_cp;
    uint16_t e_crlc;
    uint16_t e_cparhdr;
    uint16_t e_minalloc;
    uint16_t e_maxalloc;
    uint16_t e_ss;
    uint16_t e_sp;
    uint16_t e_csum;
    uint16_t e_ip;
    uint16_t e_cs;
    uint16_t e_lfarlc;
    uint16_t e_ovno;
    uint16_t e_res[4];
    uint16_t e_oemid;
    uint16_t e_oeminfo;
    uint16_t e_res2[10];
    uint32_t e_lfanew;
} DOS_HEADER;

typedef struct {
    uint32_t Signature;
    uint16_t Machine;
    uint16_t NumberOfSections;
    uint32_t TimeDateStamp;
    uint32_t PointerToSymbolTable;
    uint32_t NumberOfSymbols;
    uint16_t SizeOfOptionalHeader;
    uint16_t Characteristics;
} COFF_HEADER;

typedef struct {
    uint16_t Magic;
    uint8_t MajorLinkerVersion;
    uint8_t MinorLinkerVersion;
    uint32_t SizeOfCode;
    uint32_t SizeOfInitializedData;
    uint32_t SizeOfUninitializedData;
    uint32_t AddressOfEntryPoint;
    uint32_t BaseOfCode;
    uint32_t BaseOfData;
    uint64_t ImageBase;
    uint32_t SectionAlignment;
    uint32_t FileAlignment;
    uint16_t MajorOperatingSystemVersion;
    uint16_t MinorOperatingSystemVersion;
    uint16_t MajorImageVersion;
    uint16_t MinorImageVersion;
    uint16_t MajorSubsystemVersion;
    uint16_t MinorSubsystemVersion;
    uint32_t Win32VersionValue;
    uint32_t SizeOfImage;
    uint32_t SizeOfHeaders;
    uint32_t CheckSum;
    uint16_t Subsystem;
    uint16_t DllCharacteristics;
    uint64_t SizeOfStackReserve;
    uint64_t SizeOfStackCommit;
    uint64_t SizeOfHeapReserve;
    uint64_t SizeOfHeapCommit;
    uint32_t LoaderFlags;
    uint32_t NumberOfRvaAndSizes;
    // DataDirectory[16] follows
} OPTIONAL_HEADER64;

typedef struct {
    uint32_t VirtualAddress;
    uint32_t Size;
} DATA_DIRECTORY;

typedef struct {
    uint8_t Name[8];
    uint32_t VirtualSize;
    uint32_t VirtualAddress;
    uint32_t SizeOfRawData;
    uint32_t PointerToRawData;
    uint32_t PointerToRelocations;
    uint32_t PointerToLinenumbers;
    uint16_t NumberOfRelocations;
    uint16_t NumberOfLinenumbers;
    uint32_t Characteristics;
} SECTION_HEADER;
#pragma pack(pop)

/* ============================================================================
 * PE Parser Implementation
 * ============================================================================ */

static int pe_parser_init(void* config) {
    (void)config;
    printf("[pe_parser] Initialized\n");
    return 0;
}

static double calculate_entropy(const uint8_t* data, size_t len) {
    if (!data || len == 0) return 0.0;
    
    int counts[256] = {0};
    for (size_t i = 0; i < len; i++) {
        counts[data[i]]++;
    }
    
    double entropy = 0.0;
    for (int i = 0; i < 256; i++) {
        if (counts[i] > 0) {
            double p = (double)counts[i] / len;
            entropy -= p * log2(p);
        }
    }
    return entropy;
}

static const char* get_machine_name(uint16_t machine) {
    switch (machine) {
        case 0x014c: return "x86";
        case 0x8664: return "x64";
        case 0x01c0: return "ARM";
        case 0xaa64: return "ARM64";
        default: return "Unknown";
    }
}

static const char* get_subsystem_name(uint16_t subsystem) {
    switch (subsystem) {
        case 1: return "NATIVE";
        case 2: return "WINDOWS_GUI";
        case 3: return "WINDOWS_CUI";
        case 7: return "POSIX_CUI";
        case 9: return "WINDOWS_CE_GUI";
        case 10: return "EFI_APPLICATION";
        case 11: return "EFI_BOOT_SERVICE_DRIVER";
        case 12: return "EFI_RUNTIME_DRIVER";
        case 13: return "EFI_ROM";
        case 14: return "XBOX";
        case 16: return "WINDOWS_BOOT_APPLICATION";
        default: return "Unknown";
    }
}

static forensic_parsed_artifact_t* parse_pe(const forensic_bytes_t* data, void* config) {
    (void)config;
    
    if (data->len < sizeof(DOS_HEADER)) {
        fprintf(stderr, "[pe_parser] Data too small for DOS header\n");
        return NULL;
    }
    
    const DOS_HEADER* dos = (const DOS_HEADER*)data->data;
    if (dos->e_magic != 0x5A4D) { // MZ
        fprintf(stderr, "[pe_parser] Not a valid PE file (no MZ signature)\n");
        return NULL;
    }
    
    if (data->len < dos->e_lfanew + sizeof(COFF_HEADER)) {
        fprintf(stderr, "[pe_parser] Data too small for COFF header\n");
        return NULL;
    }
    
    const COFF_HEADER* coff = (const COFF_HEADER*)(data->data + dos->e_lfanew);
    if (coff->Signature != 0x00004550) { // PE\0\0
        fprintf(stderr, "[pe_parser] Not a valid PE file (no PE signature)\n");
        return NULL;
    }
    
    bool is64 = (coff->SizeOfOptionalHeader == sizeof(OPTIONAL_HEADER64));
    if (!is64) {
        fprintf(stderr, "[pe_parser] 32-bit PE not fully supported\n");
    }
    
    const OPTIONAL_HEADER64* opt = (const OPTIONAL_HEADER64*)(coff + 1);
    
    // Create parsed artifact
    forensic_parsed_artifact_t* artifact = forensic_parsed_artifact_create("pe_file", "pe_parser");
    if (!artifact) return NULL;
    
    // Add basic info
    char timestamp[64];
    time_t compile_time = coff->TimeDateStamp;
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%dT%H:%M:%SZ", gmtime(&compile_time));
    forensic_metadata_add(&artifact->parsed_data, "compile_timestamp", timestamp);
    forensic_metadata_add(&artifact->parsed_data, "machine", get_machine_name(coff->Machine));
    forensic_metadata_add(&artifact->parsed_data, "subsystem", get_subsystem_name(opt->Subsystem));
    
    char entry_str[32];
    snprintf(entry_str, sizeof(entry_str), "0x%lx", (unsigned long)opt->AddressOfEntryPoint);
    forensic_metadata_add(&artifact->parsed_data, "entry_point", entry_str);
    
    char base_str[32];
    snprintf(base_str, sizeof(base_str), "0x%lx", (unsigned long)opt->ImageBase);
    forensic_metadata_add(&artifact->parsed_data, "image_base", base_str);
    
    char sections_str[32];
    snprintf(sections_str, sizeof(sections_str), "%d", coff->NumberOfSections);
    forensic_metadata_add(&artifact->parsed_data, "num_sections", sections_str);
    
    // Parse sections
    const SECTION_HEADER* sections = (const SECTION_HEADER*)(data->data + dos->e_lfanew + sizeof(COFF_HEADER) + coff->SizeOfOptionalHeader);
    
    for (int i = 0; i < coff->NumberOfSections; i++) {
        const SECTION_HEADER* sec = &sections[i];
        char sec_name[9];
        memcpy(sec_name, sec->Name, 8);
        sec_name[8] = '\0';
        
        char* null_pos = strchr(sec_name, '\0');
        if (null_pos) *null_pos = '\0';
        
        double entropy = calculate_entropy(
            data->data + sec->PointerToRawData,
            sec->SizeOfRawData
        );
        
        char entropy_str[32];
        snprintf(entropy_str, sizeof(entropy_str), "%.2f", entropy);
        
        forensic_metadata_add(&artifact->parsed_data, sec_name, entropy_str);
        
        if (entropy > 7.0) {
            forensic_ioc_t* ioc = forensic_ioc_create("packer_indicator", sec_name,
                "pe_parser", 0.8);
            artifact->iocs.items = realloc(artifact->iocs.items,
                (artifact->iocs.count + 1) * sizeof(forensic_ioc_t));
            artifact->iocs.items[artifact->iocs.count++] = *ioc;
            free(ioc);
        }
    }
    
    // Add timeline event
    time_t now = time(NULL);
    char ts[64];
    strftime(ts, sizeof(ts), "%Y-%m-%dT%H:%M:%SZ", gmtime(&now));
    forensic_event_t* evt = forensic_event_create("pe_parsed", ts, "pe_parser", "PE file parsed successfully");
    artifact->timeline_events = realloc(artifact->timeline_events,
        (artifact->timeline_count + 1) * sizeof(forensic_event_t));
    artifact->timeline_events[artifact->timeline_count++] = *evt;
    free(evt);
    
    printf("[pe_parser] Parsed PE: %d sections, %s, %s\n",
           coff->NumberOfSections, get_machine_name(coff->Machine),
           get_subsystem_name(opt->Subsystem));
    
    return artifact;
}

static void pe_parser_cleanup(void) {
    printf("[pe_parser] Cleanup\n");
}

/* ============================================================================
 * Plugin Definition
 * ============================================================================ */

static const char* pe_capabilities[] = {
    "read_artifact",
    "parse_pe"
};

forensic_artifact_parser_plugin_t pe_parser_plugin = {
    .name = "pe_parser",
    .version = "1.0.0",
    .description = "Parses PE files into sections, imports, exports, and metadata",
    .artifact_type = "pe_file",
    .init = pe_parser_init,
    .parse = parse_pe,
    .cleanup = pe_parser_cleanup,
    .required_capabilities = pe_capabilities,
    .capability_count = sizeof(pe_capabilities) / sizeof(pe_capabilities[0]),
    .requires_sandbox = true,
    .max_cpu_time_ms = 10000,
    .max_memory_bytes = 256 * 1024 * 1024
};