"""
Ghidra Python Script - IOCTL Extractor for JOCKY Manifest Generator

Place this in Ghidra's script directory or run via:
  ghidra_home/support/analyzeHeadless <project_dir> <project_name> \\
    -import <driver.sys> -postScript ghidra_ioctl_extractor.py

@author JOCKY Manifest Generator
@category Search.InstructionPattern
@keybinding
@toolbar
"""

from ghidra.program.model.address import AddressSet
from ghidra.program.model.listing import CodeUnit
import json
import re

def find_ioctl_codes():
    """Find IOCTL codes used in the driver."""
    ioctls = {}
    seen = set()

    # Get current program
    program = currentProgram
    listing = program.getListing()
    memory = program.getMemory()

    print("[*] Scanning for IOCTL codes...")

    # Search through all instructions
    instr_iter = listing.getInstructions(True)

    for instr in instr_iter:
        mnemonic = instr.getMnemonicString()

        # Look for MOV, LEA, CMP with immediate values (potential IOCTLs)
        if mnemonic in ['MOV', 'movzx', 'CMP', 'LEA', 'cmp']:
            try:
                # Get operand count
                num_operands = instr.getNumOperands()

                for op_idx in range(num_operands):
                    # Check if operand is an immediate
                    operand_type = instr.getOperandType(op_idx)

                    # Type 1 = immediate
                    if operand_type == 1:  # Operand.SCALAR
                        value = instr.getOperandValue(op_idx) & 0xFFFFFFFF

                        # Check if valid IOCTL
                        if is_valid_ioctl(value) and value not in seen:
                            seen.add(value)

                            # Extract IOCTL components
                            method = value & 0x3
                            func = (value >> 2) & 0xFFF
                            device = (value >> 16) & 0xFFFF

                            ioctls[f"0x{value:08x}"] = {
                                'code': value,
                                'hex': f"0x{value:08x}",
                                'address': str(instr.getAddress()),
                                'instruction': instr.toString(),
                                'method': get_method_name(method),
                                'function_code': func,
                                'device_type': f"0x{device:04x}",
                                'device_type_int': device
                            }
            except Exception as e:
                pass

    return ioctls

def is_valid_ioctl(value):
    """Validate that a value follows IOCTL structure."""
    if value < 0x20000:
        return False
    if value > 0xFFFFFFFF:
        return False

    method = value & 0x3
    func = (value >> 2) & 0xFFF
    device = (value >> 16) & 0xFFFF

    # Valid range checks
    if not (0 <= device <= 0xFFFF):
        return False
    if not (0 <= func <= 0xFFF):
        return False
    if not (0 <= method <= 3):
        return False

    return True

def get_method_name(method_bits):
    """Convert IOCTL method bits to name."""
    methods = {
        0: "METHOD_BUFFERED",
        1: "METHOD_IN_DIRECT",
        2: "METHOD_OUT_DIRECT",
        3: "METHOD_NEITHER"
    }
    return methods.get(method_bits & 0x3, "METHOD_BUFFERED")

def find_dispatch_functions():
    """Find likely IOCTL dispatch functions."""
    functions = []

    program = currentProgram
    function_manager = program.getFunctionManager()

    print("[*] Scanning for dispatch functions...")

    for func in function_manager.getFunctions(True):
        func_name = func.getName().lower()

        # Look for common dispatch patterns
        dispatch_patterns = [
            'dispatch', 'ioctl', 'io_control', 'iofcalldriver',
            'dispatchdevicecontrol', 'handleioctl', 'control'
        ]

        if any(pattern in func_name for pattern in dispatch_patterns):
            functions.append({
                'name': func.getName(),
                'address': str(func.getEntryPoint()),
                'size': func.getBody().getNumAddresses()
            })

    return functions

def find_strings_near_ioctls():
    """Find string constants associated with IOCTL handling."""
    results = {}

    program = currentProgram
    listing = program.getListing()
    string_refs = program.getReferenceManager()

    print("[*] Finding related strings...")

    instr_iter = listing.getInstructions(True)

    for instr in instr_iter:
        refs = string_refs.getReferencesFrom(instr.getAddress())
        for ref in refs:
            to_addr = ref.getToAddress()
            unit = listing.getUnitAt(to_addr)

            if unit and unit.isString():
                string_value = unit.toString()
                if string_value:
                    results[str(to_addr)] = string_value

    return results

def infer_capabilities(ioctls):
    """Infer driver capabilities from IOCTLs."""
    capabilities = set()

    for hex_code, ioctl_info in ioctls.items():
        device_type = ioctl_info['device_type_int']
        code = ioctl_info['code']

        # Common device type patterns
        if device_type >= 0x8000:
            capabilities.add('arbitrary_io')

        if (code & 0xFF000000) == 0x80000000:
            capabilities.add('arb_physical_read')
            capabilities.add('arb_physical_write')
        elif (code & 0xFF000000) == 0x90000000:
            capabilities.add('msr_read')
            capabilities.add('msr_write')
        elif (code & 0xFF000000) == 0xA0000000:
            capabilities.add('port_io')

    return sorted(list(capabilities)) if capabilities else []

def extract_pe_metadata():
    """Extract metadata from PE header."""
    program = currentProgram

    metadata = {
        'filename': program.getName(),
        'image_base': str(program.getImageBase()),
        'min_address': str(program.getMinAddress()),
        'max_address': str(program.getMaxAddress()),
        'arch': program.getLanguage().getProcessor().toString(),
        'is_64bit': program.getLanguage().getSize() == 64
    }

    return metadata

def generate_manifest(ioctls, dispatch_funcs, metadata, capabilities):
    """Generate JOCKY manifest JSON."""
    manifest = {
        'name': metadata['filename'].split('/')[-1],
        'sha256': 'unknown',  # Requires external computation
        'arch': 'x64' if metadata['is_64bit'] else 'x86',
        'signed': True,  # Assume signed for now
        'company': '',
        'description': '',
        'evasion_score': 5.0,
        'device_paths': {
            'primary': '',
            'aliases': []
        },
        'service_name': metadata['filename'].split('/')[-1].replace('.sys', ''),
        'init_sequence': ['create_service', 'start_service', 'open_device'],
        'capabilities': capabilities,
        'ioctl_map': {},
        'analysis_method': 'ghidra',
        'confidence': 0.7 if ioctls else 0.3,
        'dispatch_functions': dispatch_funcs,
        'tested': False,
        'source': 'ghidra_ioctl_extractor'
    }

    # Build IOCTL map
    for hex_code, ioctl_info in sorted(ioctls.items()):
        name = f"ioctl_{hex_code[2:]}"  # Remove '0x' prefix
        manifest['ioctl_map'][name] = {
            'code': hex_code,
            'method': ioctl_info['method'],
            'access': 'FILE_ANY_ACCESS',
            'address': ioctl_info['address']
        }

    return manifest

def main():
    """Main extraction routine."""
    print("[*] Starting Ghidra IOCTL Extractor for JOCKY")

    # Extract data
    ioctls = find_ioctl_codes()
    dispatch_funcs = find_dispatch_functions()
    metadata = extract_pe_metadata()
    capabilities = infer_capabilities(ioctls)

    # Generate manifest
    manifest = generate_manifest(ioctls, dispatch_funcs, metadata, capabilities)

    # Output results
    print("[+] Analysis complete!")
    print(f"[+] Found {len(ioctls)} IOCTL codes")
    print(f"[+] Found {len(dispatch_funcs)} potential dispatch functions")
    print(f"[+] Inferred capabilities: {', '.join(capabilities) or '(none)'}")
    print("")
    print("[*] Manifest JSON:")
    print(json.dumps(manifest, indent=2))

    # Save to file for external processing
    output_file = f"/tmp/{metadata['filename'].split('/')[-1]}_manifest.json"
    with open(output_file, 'w') as f:
        json.dump(manifest, f, indent=2)
    print(f"[*] Saved to {output_file}")

# Execute
if __name__ == '__main__':
    main()

# Also run if imported as Ghidra script
try:
    main()
except:
    pass
