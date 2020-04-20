#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <linux/fs.h>
#include <ext2fs/ext2_fs.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>
#include <uuid.h>
#include <math.h>

int main(int argc, char *argv[]) {
	int fd = open(argv[1], O_RDONLY); // argv[1] = /dev/sdb1 
	
	int count, bs = 4096, i = 0;
	struct ext2_super_block sb; 
	struct ext2_group_desc bgdesc;
	struct ext2_inode inode;
	struct ext2_dir_entry_2 dirent;
	unsigned int block_size;


	if(fd == -1) {
		perror("error:");
		exit(errno);
	}
	
	int curr_pos = lseek(fd, 0, SEEK_CUR);
	int p_size;
	p_size = lseek(fd, 0, SEEK_END);
	lseek(fd, curr_pos, SEEK_SET);	

	count = read(fd, &sb, sizeof(struct ext2_super_block));

/*checking consistency of superblock
	 need to check: File system size    Number of inodes    Free-block count    Free-inode count
*/	
	unsigned int inodes_count = sb.s_inodes_per_group * no_of_groups;
	if (sb.s_inodes_count == inodes_count)
		//ok
	if (sb.s_blocks_per_group == bs * 8);
	if (sb.s_blocks_count == p.size / bs);				

//	unsigned int no_of_groups = ceil((float)sb.s_blocks_count / sb.s_blocks_per_group);
														
	unsigned int inode_bpg = bs / 8;	
												
	if (sb.s_inodes_per_group == inode_bpg * bs / sb.s_inode_size);

	if (sb.s_inodes_count == sb.s_inodes_per_group * no_of_groups);
		
	if (sb.s_r_blocks_count == (5 * sb.s_blocks_count) / 100);

	if (sb.s_free_blocks_count = );

	if (sb.s_free_inodes_count == sb.s_inodes_count - 11);
	if (sb.s_log_block_size == bs >> 11);	
	if (sb.s_log_cluster_size == bs >> 11);
	if (sb.s_clusters_per_group == sb.s_blocks_per_group);		



}
