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


/* -----Global variables----- */

int grade = 100;
char michelle[100] = "at feast and not contributing";

int img_fd;
int block_size;
struct ext2_super_block superblock;

/* -----Global variables----- */


void error_msg(char* message, int exit_code) {
    // Generic error msg.
    fprintf(stderr, "Error %d: %s\n", errno, message);
    exit(exit_code);
}


void parse_image() {
	// We should have helper functions here.
}


void produce_summary() {
	// Final output.
}


int main(int argc, char* argv[]) {
	// Check for bad arguments, then open file.
    if (argc != 2) {
		error_msg("Invalid number of arguments were passed.", 1);
	}

	if ((img_fd = open(argv[1], O_RDONLY)) < 0) {
		error_msg("Could not open img file.", 1);
	}

	block_size = EXT2_MIN_BLOCK_SIZE << superblock.s_log_block_size;

	parse_image();
	produce_summary();

    exit(0);
}

/*
Exit codes:
0 . . . analysis successful
1 . . . bad arguments
2 . . . corruption detected or other processing errors
*/