//NAME: Son Dang
//EMAIL: sonhdang@ucla.edu
//ID: 105215636

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include "ext2_fs.h"

unsigned int BLOCK_SIZE;
unsigned int BLOCKS_COUNT;
unsigned int BLOCKS_PER_GROUP;
unsigned int INODE_SIZE;
unsigned int INODES_PER_GROUP;
unsigned int INODES_COUNT;
unsigned int GROUPS_COUNT;
unsigned int BLOCK_BITMAP_ADDR; // Starting address of Block bitmap
unsigned int INODE_BITMAP_ADDR; // Starting address of Inode bitmap
unsigned int INODE_TABLE_ADDR;  // Starting address of Inode table
char datetime_str[24];

// Exit with failure
void fail(char* message)    
{
    fprintf(stderr, "Error: %s\n", message);
    exit(EXIT_FAILURE);
}

// Printing date and time in GMT for Inodes
char* print_datetime(int epoch)
{
    struct tm *time_object;
    time_t date_time = epoch;
    time_object = gmtime(&date_time);
    sprintf(datetime_str, "%02d/%02d/%02d %02d:%02d:%02d, GMT",
        time_object->tm_mon, time_object->tm_mday, time_object->tm_year % 100,
        time_object->tm_hour, time_object->tm_min, time_object->tm_sec);
    return datetime_str;
}

// Output superblock summary
void read_superblock(int file)  // Print
{
    struct ext2_super_block super;
    pread(file, &super, sizeof(super), 1024);
    BLOCK_SIZE = EXT2_MIN_BLOCK_SIZE << super.s_log_block_size; // Formula for getting block size
    BLOCKS_COUNT = super.s_blocks_count;
    GROUPS_COUNT = super.s_blocks_count / super.s_blocks_per_group + 1;
    BLOCKS_PER_GROUP = super.s_blocks_per_group;
    INODES_PER_GROUP = super.s_inodes_per_group;
    INODES_COUNT = super.s_inodes_count;
    INODE_SIZE = super.s_inode_size;
    printf("SUPERBLOCK,%d,%d,%d,%d,%d,%d,%d\n", 
        super.s_blocks_count, super.s_inodes_count,
        BLOCK_SIZE, super.s_inode_size, super.s_blocks_per_group,
        super.s_inodes_per_group, super.s_first_ino);
    
}

// Output group summary
void read_group(int file)
{
    struct ext2_group_desc group;
    int group_desc_addr;
    unsigned int group_num = 0;
    
    if (BLOCK_SIZE == 1024)     // If block size is 1024, Group Descriptors at block 2
        group_desc_addr = BLOCK_SIZE * 2;
    else                        // If block size > 1024, Group Descriptors at block 1
    {
        group_desc_addr = BLOCK_SIZE;
    }

    while(pread(file, &group, sizeof(group), group_desc_addr) != 0 && group_num < GROUPS_COUNT)
    {
        BLOCK_BITMAP_ADDR = group.bg_block_bitmap * BLOCK_SIZE;
        INODE_BITMAP_ADDR = group.bg_inode_bitmap * BLOCK_SIZE;
        INODE_TABLE_ADDR = group.bg_inode_table * BLOCK_SIZE;
        printf("GROUP,%d,%d,%d,%d,%d,%d,%d,%d\n",
            group_num, BLOCKS_COUNT, INODES_PER_GROUP,
            group.bg_free_blocks_count, group.bg_free_inodes_count,
            group.bg_block_bitmap, group.bg_inode_bitmap,
            group.bg_inode_table);
        group_num++;
    }
}

// Output Free blocks summary
void read_free_blocks(int file)
{

    int byte_offset = 0;
    unsigned int block = 1;     // First block of block bitmap is indexed 1
    char c;
    while (block <= BLOCKS_COUNT)
    {
        pread(file, &c, 1, BLOCK_BITMAP_ADDR + byte_offset);
        for(int i = 0; i < 8; i++)  // Reading every bit (8 total) of a byte
        {
            int lsb = c & 1;
            c = c >> 1;
            if (lsb == 0)
                printf("BFREE,%d\n", block);
            block++;
        }
        byte_offset++;
    }
}

// Output directory info
void read_directory(int parent_inode, int block, int file)
{
    unsigned int offset = 0;
    struct ext2_dir_entry dir;
    char name[EXT2_NAME_LEN + 1];   // 1 extra bit for the string terminating value '\0'
    while (offset < BLOCK_SIZE && 
        pread(file, &dir, sizeof(dir), block * BLOCK_SIZE + offset) > 0)
    {
        memcpy(name, dir.name, dir.name_len);
        name[dir.name_len] = '\0';
        printf("DIRENT,%d,%d,%d,%d,%d,'%s'\n",
            parent_inode, offset, dir.inode, dir.rec_len, dir.name_len, name);
        offset += dir.rec_len;
    }
}

// Output summary of indirect blocks (recursive function)
void read_indirect(int file, int inode, int level, int block)
{
    if(level == 1)      // Single Indirect Block
    {
        int block_number = 1;   // Index of Inderect block in the same level
        unsigned int offset = 0;
        __u32 logical_block;
        while (offset < BLOCK_SIZE)
        {
            pread(file, &logical_block, sizeof(__u32), BLOCK_SIZE * block + offset);
            if (logical_block == 0)
            {
                break;
            }
            printf("INDIRECT,%d,%d,%d,%d,%d\n", inode, level, logical_block, block_number, block);
            block_number++;
            offset += sizeof(__u32);
        }
        return;
    }
    else if(level == 2)     // Double Indirect Block
    {
        int block_number = 1;
        unsigned int offset = 0;
        __u32 logical_block;
        while (offset < BLOCK_SIZE)
        {
            pread(file, &logical_block, sizeof(__u32), BLOCK_SIZE * block + offset);
            if (logical_block == 0)
                break;
            printf("INDIRECT,%d,%d,%d,%d,%d\n", inode, level, logical_block, block_number, block);
            read_indirect(file, inode, 1, logical_block);
            block_number++;
            offset += sizeof(__u32);
        }
    }
    else if(level == 3)     // Triple Indirect Block
    {
        int block_number = 1;
        unsigned int offset = 0;
        __u32 logical_block;
        while (offset < BLOCK_SIZE)
        {
            pread(file, &logical_block, sizeof(__u32), BLOCK_SIZE * block + offset);
            if (logical_block == 0)
                break;
            printf("INDIRECT,%d,%d,%d,%d,%d\n", inode, level, logical_block, block_number, block);
            read_indirect(file, inode, 2, logical_block);
            block_number++;
            offset += sizeof(__u32);
        }
    }
}

void read_inodes(int file)
{
    int byte_num = 0;
    unsigned int bit_num = 0;
    char c;
    while (bit_num < INODES_COUNT)
    {
        pread(file, &c, 1, INODE_BITMAP_ADDR + byte_num);
        for(int i = 0; i < 8; i++)
        {                               // In ext2_fs.h, root inode (EXT2_ROOT_INODE) starts
            int inode = bit_num + 1;    // at 2 since inode 1 is bad inode (EXT2_BAD_INODE)
            int lsb = c & 1;
            c = c >> 1;
            if (lsb == 0)
                printf("IFREE,%d\n", inode);
            else
            {
                struct ext2_inode i_node;
                char type;
                const char* inode_change_time;
                const char* inode_modified_time;
                const char* inode_accessed_time;

                pread(file, &i_node, sizeof(i_node),
                    INODE_TABLE_ADDR + bit_num * INODE_SIZE);

                if (i_node.i_links_count != 0)          // categorizing file type
                {
                    if (S_ISDIR(i_node.i_mode))         // File is a directory
                        type = 'd';
                    else if (S_ISREG(i_node.i_mode))    // File is a regular file
                        type = 'f';
                    else if (S_ISLNK(i_node.i_mode))    // File is a symbolic link
                        type = 's';
                    else                                // File is indeterminable
                        type = '?';

                    inode_change_time = print_datetime(i_node.i_ctime);
                    inode_modified_time = print_datetime(i_node.i_mtime);
                    inode_accessed_time = print_datetime(i_node.i_atime);
                    
                    printf("INODE,%d,%c,%o,%d,%d,%d,%s,%s,%s,%d,%d",
                        inode, type, i_node.i_mode & 0xFFF, i_node.i_uid,
                        i_node.i_gid, i_node.i_links_count, inode_change_time, 
                        inode_modified_time, inode_accessed_time,
                        i_node.i_size, i_node.i_blocks);

                    if (type == 's' && i_node.i_size <= 60) // For symbolic links with file
                        printf("\n");                       // size less than 60 bytes
                    else
                    {
                        for (int i = 0; i < EXT2_N_BLOCKS; i++)
                        {
                            printf(",%d", i_node.i_block[i]);
                        }
                        printf("\n");
                    }

                    if (type == 'd')    // Output more information if file is a directory
                    {
                        // since i_blocks size is 512 bytes while the actual
                        // block size EXT2_MIN_BLOCK_SIZE is 1024
                        int actual_blocks_num = i_node.i_blocks / 2;
                        if (actual_blocks_num <= 12)
                        {
                            for (int i = 0; i < actual_blocks_num; i++)
                            {
                                read_directory(inode, i_node.i_block[i], file);
                            }
                        }
                        else
                        {
                            for (int i = 0; i < EXT2_NDIR_BLOCKS; i++)
                            {
                                read_directory(inode, i_node.i_block[i], file);
                            }
                        }
                    }
                    
                    // Output a summary of all indirect blocks
                    if (i_node.i_block[EXT2_IND_BLOCK] != 0)
                        read_indirect(file, inode, 1, i_node.i_block[EXT2_IND_BLOCK]);
                    if (i_node.i_block[EXT2_DIND_BLOCK] != 0)
                        read_indirect(file, inode, 2, i_node.i_block[EXT2_DIND_BLOCK]);
                    if (i_node.i_block[EXT2_TIND_BLOCK] != 0)
                        read_indirect(file, inode, 3, i_node.i_block[EXT2_TIND_BLOCK]);
                }

            }
            
            bit_num++;
        }
        byte_num++;
    }
}



int main(int argc, char *argv[])
{
    int fd;
    

    if (argc != 2)
        fail("invalid input");          // Invalid input for the program
    else
    {
        fd = open(argv[1], O_RDONLY);
        if (fd < 0)
            fail("fail to open file");  // Cannot find file
    }

    read_superblock(fd);
    read_group(fd);
    read_free_blocks(fd);
    read_inodes(fd);

    
    close(fd);
    return 0;
}