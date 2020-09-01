# Project 3

## Structure of an Ext2 file system
* The first 1024 bytes (1KB) is the boot block
* The rest of the disk is partitioned into multiple block groups
* Block groups: contiguous bytes on adjacent location of the disk, a single file can only on one block group -> speed up the performance, while increase fragmentation

### Boot block

* Contains the data/code used for booting up the operating system
* BIOS needs to load OS from disk to memory -> Boot block contains the location of the operating system on the disk

### Super block

* Located after the boot block:
* Starting at 1024 bytes offset from the beginning of the disk
* Size: 1024 Bytes
* Describe the general information of the fil system
	* e.g. what is the fyle system on the disk, ext2 or FAT
	* how many blocks are there in the file system
	* what is the block size of the file system

### Group Descriptors

* Located in the next blcok after super block
* Three conditions:
	* block size = 1024, at block 2
	* block size > 1024, at block 1
	* block size < 1024, not possible, minimum size is 1024

### Block Bitmap

* A sequence of 0 & 1 indicating whether a block has been used or not
* 1 indicates the blcok is used (occupied by files or used by file system), 0 indicates the block is free 9can be used by newly created/enlarged files)

### Inode Bitmap:

* Indicate whether an ext2_inode in inode table is used or not.
* Inode 0 is reversed, inode bitmap starts at inode 1.
* Exactly the same as block bitmap.