"""
IDA Pro Python Script - IOCTL Extractor for JOCKY Manifest Generator

To use this script:
1. Open a driver binary in IDA Pro
2. File -> Script file... -> Select this script
3. Results will be printed to the IDA console

Or run from command line (IDA Pro 7.0+):
  idat.exe -A -S<script_path> driver.sys
"""

import idaapi
import idautils
import idc
import json
from collections import defaultdict

def find_device_io_control_calls():
    """Find all calls to DeviceIoControl / IofCallDriver."""
    results = []

    # Search for patterns that indicate IOCTL handling
    # Common patterns:
    # - mov eax, 0x<ioctl_code>
    # - lea rax, [rip + ioctl_table]
    # - switch on IOCTL code

    device_io_control = idc.get_name_ea_simple("DeviceIoControl")
    iof_call_driver = idc.get_name_ea_simple("IofCallDriver")

    for ea in idautils.Functions():
        func_name = idc.get_func_name(ea)

        # Look for IOCTL dispatch function patterns
        if 'dispatch' in func_name.lower() or 'ioctl' in func_name.lower():
            results.append({
                'type': 'dispatch_function',
                'name': func_name,
                'address': ea
            })

    return results

def find_ioctl_values():
    """Find IOCTL codes in the binary."""
    ioctls = defaultdict(list)

    for ea in idautils.Segments():
        for addr in range(ea, idc.get_segm_end(ea)):
            # Look for MOV instructions with 4-byte immediates
            instr = idautils.DecodeInstruction(addr)

            if instr and instr.itype in [idaapi.NN_mov, idaapi.NN_lea]:
                try:
                    # Check for immediate operands
                    for op_idx in range(3):
                        op = instr.operands[op_idx]
                        if op.type == idaapi.o_imm:
                            value = op.value & 0xFFFFFFFF

                            # Validate IOCTL structure
                            if is_valid_ioctl(value):
                                method = value & 0x3
                                func = (value >> 2) & 0xFFF
                                device = (value >> 16) & 0xFFFF

                                ioctls[value].append({
                                    'address': addr,
                                    'instruction': idc.GetDisasm(addr),
                                    'method': get_method_name(method),
                                    'function': func,
                                    'device_type': device
                                })
                except:
                    pass

    return dict(ioctls)

def is_valid_ioctl(value):
    """Validate IOCTL code structure."""
    if value < 0x20000 or value > 0xFFFFFFFF:
        return False

    method = value & 0x3
    func = (value >> 2) & 0xFFF
    device = (value >> 16) & 0xFFFF

    return (0 <= device <= 0xFFFF and
            0 <= func <= 0xFFF and
            0 <= method <= 3)

def get_method_name(method):
    """Get CTL_CODE method name from code."""
    methods = {
        0: "METHOD_BUFFERED",
        1: "METHOD_IN_DIRECT",
        2: "METHOD_OUT_DIRECT",
        3: "METHOD_NEITHER"
    }
    return methods.get(method & 0x3, "METHOD_BUFFERED")

def find_string_references(ioctl_code):
    """Find string constants near IOCTL code usage."""
    strings = []

    # Search for references to this IOCTL value
    for xref in idautils.XrefsTo(ioctl_code, idaapi.XREF_ALL):
        ea = xref.frm

        # Look for nearby strings
        for offset in range(-0x100, 0x100, 4):
            try:
                s = idc.get_strlit_contents(ea + offset)
                if s and len(s) > 3:
                    strings.append(s.decode('utf-8', errors='ignore'))
            except:
                pass

    return strings

def analyze_switch_statements():
    """Analyze switch statements for IOCTL dispatch patterns."""
    results = []

    # Use IDA's jump tables to find switch statements
    for addr in idautils.Heads():
        if idc.get_mnem(addr) in ['jmpq', 'jmp', 'jmpt']:
            # Potential jump table
            try:
                targets = idautils.CodeRefs(addr, False)
                if list(targets):
                    results.append({
                        'type': 'switch_dispatch',
                        'address': addr,
                        'targets': list(targets)
                    })
            except:
                pass

    return results

def extract_driver_info():
    """Extract driver metadata from PE headers."""
    info = {
        'filename': idc.get_root_filename(),
        'entry_point': idc.get_imagebase() + idc.get_entrypoint(),
        'segments': []
    }

    # Get all segments
    for seg_ea in idautils.Segments():
        seg = idaapi.get_segment(seg_ea)
        info['segments'].append({
            'name': idc.get_segm_name(seg_ea),
            'start': seg.start_ea,
            'end': seg.end_ea,
            'size': seg.end_ea - seg.start_ea,
            'class': seg.sclass
        })

    return info

def generate_manifest(ioctls_dict, dispatch_functions, driver_info):
    """Generate JOCKY manifest structure."""
    manifest = {
        'filename': driver_info['filename'],
        'arch': 'x64',  # Detect from PE
        'signed': True,
        'company': '',
        'description': '',
        'device_paths': {
            'primary': '',
            'aliases': []
        },
        'service_name': driver_info['filename'].replace('.sys', ''),
        'init_sequence': ['create_service', 'start_service', 'open_device'],
        'capabilities': infer_capabilities(ioctls_dict),
        'ioctl_map': {}
    }

    # Build IOCTL map
    for code, locations in sorted(ioctls_dict.items()):
        name = f"ioctl_{code:08x}"
        manifest['ioctl_map'][name] = {
            'code': f"0x{code:08x}",
            'method': locations[0]['method'],
            'access': 'FILE_ANY_ACCESS',
            'locations': [{'address': f"0x{l['address']:08x}", 'instruction': l['instruction']}
                         for l in locations[:3]]  # Limit to 3 examples
        }

    return manifest

def infer_capabilities(ioctls_dict):
    """Infer driver capabilities from IOCTL patterns."""
    capabilities = set()

    # Map IOCTL device types to capabilities
    device_patterns = {
        0x8800: 'arbitrary_io',
        0x2200: 'memory_access',
        0x4400: 'msr_access',
        0x6600: 'port_io'
    }

    for code in ioctls_dict.keys():
        device = (code >> 16) & 0xFFFF

        if device == 0x8000 or (code & 0xFF000000) == 0x80000000:
            capabilities.add('arb_physical_read')
            capabilities.add('arb_physical_write')
        elif device == 0x9000:
            capabilities.add('msr_read')
            capabilities.add('msr_write')
        elif code > 0xA0000000:
            capabilities.add('arbitrary_io')

    return sorted(list(capabilities)) if capabilities else ['unknown']

def export_results(manifest, ioctls_dict):
    """Export results to console and file."""
    output = {
        'manifest': manifest,
        'ioctl_count': len(ioctls_dict),
        'ioctls': {f"0x{code:08x}": {
            'method': loc[0]['method'],
            'locations': len(loc)
        } for code, loc in ioctls_dict.items()}
    }

    # Print to IDA console
    print("[+] JOCKY Manifest Generator Results")
    print("[+] Found {} IOCTLs".format(len(ioctls_dict)))
    print("[+] Capabilities: {}".format(', '.join(manifest['capabilities'])))
    print("")
    print("Manifest JSON:")
    print(json.dumps(manifest, indent=2))

    return output

def main():
    """Main extraction routine."""
    print("[*] Starting IDA IOCTL extractor for JOCKY")

    # Extract all information
    print("[*] Analyzing driver...")
    driver_info = extract_driver_info()

    print("[*] Finding IOCTL values...")
    ioctls = find_ioctl_values()

    print("[*] Finding dispatch functions...")
    dispatch = find_device_io_control_calls()

    print("[*] Analyzing switch statements...")
    switches = analyze_switch_statements()

    # Generate manifest
    print("[*] Generating manifest...")
    manifest = generate_manifest(ioctls, dispatch, driver_info)

    # Export
    results = export_results(manifest, ioctls)

    print("[+] Complete!")
    print("[*] Save results from IDA console for processing")

# Run main if executed as script
if __name__ == "idapython":
    main()
elif idaapi.is_ida_pro():
    main()
else:
    # Fallback for command-line execution
    main()
