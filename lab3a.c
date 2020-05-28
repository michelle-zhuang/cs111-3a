// NAME: Miles Kang,Michelle Zhuang
// EMAIL: milesjkang@gmail.com,michelle.zhuang@g.ucla.edu
// ID: 405106565,505143435

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <time.h>
#include "ext2_fs.h"

/* -----Global variables----- */

#define SUPERBLOCK_OFFSET 1024
int img_fd;
int block_size, block_count, inode_count;
struct ext2_super_block superblock;
struct ext2_group_desc group;

/* -----Global variables----- */

void error_msg(char* message, int exit_code) {
    // Generic error msg.
    fprintf(stderr, "Error %d: %s\n", errno, message);
    exit(exit_code);
}

char get_file_type(__u16 i_mode) {
    if (i_mode == 0xA000) {
        return 's';
    }
    else if (i_mode == 0x4000) {
        return 'd';
    }
    else if (i_mode == 0x8000) {
        return 'f';
    }
    // if none of the above
    return '?';
}

char* log_time(__u32 i_time) {
    char output[64];
    time_t rawtime = i_time;
    struct tm time = gmtime(&rawtime);
    strftime(output, 64, "%m/%d/%y %H:%M:%S", &time);

    return output;
}

void log_superblock() {
    // Parses information from the superblock.
    if (pread(img_fd, &superblock, sizeof(superblock), SUPERBLOCK_OFFSET) < 0){
        error_msg("pread failure.", 2);
    }
    block_size = EXT2_MIN_BLOCK_SIZE << superblock.s_log_block_size;
    block_count = superblock.s_blocks_count;
    inode_count = superblock.s_inodes_count;
    
    if (superblock.s_magic != EXT2_SUPER_MAGIC)
        error_msg("Failure to locate superblock.",2);
    
    printf("SUPERBLOCK,%d,%d,%d,%d,%d,%d,%d\n",
           block_count,
           inode_count,
           block_size,
           superblock.s_inode_size,
           superblock.s_blocks_per_group,
           superblock.s_inodes_per_group,
           superblock.s_first_ino);
}

long find_offset(int block_num){
    return SUPERBLOCK_OFFSET + block_size*(block_num-1);
}

void log_free_block(int block){
    char* buf = (char*)malloc(block_size);
    long offset = find_offset(block);
    int block_num = 1;
    
    if (pread(img_fd, buf, block_size, offset) < 0){
        error_msg("pread failure.", 2);
    }
    
    for(int i = 0; i < block_size; i++){
        char byte = buf[i];
        for(int j = 0; j < 8; j++){
            int mask = 1 << j;
            int read = byte & mask;
            if (!read){
                printf("BFREE,%d\n",block_num);
            }
            block_num++;
            
            if (block_num > block_count){
                break;
            }
        }
    }
    free(buf);
}

void log_allocated_inode() {
    struct ext2_inode inode;
    
}

void log_free_inode(int block){
    char* buf = (char*)malloc(block_size);
    long offset = find_offset(block);
    int inode_num = 1;
    
    if (pread(img_fd, buf, block_size, offset) < 0){
        error_msg("pread failure.", 2);
    }
    
    for(int i = 0; i < block_size; i++){
        char byte = buf[i];
        for(int j = 0; j < 8; j++){
            int mask = 1 << j;
            int read = byte & mask;
            if (!read){
                printf("IFREE,%d\n",inode_num);
            }
            else{
                log_allocated_inode();
            }
            inode_num++;
            
            if (inode_num > inode_count){
                break;
            }
        }
    }
    free(buf);
}

void log_group() {
    // Parses information from the group.
    if (pread(img_fd, &group, sizeof(group), SUPERBLOCK_OFFSET + block_size) < 0){
        error_msg("pread failure.", 2);
    }
    
    int group_num = 0;
    int block_bitmap = group.bg_block_bitmap;
    int inode_bitmap = group.bg_inode_bitmap;
    
    printf("GROUP,%d,%d,%d,%d,%d,%d,%d,%d\n",
           group_num,
           block_count,
           inode_count,
           group.bg_free_blocks_count,
           group.bg_free_inodes_count,
           block_bitmap,
           inode_bitmap,
           group.bg_inode_table);
    
    log_free_block(block_bitmap);
    log_free_inode(inode_bitmap);
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
    log_group();
    
    exit(0);
}

/*
 Exit codes:
 0 . . . analysis successful
 1 . . . bad arguments
 2 . . . corruption detected or other processing errors
 */
