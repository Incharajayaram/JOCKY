/*
 * BYOVD Driver Manifest Integration
 * JOCKY Compiler Runtime Header
 *
 * This header provides compile-time access to the driver manifest
 * so the compiler can generate correct code for loading and invoking
 * the selected driver.
 */

#ifndef BYOVD_MANIFEST_H
#define BYOVD_MANIFEST_H

#ifdef __cplusplus
extern "C" {
#endif

/* Driver selection priority (fallback chain) */
#define BYOVD_DRIVER_CHAIN \
    BYOVD_DRIVER("rtkiow10x64.sys",    0x82000000, "\\\\.\\RTCore64") \
    BYOVD_DRIVER("rtkiow8x64.sys",     0x82000000, "\\\\.\\RTCore64") \
    BYOVD_DRIVER("AMDRyzenMasterDriver.sys", 0x81000000, "\\\\.\\AMDRyzenMasterDriver") \
    BYOVD_DRIVER("nvflsh64.sys",       0x80002000, "\\\\.\\nvflsh64") \
    BYOVD_DRIVER("speedfan.sys",       0x80002000, "\\\\.\\speedfan") \
    BYOVD_DRIVER("ene.sys",            0x85000000, "\\\\.\\EneIo") \
    BYOVD_DRIVER("iQVW64.SYS",         0x80802000, "\\\\.\\Nal") \
    BYOVD_DRIVER("UCOREW64.SYS",       0x88000000, "\\\\.\\Global\\") \
    BYOVD_DRIVER("NTIOLib.sys",        0x84002000, "\\\\.\\NTIOLib")

/* IOCTL definitions for primary driver (rtkiow10x64) */
#define BYOVD_IOCTL_MAP_PHYS        0x82000000
#define BYOVD_IOCTL_UNMAP_PHYS      0x82000004
#define BYOVD_IOCTL_READ_MSR        0x82000008
#define BYOVD_IOCTL_WRITE_MSR       0x8200000C
#define BYOVD_IOCTL_READ_PORT       0x82000010
#define BYOVD_IOCTL_WRITE_PORT      0x82000014

/* IOCTL definitions for AMD driver */
#define BYOVD_AMD_IOCTL_MAP_PHYS    0x81000000
#define BYOVD_AMD_IOCTL_UNMAP_PHYS  0x81000004
#define BYOVD_AMD_IOCTL_READ_MSR    0x81000008
#define BYOVD_AMD_IOCTL_WRITE_MSR   0x8100000C

/* IOCTL definitions for Intel/NVIDIA/generic */
#define BYOVD_GEN_IOCTL_MAP_PHYS    0x80002000
#define BYOVD_GEN_IOCTL_UNMAP_PHYS  0x80002004
#define BYOVD_GEN_IOCTL_READ_MSR    0x80002008

/* IOCTL definitions for ENE */
#define BYOVD_ENE_IOCTL_MAP_PHYS    0x85000000
#define BYOVD_ENE_IOCTL_UNMAP_PHYS  0x85000004

/* IOCTL definitions for MSI NTIOLib */
#define BYOVD_MSI_IOCTL_MAP_PHYS    0x84002000
#define BYOVD_MSI_IOCTL_UNMAP_PHYS  0x84002004

/* Input/output buffer layouts */
struct byovd_map_request {
    unsigned long long physical_address;
    unsigned int       size;
    unsigned int       _padding;
    unsigned long long virtual_address;  /* OUT */
};

struct byovd_msr_request {
    unsigned int       index;
    unsigned int       _padding;
    unsigned long long value;
};

struct byovd_port_request {
    unsigned short     port;
    unsigned int       size;    /* 1, 2, or 4 */
    unsigned int       value;
};

/* Capability flags */
#define BYOVD_CAP_PHYS_READ     0x0001
#define BYOVD_CAP_PHYS_WRITE    0x0002
#define BYOVD_CAP_MSR_READ      0x0004
#define BYOVD_CAP_MSR_WRITE     0x0008
#define BYOVD_CAP_PORT_READ     0x0010
#define BYOVD_CAP_PORT_WRITE    0x0020
#define BYOVD_CAP_MMIO_READ     0x0040
#define BYOVD_CAP_MMIO_WRITE    0x0080
#define BYOVD_CAP_PCI_READ      0x0100
#define BYOVD_CAP_PCI_WRITE     0x0200
#define BYOVD_CAP_PROC_KILL     0x0400

/* Runtime driver info structure */
struct byovd_driver_info {
    const char*        name;
    const char*        device_path;
    unsigned int       map_ioctl;
    unsigned int       unmap_ioctl;
    unsigned int       read_msr_ioctl;
    unsigned int       write_msr_ioctl;
    unsigned int       capabilities;
    int                requires_amd_cpu;
    int                requires_intel_nic;
    int                requires_nvidia_gpu;
};

/* Driver registry (compile-time and runtime) */
extern const struct byovd_driver_info g_byovd_drivers[];
extern const int g_byovd_driver_count;

#ifdef __cplusplus
}
#endif

#endif /* BYOVD_MANIFEST_H */
