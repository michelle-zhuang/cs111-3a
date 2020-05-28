// NAME: Miles Kang,Michelle Zhuang
// EMAIL: milesjkang@gmail.com,michelle.zhuang@g.ucla.edu
// ID: 405106565,505143435

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <getopt.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/uio.h>
#include "ext2_fs.h"


/* -----Global variables----- */

int grade = 100;
char michelle[100] = "at feast and not contributing";

#define SUPERBLOCK_OFFSET 1024
int img_fd;
int block_size;
struct ext2_super_block superblock;


/* -----Global variables----- */


void error_msg(char* message, int exit_code) {
    // Generic error msg.
    fprintf(stderr, "Error %d: %s\n", errno, message);
    exit(exit_code);
}


void log_superblock() {
	// Parses information from the superblock.
    if (pread(img_fd, &superblock, sizeof(superblock), SUPERBLOCK_OFFSET) < 0){
        error_msg("pread failure.", 2);
    }
    block_size = EXT2_MIN_BLOCK_SIZE << superblock.s_log_block_size;
    
    printf("SUPERBLOCK,%d,%d,%d,%d,%d,%d,%d\n",
           superblock.s_blocks_count,
           superblock.s_inodes_count,
           block_size,
           superblock.s_inode_size,
           superblock.s_blocks_per_group,
           superblock.s_inodes_per_group,
           superblock.s_first_ino);
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

	log_superblock();
	produce_summary();

    exit(0);
}

/*
Exit codes:
0 . . . analysis successful
1 . . . bad arguments
2 . . . corruption detected or other processing errors
*/
