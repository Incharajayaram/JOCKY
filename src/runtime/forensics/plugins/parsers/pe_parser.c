#include "../../forensic_types.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#pragma pack(push, 1)
typedef struct { uint16_t e_magic; uint8_t _pad[58]; int32_t e_lfanew; } PE_DosHeader;
typedef struct { uint16_t Machine; uint16_t NumberOfSections; uint32_t TimeDateStamp;
                 uint32_t PointerToSymbolTable; uint32_t NumberOfSymbols;
                 uint16_t SizeOfOptionalHeader; uint16_t Characteristics; } PE_FileHeader;
typedef struct { uint16_t Magic; uint8_t MajorLinkerVersion; uint8_t MinorLinkerVersion;
                 uint32_t SizeOfCode; uint32_t SizeOfInitializedData;
                 uint32_t SizeOfUninitializedData; uint32_t AddressOfEntryPoint;
                 uint32_t BaseOfCode; } PE_OptionalHeaderBase;
typedef struct { char Name[8]; uint32_t VirtualSize; uint32_t VirtualAddress;
                 uint32_t SizeOfRawData; uint32_t PointerToRawData;
                 uint32_t PointerToRelocations; uint32_t PointerToLinenumbers;
                 uint16_t NumberOfRelocations; uint16_t NumberOfLinenumbers;
                 uint32_t Characteristics; } PE_SectionHeader;
#pragma pack(pop)

static int pe_parser_init(void* config) { (void)config; return 0; }
static void pe_parser_cleanup(void) {}

static forensic_parsed_artifact_t* parse_pe(const forensic_bytes_t* data, void* config) {
    (void)config;
    if (!data || data->len < sizeof(PE_DosHeader) + 4 + sizeof(PE_FileHeader)) return NULL;

    const uint8_t* buf = (const uint8_t*)data->data;
    size_t len = data->len;

    const PE_DosHeader* dos = (const PE_DosHeader*)buf;
    if (dos->e_magic != 0x5A4D) return NULL;  /* MZ */

    int32_t pe_offset = dos->e_lfanew;
    if (pe_offset < 0 || (size_t)pe_offset + 4 + sizeof(PE_FileHeader) > len) return NULL;

    uint32_t sig;
    memcpy(&sig, buf + pe_offset, 4);
    if (sig != 0x00004550) return NULL;  /* PE\0\0 */

    const PE_FileHeader* fhdr = (const PE_FileHeader*)(buf + pe_offset + 4);

    forensic_parsed_artifact_t* result = forensic_parsed_artifact_create("pe_file", "pe_parser");
    if (!result) return NULL;

    char val[64];
    snprintf(val, sizeof(val), "%04x", fhdr->Machine);
    forensic_metadata_add(&result->parsed_data, "machine", val);
    snprintf(val, sizeof(val), "%u", fhdr->NumberOfSections);
    forensic_metadata_add(&result->parsed_data, "section_count", val);
    snprintf(val, sizeof(val), "%u", fhdr->TimeDateStamp);
    forensic_metadata_add(&result->parsed_data, "timestamp", val);
    snprintf(val, sizeof(val), "%04x", fhdr->Characteristics);
    forensic_metadata_add(&result->parsed_data, "characteristics", val);

    /* Optional header magic: 0x10b = PE32, 0x20b = PE32+ */
    size_t opt_offset = pe_offset + 4 + sizeof(PE_FileHeader);
    if (opt_offset + sizeof(PE_OptionalHeaderBase) <= len) {
        const PE_OptionalHeaderBase* opt = (const PE_OptionalHeaderBase*)(buf + opt_offset);
        forensic_metadata_add(&result->parsed_data, "format",
                              opt->Magic == 0x20b ? "PE32+" : "PE32");
        snprintf(val, sizeof(val), "%08x", opt->AddressOfEntryPoint);
        forensic_metadata_add(&result->parsed_data, "entrypoint", val);
    }

    /* Enumerate section names */
    size_t sect_base = opt_offset + fhdr->SizeOfOptionalHeader;
    for (uint16_t i = 0; i < fhdr->NumberOfSections; i++) {
        size_t off = sect_base + i * sizeof(PE_SectionHeader);
        if (off + sizeof(PE_SectionHeader) > len) break;
        const PE_SectionHeader* sec = (const PE_SectionHeader*)(buf + off);
        char name[16] = {0};
        memcpy(name, sec->Name, 8);
        char key[32];
        snprintf(key, sizeof(key), "section_%u", i);
        forensic_metadata_add(&result->parsed_data, key, name);
    }

    return result;
}

static const char* pe_parser_capabilities[] = { "read_artifact", "parse_pe" };

forensic_artifact_parser_plugin_t pe_parser_plugin = {
    .name = "pe_parser",
    .version = "1.0.0",
    .description = "Parses PE/PE32+ binaries into headers, sections, and metadata",
    .artifact_type = "pe_file",
    .init = pe_parser_init,
    .parse = parse_pe,
    .cleanup = pe_parser_cleanup,
    .required_capabilities = pe_parser_capabilities,
    .capability_count = sizeof(pe_parser_capabilities) / sizeof(pe_parser_capabilities[0]),
    .requires_sandbox = true,
    .max_cpu_time_ms = 5000,
    .max_memory_bytes = 64 * 1024 * 1024
};
