// NAME: Miles Kang
// EMAIL: milesjkang@gmail.com
// ID: 405106565

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <getopt.h>
#include <fcntl.h>
#include <string.h>
#include "ext2_fs.h"

/* Global variables */
int grade = 100;
/* Global variables */

void error_msg(char* message, int exit_code) {

    // Generic error msg.

    fprintf(stderr, "Error %d: %s\n", errno, message);
    exit(exit_code);
}

int main(int argc, char* argv[]) {

    if (argc != 2) {
		error_msg("Invalid number of arguments were passed.", 1);
	}


    exit(0);
}