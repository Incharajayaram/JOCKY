#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
	printf("=== Fileless Execution Payload ===\n");
	printf("PID: %d\n", getpid());
	printf("PPID: %d\n", getppid());
	printf("Name: %s\n", argv[0]);
	printf("Args: ");

	for (int i = 1; i < argc; i++) {
		printf("%s ", argv[i]);
	}
	printf("\n");

	printf("\nRunning for 60 seconds...\n");
	printf("Try: ps aux | grep %d\n", getpid());
	printf("With LD_PRELOAD hiding, this process should be invisible!\n\n");

	/* Sleep for 60 seconds */
	sleep(60);

	printf("Payload complete.\n");
	return 0;
}
