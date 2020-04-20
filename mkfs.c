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
#include <getopt.h>

int block_size;
int no_of_groups;
int gdt_blocksize;

int to_decimal(int n){

	int result = 0, i = 0;

	while(n != 0){
		result += pow(2, i);
		++i;
		--n;
	}

	return result;
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

	lseek(fd, block_size + group_no * sizeof(struct ext2_group_desc), SEEK_SET);
	read(fd, &bgdesc, sizeof(struct ext2_group_desc));
	
	return 0;
}	
	
int duplicate_super_gdt(int fd, struct ext2_super_block sb, struct ext2_group_desc bgdesc){
	
	for (int block_group = 1; block_group < no_of_groups; block_group++){		
	
		if (block_group == 1 || ispowerof(block_group, 3) || ispowerof(block_group, 5) || ispowerof(block_group, 7)){		
		
			lseek(fd, block_group * sb.s_blocks_per_group * block_size, SEEK_SET);
			write(fd, sb, sizeof(ext2_super_block));
			
			for(int i = 0; i < no_of_groups; i++){
				
				read_bgdesc(fd, block_size, &bgdesc, i);
				lseek(fd, block_group * sb.s_blocks_per_group * block_size + block_size,SEEK_SET);
				lseek(fd, i * sizeof(struct ext2_group_desc), SEEK_CUR);
				write(fd, bgdesc, sizeof(ext2_group_desc));	
				
			}
								
		}				
	}	
	
	return 0;
}

int set_dir_entry(struct ext2_dir_entry_2* dirent, int inode, int record_length, int name_length, int ftype, char * str){
	
	dirent->inode = inode;
	dirent->rec_len = record_length;
	dirent->name_len = name_length;
	dirent->file_type = ftype;
	strcpy(dirent->name,str);
	return 0;
}
	
int set_datablock_bitmap(int fd, struct ext2_super_block sb, struct ext2_group_desc bgdesc){
	
	int no_full_1s;
	int extra_1s;
	int no_full_0s;
	int decimal_number;
	int full_0s = 0;
	int full_1s = 0xFFFFFFFF;
	int total_int = sb.s_inodes_per_group / 32;		 
	
	for(int block_group = 0; block_group < no_of_groups; block_group++){
			
		read_bgdesc(fd, &bgdesc, block_group);	
		lseek(fd, bgdesc.bg_block_bitmap * block_size, SEEK_SET);
				
		if(block_group == 0){
			value = 1 + gdt_blocksize + sb.s_reserved_gdt_blocks + 1 + 1 + ((sb.s_inodes_per_group * sb.s_inode_size) / block_size) + 5;
		}
		else if (block_group == 1 || ispowerof(block_group, 3) || ispowerof(block_group, 5) || ispowerof(block_group, 7)){
			value = 1 + gdt_blocksize + sb.s_reserved_gdt_blocks + 1 + 1 + ((sb.s_inodes_per_group * sb.s_inode_size) / block_size);
		}
		else {
			value = 1 + 1 + ((sb.s_inodes_per_group * sb.s_inode_size) / block_size);
		}
				
		no_full_1s = value / 32;
		for(int i = 0; i < no_full_1s; i++){
			write(fd, &full_1s, sizeof(full_1s));
		}
		extra_1s = value % 32;
		if(extra_1s){
			decimal_number = to_decimal(extra_1s);
			write(fd, &decimal_number, sizeof(decimal_number));	
		}
		no_full_0s = (total_int - ceil((float) value/32)); 
		for(int i = 0; i < no_full_0s; i++){
			write(fd, &full_0s, sizeof(full_0s));
		}
	}
	
}

int set_inode_bitmap(int fd, struct ext2_super_block sb, struct ext2_group_desc bgdesc){
	
	int value;
	int total_int = sb.s_inodes_per_group / 32;		
	
	read_bgdesc(fd, &bgdesc, 0)
	lseek(fd, bgdesc.bg_inode_bitmap * block_size, SEEK_SET);
	
	value = 0x000007FF;
	write(fd, &value, sizeof(value));
	value = 0x00000000;
	for(int i = 0; i < total_int-1; i++){
		write(fd, &value, sizeof(value));
	}
	value = 0x00000000;
	for(int i = 1; i < no_of_groups; i++){
		read_bgdesc(fd, &bgdesc, i)
		lseek(fd, bgdesc.bg_inode_bitmap * block_size, SEEK_SET);
		for(int j = 0; j < total_int; j++){
			write(fd, &value, sizeof(value));
		}
	}
	return 0;
}

int write_inodetable(int fd, int inode_no, struct ext2_super_block sb, struct ext2_group_desc bgdesc, struct ext2_inode* inode){
	
	if (inode_no == 2){	
		inode->i_mode = 16877;
		inode->i_size = block_size;		
		inode->i_links_count = 3;			
	}
	if (inode_no == 11){	
		inode->i_mode = 16832;
		inode->i_size = 4 * block_size;			
		inode->i_links_count = 2;		
	}
	
	inode->i_blocks = inode.i_size / 512;
	inode->i_atime = time(NULL);	
	inode->i_ctime = time(NULL);	
	inode->i_mtime = time(NULL);	
	inode->i_uid = 0;		
	inode->i_dtime = 0;	
	inode->i_gid = 0;	
	inode->i_flags = 0;	
		
	union {
		struct {
			inode->l_i_version = 0; 
		} linux1;
		struct {
			inode->h_i_translator = 0;
		} hurd1;
	} osd1;				
	
	if (inode_no == 2){			
		inode->i_block[0] = 1 + gdt_blocksize + sb.s_reserved_gdt_blocks + 1 + 1 + ((sb.s_inodes_per_group * sb.s_inode_size) / block_size); 
	}
	else if (inode_no == 11){
		inode->i_block[0] = 1 + gdt_blocksize + sb.s_reserved_gdt_blocks + 1 + 1 + ((sb.s_inodes_per_group * sb.s_inode_size) / block_size) + 1;    
		inode->i_block[1] = inode.i_block[0] + 1;	
		inode->i_block[2] = inode.i_block[0] + 2;	
		inode->i_block[3] = inode.i_block[0] + 3;
	}

	inode->i_generation = 0;	
	inode->i_file_acl = 0;	
	inode->i_size_high = 0;
	inode->i_faddr = 0;	
	union {
		struct {
			inode->l_i_blocks_hi = 0;
			inode->l_i_file_acl_high = 0;
			inode->l_i_uid_high = 0;	
			inode->l_i_gid_high = 0;	
			inode->l_i_checksum_lo = 0; 
			inode->l_i_reserved = 0;
		} linux2;
		struct {
			inode->h_i_frag = 0;	
			inode->h_i_fsize = 0;	
			inode->h_i_mode_high = 0;
			inode->h_i_uid_high = 0;
			inode->h_i_gid_high = 0;
			inode->h_i_author = 0;
		} hurd2;
	} osd2;

	read_bgdesc(fd, &bgdesc, 0);
	lseek(fd, bgdesc.bg_inode_table * block_size + (inode_no - 1) * 256, SEEK_SET);
	write(fd, inode, sizeof(struct ext2_inode));

	return 0;
}

int write_datablocks(int fd, struct ext2_super_block sb, struct ext2_group_desc bgdesc, struct ext2_inode inode, struct ext2_dir_entry_2* dirent){

	read_bgdesc(fd, &bgdesc, 0);
	lseek(fd, bgdesc.bg_inode_table * block_size + 2*sizeof(struct ext2_inode), SEEK_SET);
	read(fd, &inode, sizeof(struct ext2_inode));

	lseek(fd, inode.i_block[0] * block_size, SEEK_SET);
	set_dir_entry(dirent, 2, 12, 1, 2, ".\0\0\0");	
	write(fd, dirent, sizeof(struct ext2_dir_entry_2));
	lseek(fd, dirent.rec_len - sizeof(struct ext2_dir_entry_2), SEEK_CUR);
	set_dir_entry(dirent, 2, 12, 2, 2, "..\0\0");
	write(fd, dirent, sizeof(struct ext2_dir_entry_2));		
	lseek(fd, dirent.rec_len - sizeof(struct ext2_dir_entry_2), SEEK_CUR);
	set_dir_entry(dirent, 2, 20, 10, 2,"lost+found\0\0");	
	write(fd, dirent, sizeof(struct ext2_dir_entry_2));
	
	lseek(fd, bgdesc.bg_inode_table * block_size + 10*2*sizeof(struct ext2_inode), SEEK_SET);
	read(fd, &inode, sizeof(struct ext2_inode));
	
	lseek(fd, inode.i_block[0] * block_size, SEEK_SET);
	set_dir_entry(dirent, 11, 12, 1, 2, ".\0\0\0");
	write(fd, dirent, sizeof(struct ext2_dir_entry_2));	
	lseek(fd, dirent.rec_len - sizeof(struct ext2_dir_entry_2), SEEK_CUR);
	set_dir_entry(dirent, 2, 12, 2, 2, "..\0\0");
	write(fd, dirent, sizeof(struct ext2_dir_entry_2));
	
	return 0;

}

int main(int argc, char *argv[]) {
	
	int c;
	imt dummy = 0;
	struct ext2_super_block sb; 
	struct ext2_group_desc bgdesc;
	struct ext2_inode inode;
	struct ext2_dir_entry_2 dirent;
	
	
	while ((c = getopt(argc, argv, "b")) != -1){
		if(c){
			block_size = atoi(argv[optind]);
			dummy = 1;
		}
	}
	if(dummy == 0){
		block_size = 4096;
	}
	
	int fd = open(argv[1], O_RDONLY | O_WRONLY); 


	if(fd == -1) {
		perror("error:");
		exit(errno);
	}
	
	long long int p_size = lseek(fd, 0, SEEK_END);
	lseek(fd, 1024, SEEK_SET);
	
	 //the superblock
	{
	sb.s_blocks_per_group = block_size * 8;	/* # Blocks per group */
	sb.s_blocks_count = ceil(p_size / block_size);				

	no_of_groups = ceil((float)sb.s_blocks_count / sb.s_blocks_per_group);
														
	unsigned int inode_bpg = block_size / 8;	
														
	sb.s_inode_size = 256;		/* size of inode structure */
	sb.s_inodes_per_group = inode_bpg * block_size / sb.s_inode_size;	/* # Inodes per group */
	sb.s_inodes_count = sb.s_inodes_per_group * no_of_groups;		
	sb.s_r_blocks_count = (5 * sb.s_blocks_count) / 100;	   /* Reserved blocks count */
	sb.s_free_blocks_count = 			/* Free blocks count */
	sb.s_free_inodes_count = sb.s_inodes_count - 11;	/* Free inodes count */
	sb.s_first_data_block = 0;    // 	/* First Data Block */
	sb.s_log_block_size = block_size >> 11;	/* Block size */ //blocksize = 1024 << s_log_block_size
	sb.s_log_cluster_size = block_size >> 11;	/* Allocation cluster size */
	sb.s_clusters_per_group = sb.s_blocks_per_group;	/* # Fragments per group */	
	sb.s_mtime = 0;		/* Mount time */
	sb.s_wtime = time(NULL);		/* Write time */
	sb.s_mnt_count = 0;		/* Mount count */
	sb.s_max_mnt_count = -1;	/* Maximal mount count */
	sb.sb.s_magic = EXT2_SUPER_MAGIC;		/* Magic signature */
	sb.s_state = 1;		/* File system state */
	sb.s_errors = 1;		/* Behaviour when detecting errors */
	sb.s_minor_rev_level = 0;	/* minor revision level */
	sb.s_lastcheck = time(NULL);		/* time of last check */
	sb.s_checkinterval = 0;	/* max. time between checks */
	sb.s_creator_os = 0;		/* OS */
	sb.s_rev_level = 1;		/* Revision level */
	sb.s_def_resuid = 0 	/* Default uid for reserved blocks */
	sb.s_def_resgid = 0 	/* Default gid for reserved blocks */
	sb.s_first_ino = 11;		/* First non-reserved inode */
	sb.s_block_group_nr = 0;	/* block group # of this superblock */
	sb.s_feature_compat = 56;	/* compatible feature set */ //values set after reading the structure
	sb.s_feature_incompat = 2;	/* incompatible feature set */
	sb.s_feature_ro_compat = 3;  	/* readonly-compatible feature set */
	uuid_generate(sb->s_uuid);		/* 128-bit uuid for volume */
	sb -> s_volume_name[0] = '\0';	/* volume name */
	sb -> s_last_mounted[0] = '\0';	/* directory where last mounted */
	sb.s_algorithm_usage_bitmap = 0; /* For compression */
	sb.s_prealloc_blocks = 0;	/* Nr of blocks to try to preallocate*/
	sb.s_prealloc_dir_blocks = 0;	/* Nr to preallocate for dirs */
	sb.s_reserved_gdt_blocks = 127;	/* Per group table for online growth */
	sb.s_journal_uuid[16] = 0;	/* uuid of journal superblock */				
	sb.s_journal_inum = 0;		/* inode number of journal file */
	sb.s_journal_dev = 0;		/* device number of journal file */
	sb.s_last_orphan = 0;		/* start of list of inodes to delete */
	uuid_generate((char *)sb->s_hash_seed);		/* HTREE hash seed */
	sb.s_def_hash_version = 1;	/* Default hash version to use */
	sb.s_jnl_backup_type = 0; 	/* Default type of journal backup */		
	sb.s_desc_size = 0;		/* Group desc. size: INCOMPAT_64BIT */
	sb.s_default_mount_opts = 0;
	sb.s_first_meta_bg = 0;	/* First metablock group */
	sb.s_mkfs_time = time(NULL);		/* When the filesystem was created */
	
	for(int i = 0; i < 17; i++){
		sb.s_jnl_blocks[i] = 0; 	/* Backup of the journal inode */
	}
	sb.s_blocks_count_hi = 0;	/* Blocks count high 32bits */
	sb.s_r_blocks_count_hi = 0;	/* Reserved blocks count high 32 bits*/
	sb.s_free_blocks_hi = 0; 	/* Free blocks count */
	sb.s_min_extra_isize = 32;	/* All inodes have at least # bytes */ //256
	sb.s_want_extra_isize = 32; 	/* New inodes should reserve # bytes */
	sb.s_flags = 1;		/* Miscellaneous flags */
	sb.s_raid_stride = 0;		/* RAID stride */
	sb.s_mmp_update_interval = 0;  /* # seconds to wait in MMP checking */
	sb.s_mmp_block = 0;            /* Block for multi-mount protection */
	sb.s_raid_stripe_width = 0;    /* blocks on all data disks (N*stride)*/
	sb.s_log_groups_per_flex = 0;	/* FLEX_BG group size */
	sb.s_checksum_type = 0;	/* metadata checksum algorithm */
	sb.s_encryption_level = 0;	/* versioning level for encryption */
	sb.s_reserved_pad = 0;		/* Padding to next 32bits */
	sb.s_kbytes_written = 0;	/* nr of lifetime kilobytes written */
	sb.s_snapshot_inum = 0;	/* Inode number of active snapshot */
	sb.s_snapshot_id = 0;		/* sequential ID of active snapshot */
	sb.s_snapshot_r_blocks_count = 0; /* reserved blocks for active snapshot's future use */
	sb.s_snapshot_list = 0;	/* inode number of the head of the on-disk snapshot list */			
	sb.s_error_count = 0;		/* number of fs errors */
	sb.s_first_error_time = 0;	/* first time an error happened */
	sb.s_first_error_ino = 0;	/* inode involved in first error */
	sb.s_first_error_block = 0;	/* block involved of first error */
														
	for(int i = 0; i < 32; i++){
		sb.s_first_error_func[i] = 0;	/* function where the error happened */
	}
														
	sb.s_first_error_line = 0;	/* line number where error happened */
	sb.s_last_error_time = 0;	/* most recent time of an error */
	sb.s_last_error_ino = 0;	/* inode involved in last error */
	sb.s_last_error_line = 0;	/* line number where error happened */
	sb.s_last_error_block = 0;	/* block involved of last error */
	sb.s_last_error_func[32] = 0;	/* function where the error happened */
		#define EXT4_S_ERR_END ext4_offsetof(struct ext2_super_block, s_mount_opts)											
	sb.s_mount_opts[64] = 0;
	sb.s_usr_quota_inum = 0;	/* inode number of user quota file */
	sb.s_grp_quota_inum = 0;	/* inode number of group quota file */
	sb.s_overhead_blocks = 0;	/* overhead blocks/clusters in fs */
	sb.s_backup_bgs[2] = 0;	/* If sparse_super2 enabled */
	sb.s_encrypt_algos[4] = 0;	/* Encryption algorithms in use  */
	sb.s_encrypt_pw_salt[16] = 0;	/* Salt used for string2key algorithm */
	sb.s_lpf_ino = 0;		/* Location of the lost+found inode */
	sb.s_prj_quota_inum = 0;	/* inode for tracking project quota */
	sb.s_checksum_seed = 0;	/* crc32c(orig_uuid) if csum_seed set */
	sb.s_reserved[98] = 0;		/* Padding to the end of the block */
	sb.s_checksum = 0;		/* crc32c(superblock) */
	
	write(fd, sb, sizeof(ext2_super_block));
	
	}
	// the block group descriptor table
	{
	
	int reqsize_gdt = no_of_groups * 32;
	gdt_blocksize = ceil((float)reqsize_gdt / block_size);

	
	for(int block_group = 0; i < no_of_groups; block_group++){
		
		long long int initial = block_group * s_blocks_per_group;
		
		if (block_group == 0 || block_group == 1 || ispowerof(block_group, 3) || ispowerof(block_group, 5) || ispowerof(block_group, 7)) {
			bgdesc.bg_block_bitmap = initial + 1 + gdt_blocksize + sb.s_reserved_gdt_blocks;
		} else{
			bg.desc.bg_block_bitmap =  initial;
		}
		
		
		bgdesc.bg_inode_bitmap = bgdesc.bg_block_bitmap + 1;
		bgdesc.bg_inode_table = bgdesc.bg_inode_bitmap + 1;
		
		if(block_group == 0) {
			bgdesc.bg_free_blocks_count = sb.s_blocks_per_group - gdt_blocksize - sb.s_reserved_gdt_blocks - ((sb.s_inodes_per_group * sb.s_inode_size) / block_size) - 8;
		}
		else if(block_group == 1 || ispowerof(block_group, 3) || ispowerof(block_group, 5) || ispowerof(block_group, 7)) {
			bgdesc.bg_free_blocks_count = sb.s_blocks_per_group - gdt_blocksize - sb.s_reserved_gdt_blocks - ((sb.s_inodes_per_group * sb.s_inode_size) / block_size) - 3;
		}
		else{
			bgdesc.bg_free_blocks_count = sb.s_blocks_per_group - ((sb.s_inodes_per_group * sb.s_inode_size) / block_size) - 2; 
		}
		
		if (block_group == 0){		
			bgdesc.bg_used_dirs_count = 2;
			bgdesc.bg_free_inodes_count = sb.s_inodes_per_group - 11;
		}			
		else { 
			bgdesc.bg_used_dirs_count = 0;
			bgdesc.bg_free_inodes_count = sb.s_inodes_per_group;			
		}
		
		bgdesc.bg_flags = 0;
		bgdesc.bg_exclude_bitmap_lo = 0;
		bgdesc.bg_block_bitmap_csum_lo = 0;
		bgdesc.bg_inode_bitmap_csum_lo = 0;
		bgdesc.bg_itable_unused = 0;
		bgdesc.bg_checksum = 0;
		
		write(fd, bgdesc, sizeof(ext2_group_desc);
	}
			
	}
	duplicate_super_gdt(fd, block_size, sb, bgdesc);
	set_datablock_bitmap(fd, sb, bgdesc);
	set_inode_bitmap(fd, sb, bgdesc);
	write_inodetable(fd, 2, sb, bgdesc, &inode);
	write_inodetable(fd, 11, sb, bgdesc, &inode);	
	write_datablocks(fd, sb, bgdesc, inode, &dirent);

}		