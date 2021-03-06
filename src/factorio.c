#include "factorio.h"

#include "log.h"

#include <ucw/lib.h>

#include <fcntl.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

void run_factorio(char* fac_bin, char* done_file, char* save_name) {
	log("Starting Factorio (%s).", fac_bin);

	pid_t fk = fork();
	if (fk == 0) { // child process
		int dev_null = open("/dev/null", O_WRONLY);
		dup2(dev_null, 1); // redirect factorio output to /dev/null

		if (save_name == NULL)
			execl(fac_bin, "", NULL);
		else
			execl(fac_bin, "", "--load-game", save_name, NULL);

		ASSERT(false); // we should never get here
	} else if (fk > 0) { // parent process
		while (1) {
			log("Checking the done file (%s).", done_file);

			sleep(1);
			FILE* done_fd = fopen(done_file, "r");

			if (done_fd != NULL) {
				log("Done file found. Breaking the loop.");
				fclose(done_fd);
				remove(done_file);
				kill(fk, SIGTERM);
				break;
			}
		}
	} else if (fk == -1)
		die("Failed to fork.");
}

