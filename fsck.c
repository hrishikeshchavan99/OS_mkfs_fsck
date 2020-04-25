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



int block_size;
int no_of_groups;
int gdt_blocksize;
long long int freebl_usingbgdesc;

typedef struct node {
	int inode_no;
	int no_of_children, level;
	struct node  **child;
}node;

typedef node *tree;

int addnode(node *root, int inode_no, int no_of_children, int level){
	node *current_node;
	current_node = root;	
	int i;	
	for (i = 0; i < current_node->no_of_children; i++){
		current_node->child[i] = (node *)malloc(sizeof(node));
		current_node->child[i]->inode_no = 0;   //to be changed later
		current_node->child[i]->no_of_children = 3;
		current_node->child[i]->level = 1;	
		if (current_node->no_of_children != 0){
			addnode(current_node->child[i]->inode_no, current_node->child[i]->no_of_children, current_node->child[i]->level);
		}
	}
}	

int ispowerof(int n, int k){
	
	while (n != 1){
		if(n % k != 0)
        		return 0;
		n = n / k;
	}
    
	return 1;
}

int read_bgdesc(int fd, struct ext2_group_desc* bgdesc, int group_no){	
	
	if(block_size == 1024)
		lseek(fd, 2*block_size + group_no * sizeof(struct ext2_group_desc), SEEK_SET);
	else
		lseek(fd, block_size + group_no * sizeof(struct ext2_group_desc), SEEK_SET);

	read(fd, &bgdesc, sizeof(struct ext2_group_desc));
	return 0;
}


int main(int argc, char *argv[]) {
	int fd = open(argv[1], O_RDONLY); // argv[1] = /dev/sdb1 
	
	int count, bs = 4096, i = 0;
	struct ext2_super_block sb; 
	struct ext2_group_desc bgdesc;
	struct ext2_inode inode;
	struct ext2_dir_entry_2 dirent;
	unsigned int block_size;

	int flag = 0, flag2 = 0;
	no_of_groups = ceil((float)sb.s_blocks_count / sb.s_blocks_per_group);

	if(fd == -1) {
		perror("error:");
		exit(errno);
	}
	
	int curr_pos = lseek(fd, 0, SEEK_CUR);
	int p_size;
	p_size = lseek(fd, 0, SEEK_END);
	lseek(fd, curr_pos, SEEK_SET);	

	count = read(fd, &sb, sizeof(struct ext2_super_block));


	

	//checking consistency of block groups
	if(block_size == 1024)
		lseek(fd, 2048, SEEK_SET);
	else
		lseek(fd, block_size,SEEK_SET);
		
	// the block group descriptor table
	int reqsize_gdt = no_of_groups * 32;
	gdt_blocksize = ceil((float)reqsize_gdt / block_size);

	freebl_usingbgdesc = 0;
	long  long int tb;
	
	for(int block_group = 0; block_group < no_of_groups; block_group++){
		
		read_bgdesc(fd, &bgdesc, block_group);
		
		long long int initial = block_group * sb.s_blocks_per_group;
		
		if (block_group == 0 || block_group == 1 || ispowerof(block_group, 3) || ispowerof(block_group, 5) || ispowerof(block_group, 7)) {	
			if(block_size == 1024)
				if (bgdesc.bg_block_bitmap != initial + 1 + 1 + gdt_blocksize + sb.s_reserved_gdt_blocks){
					flag2++;
					printf();
				}
			else
				if (bgdesc.bg_block_bitmap != initial + 1 + gdt_blocksize + sb.s_reserved_gdt_blocks){
					flag2++;
					printf();
				}
		} else{
			if(block_size == 1024)
				if (bgdesc.bg_block_bitmap !=  initial + 1){
					flag2++;
					printf();
				}
			else	
				if (bgdesc.bg_block_bitmap !=  initial){
					flag2++;
					printf();
				}
		}
			
		if (bgdesc.bg_inode_bitmap != bgdesc.bg_block_bitmap + 1){
					flag2++;
					printf();
		}
		if (bgdesc.bg_inode_table != bgdesc.bg_inode_bitmap + 1){
					flag2++;
					printf();
		}
		
		if(block_group == 0) {
			if(block_group == no_of_groups-1){
				tb = sb.s_blocks_count % sb.s_blocks_per_group;
				if(tb == 0)
					tb = sb.s_blocks_per_group;
			}
			else{
				tb = sb.s_blocks_per_group;
			}
			if (bgdesc.bg_free_blocks_count != tb - gdt_blocksize - sb.s_reserved_gdt_blocks - ((sb.s_inodes_per_group * sb.s_inode_size) / block_size) - 9){
					flag2++;
					printf();
			}
			
			if(block_size == 1024)
				bgdesc.bg_free_blocks_count -= 9;
			if(block_size == 2048)
				bgdesc.bg_free_blocks_count -= 4;
		}
		else if(block_group == 1 || ispowerof(block_group, 3) || ispowerof(block_group, 5) || ispowerof(block_group, 7)) {
			if(block_group == no_of_groups-1){
				tb = sb.s_blocks_count % sb.s_blocks_per_group;
				if(tb == 0)
					tb = sb.s_blocks_per_group;
			}
			else{
				tb = sb.s_blocks_per_group;
			}
			if (bgdesc.bg_free_blocks_count != tb - gdt_blocksize - sb.s_reserved_gdt_blocks - ((sb.s_inodes_per_group * sb.s_inode_size) / block_size) - 3){
					flag2++;
					printf();
			}
		
		}
		else{
			if(block_group == no_of_groups-1){
				tb = sb.s_blocks_count % sb.s_blocks_per_group;
				if(tb == 0)
					tb = sb.s_blocks_per_group;
			}
			else{
				tb = sb.s_blocks_per_group;
			}
			if (bgdesc.bg_free_blocks_count != tb - ((sb.s_inodes_per_group * sb.s_inode_size) / block_size) - 2){
					flag2++;
					printf();
			} 
			
		}
		
		freebl_usingbgdesc += bgdesc.bg_free_blocks_count;
		
		if (block_group == 0){		
			if (bgdesc.bg_free_inodes_count != sb.s_inodes_per_group - 11){
					flag2++;
					printf();
			}
		}			
		else { 
			if (bgdesc.bg_free_inodes_count != sb.s_inodes_per_group){
					flag2++;
					printf();
			}			
		}
		
		
		
	}


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
												
	if (sb.s_inodes_per_group != inode_bpg * bs / sb.s_inode_size){
		flag++;
		printf();
	}
	if (sb.s_inodes_count == sb.s_inodes_per_group * no_of_groups){
		flag++;
		printf();
	}
	
	if (sb.s_r_blocks_count == (5 * sb.s_blocks_count) / 100){
		flag++;
		printf();
	}

	if (sb.s_free_blocks_count != freebl_usingbgdesc){
		flag++;
		printf();
	}

	if (sb.s_free_inodes_count == sb.s_inodes_count - 11){
		flag++;
		printf();
	}

	if (sb.s_log_block_size == bs >> 11){
		flag++;
		printf();
	}
	if (sb.s_log_cluster_size == bs >> 11){
		flag++;
		printf();
	}

	if (sb.s_clusters_per_group == sb.s_blocks_per_group){
		flag++;
		printf();
	}
	
	if(sb.s_log_block_size)
		if (sb.s_first_data_block != 0){
		flag++;
		printf();
	}
	else
		if (sb.s_first_data_block != 1){
		flag++;
		printf();
	}

	if (flag != 0){
		printf("Superblock is not clean\n");
	}

/*
//inode number as input


	int children, lvl = 0, i = 0;
	node *root, *current_node;

	current_node = root;

	//creating root node
	root = (node *)malloc(sizeof(node));
	root -> inode_no = 0;   //to be changed later
	root -> no_of_children = 3;
	root -> level = 0;

	
	addnode(root, inode_no, no_of_children, level);


read_bgdesc(fd, &bgdesc, group_no);
lseek(fd, bgdesc.bg_inode_table * block_size + (inode_no - 1) * 256, SEEK_SET);
read(fd, &inode, sizeof(struct ext2_inode));
int inode_blocks = inode.i_blocks;
for(int i = 0; i < inode_blocks; i++){
	lseek(fd, inode.i_block[i] * block_size, SEEK_SET);
	while(fd != (fd + block_size)){
		read(fd, &dirent, sizeof(struct ext2_dir_entry_2));
		dirent.inode
	}
}
if(inode.i_links_count == 2)
//end
/*
}


