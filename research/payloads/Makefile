CC := gcc
CFLAGS := -O2 -Wall
STATIC_FLAGS := -static
TARGETS := simple_payload fileless_exec

all: $(TARGETS)

simple_payload: simple_payload.c
	$(CC) $(CFLAGS) $(STATIC_FLAGS) $< -o $@

fileless_exec: fileless_exec.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(TARGETS)

test: all
	@echo "=== Fileless Execution Test ==="
	@echo ""
	@echo "Running payload in memory..."
	./fileless_exec ./simple_payload arg1 arg2 &
	CHILD_PID=$$!
	sleep 1
	@echo ""
	@echo "=== Process visible normally ==="
	ps aux | grep "$$CHILD_PID" | grep -v grep || true
	@echo ""
	@echo "=== Hiding with LD_PRELOAD ==="
	HIDE_PIDS=$$CHILD_PID LD_PRELOAD=../preload/libprocessHider.so ps aux | grep "$$CHILD_PID" | grep -v grep || echo "Process hidden ✓"
	@echo ""
	@echo "Waiting for payload to complete..."
	wait $$CHILD_PID 2>/dev/null || true

.PHONY: all clean test

