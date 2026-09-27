obj-m += kernel_evasion.o
kernel_evasion-objs := main.o

KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean

load:
	sudo insmod kernel_evasion.ko

unload:
	sudo rmmod kernel_evasion || true

reload: unload load

# Test: hide a sleep process and verify
test: load
	@sleep 1000 & \
	echo $$! > /tmp/hidden_pid; \
	PID=$$(cat /tmp/hidden_pid); \
	echo "Hiding PID $$PID"; \
	echo $$PID | sudo tee /sys/kernel/kernel_evasion/hide_pid > /dev/null; \
	echo "--- Checking if PID is hidden from ls /proc ---"; \
	if ls /proc | grep "^$$PID$$" > /dev/null; then \
		echo "FAIL: PID still visible"; \
	else \
		echo "PASS: PID successfully hidden"; \
	fi; \
	echo "--- Verifying process still exists (kill -0) ---"; \
	if kill -0 $$PID 2>/dev/null; then \
		echo "PASS: Process still alive"; \
	else \
		echo "FAIL: Process disappeared"; \
	fi; \
	echo "Cleaning up..."; \
	kill $$PID 2>/dev/null; \
	rm /tmp/hidden_pid

.PHONY: all clean load unload reload test
