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
#include "ext2_fs.h"

int BLOCK_SIZE;
int BLOCKS_COUNT;
int BLOCKS_PER_GROUP;
int INODE_SIZE;
int INODES_PER_GROUP;
int INODES_COUNT;
int GROUPS_COUNT;
int BLOCK_BITMAP_ADDR;
int INODE_BITMAP_ADDR;
int INODE_TABLE_ADDR;
char datetime_str[24];

void fail(char* message)
{
    fprintf(stderr, "Error: %s\n", message);
    exit(EXIT_FAILURE);
}

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

void read_superblock(int file)
{
    struct ext2_super_block super;
    pread(file, &super, sizeof(super), 1024);
    BLOCK_SIZE = EXT2_MIN_BLOCK_SIZE << super.s_log_block_size;
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

void read_group(int file)
{
    struct ext2_group_desc group;
    int group_desc_addr;
    int group_num = 0;
    if (BLOCK_SIZE == 1024)
        group_desc_addr = BLOCK_SIZE * 2;
    else
    {
        group_desc_addr = BLOCK_SIZE;
    }

    lseek(file, group_desc_addr, SEEK_SET);

    while(read(file, &group, sizeof(group)) != 0 && group_num < GROUPS_COUNT)
    {
        BLOCK_BITMAP_ADDR = group.bg_block_bitmap * BLOCK_SIZE;
        INODE_BITMAP_ADDR = group.bg_inode_bitmap * BLOCK_SIZE;
        INODE_TABLE_ADDR = group.bg_inode_table * BLOCK_SIZE;
        printf("GROUP,%d,%d,%d,%d,%d,%d,%d,%d\n",
            group_num, BLOCKS_PER_GROUP, INODES_PER_GROUP,
            group.bg_free_blocks_count, group.bg_free_inodes_count,
            group.bg_block_bitmap, group.bg_inode_bitmap,
            group.bg_inode_table);
        group_num++;
    }
}

void read_free_blocks(int file)
{

    int byte_num = 0;
    int bit_num = 1;
    char c;
    while (bit_num < BLOCKS_COUNT)
    {
        pread(file, &c, 1, BLOCK_BITMAP_ADDR + byte_num);
        for(int i = 0; i < 8; i++)
        {
            int lsb = c & 1;
            c = c >> 1;
            if (lsb == 0)
                printf("BFREE,%d\n", bit_num);
            bit_num++;
        }
        byte_num++;
    }
}

void read_inodes(int file)
{
    int byte_num = 0;
    int bit_num = 0;
    char c;
    while (bit_num < INODES_COUNT)
    {
        pread(file, &c, 1, INODE_BITMAP_ADDR + byte_num);
        for(int i = 0; i < 8; i++)
        {
            int lsb = c & 1;
            c = c >> 1;
            if (lsb == 0)
                printf("IFREE,%d\n", bit_num + 1);
            else
            {
                struct ext2_inode i_node;
                char type;
                const char* inode_change_time;
                const char* inode_modified_time;
                const char* inode_accessed_time;

                pread(file, &i_node, sizeof(i_node),
                    INODE_TABLE_ADDR + bit_num * INODE_SIZE);

                if (i_node.i_links_count != 0)
                {
                    if (S_ISDIR(i_node.i_mode))
                        type = 'd';
                    else if (S_ISREG(i_node.i_mode))
                        type = 'f';
                    else if (S_ISLNK(i_node.i_mode))
                        type = 's';
                    else
                        type = '?';

                    inode_change_time = print_datetime(i_node.i_ctime);
                    inode_modified_time = print_datetime(i_node.i_mtime);
                    inode_accessed_time = print_datetime(i_node.i_atime);
                    
                    printf("INODE,%d,%c,%o,%d,%d,%d,%s,%s,%s,%d,%d",
                        bit_num + 1, type, i_node.i_mode & 0xFFF, i_node.i_uid,
                        i_node.i_gid, i_node.i_links_count, inode_change_time, 
                        inode_modified_time, inode_accessed_time,
                        i_node.i_size, i_node.i_blocks);
                    if (type == 's' && i_node.i_size <= 60)
                        printf("\n");
                    else
                    {
                        for (int i = 0; i < EXT2_N_BLOCKS; i++)
                        {
                            printf(",%d", i_node.i_block[i]);
                        }
                        printf("\n");
                    }
                }

            }
            
            bit_num++;
        }
        byte_num++;
    }
}

void read_directory(int file, int inode)
{
    struct ext2_inode i_node;

}

int main(int argc, char *argv[])
{
    int fd;
    

    if (argc != 2)
        fail("invalid input");
    else
    {
        fd = open(argv[1], O_RDONLY);
        if (fd < 0)
            fail("fail to open file");
    }

    read_superblock(fd);
    read_group(fd);
    read_free_blocks(fd);
    read_inodes(fd);

    
    close(fd);
    return 0;
}