# BYOVD Driver Manifest Auto-Updater

This directory contains the automated system for keeping the BYOVD driver arsenal up-to-date with the latest LOLDrivers catalog and Microsoft blocklist changes.

## Files

| File | Purpose |
|------|---------|
| `driver_manifest.yaml` | Human-readable driver manifest with full capabilities, IOCTL maps, and dependencies |
| `driver_manifest.json` | Machine-readable format for compiler integration |
| `byovd_manifest.h` | C header with compile-time driver selection macros |
| `byovd_manifest.c` | Runtime driver registry with fallback chain |
| `update_driver_manifest.py` | Auto-updater script (fetches LOLDrivers + Microsoft blocklist, flags changes) |
| `driver_update_report.md` | Generated report of each update run |

## Auto-Update (Cron)

A daily cron job runs at 06:00 UTC:

```cron
0 6 * * * cd /home/kamini/projects/sih148/JOCKY && python3 src/runtime/byovd/update_driver_manifest.py --commit >> /tmp/byovd_update.log 2>&1
```

What it does:
1. Fetches latest `drivers.json` from loldrivers.io
2. Loads current Microsoft blocked driver list
3. Checks every driver in our manifest against the blocklist
4. Scans LOLDrivers for new unblocked drivers not yet in our manifest
5. Generates `driver_update_report.md` with findings
6. Commits changes if any blocked drivers or new candidates are found

## Manual Run

```bash
cd /home/kamini/projects/sih148/JOCKY

# Check for updates without committing
python3 src/runtime/byovd/update_driver_manifest.py

# Check for updates and auto-commit
python3 src/runtime/byovd/update_driver_manifest.py --commit

# Use cached data (skip network fetch)
python3 src/runtime/byovd/update_driver_manifest.py --no-fetch
```

## Exit Codes

| Code | Meaning |
|------|---------|
| 0 | Nothing to update |
| 1 | New candidates found (review report) |
| 2 | **CRITICAL: Existing drivers now BLOCKED** |

## Alerting

If the script exits with code 2, one or more drivers in our active manifest have appeared on the Microsoft blocklist. **Immediate action required:**

1. Check `driver_update_report.md`
2. Remove blocked drivers from `driver_manifest.yaml` and `driver_manifest.json`
3. Promote fallback drivers in the compiler selection chain
4. Rebuild any binaries that embedded the blocked driver

## Manifest Structure

Each driver entry includes:

```yaml
- name: driver_name.sys
  sha256: "..."
  device_path: "\\.\\DeviceName"
  service_name: "ServiceName"
  capabilities: [arb_physical_read, arb_physical_write, msr_read, ...]
  ioctl_map:
    map_physical:
      code: 0x82000000
      input_layout:
        offset_0x00: { type: uint64, name: physical_address }
  blocklist_status:
    microsoft_blocked: false
  tested: true
  test_results:
    driver_loaded: true
    blocked_by_defender: false
```

## Adding a New Driver

1. Download and test the driver in the VM
2. Add entry to `driver_manifest.yaml` (human-readable)
3. Add entry to `driver_manifest.json` (compiler consumption)
4. Add to `byovd_manifest.h` and `byovd_manifest.c` (runtime registry)
5. Run `update_driver_manifest.py --no-fetch` to validate
6. Commit changes

## Sources

- LOLDrivers: https://www.loldrivers.io/api/drivers.json
- Microsoft Blocklist: `data/microsoft_blocked_driver_list.json` (updated via download_drivers.py)
