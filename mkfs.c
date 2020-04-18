#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <linux/fs.h>
#include <ext2fs/ext2_fs.h>
//#include "ext2_fs.h"
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>

/*
changes

1. number of elements in the i_block array is variable, to calculate, size/blocksize.
2. i_blocks = i_size/512
3. i_mode value?? 
4. should we make separate function? set_inode?
5. i_blocks[] must be inside and is specific to that particular inode, if chya aat ghyayla lagel. tu kahi goshti kay kelet tyat mala kalat nahiye mhnun mi 
change karat nahiye ata he
6. struct ext2_dir_entry_2 is used instead of the previous one
7. exact position of the data blocks
8. use getopt
option -b, using that set it to the bs
*/
int ispowerof(int n, int k){
	while (n != 1){
		if(n % k != 0)
        		return 0;
		n = n / k;
	}
    	return 1;
}

int main(int argc, char *argv[]) {
	int fd = open(argv[1], O_RDONLY); // argv[1] = /dev/sdb1 
	
	int count, bs = 4096, i = 0;
	struct ext2_super_block sb; 
	struct ext2_group_desc bgdesc;
	struct ext2_inode inode;
	struct ext2_dir_entry_2 dirent;
	unsigned int block_size;
//	time_t T= time(NULL);

	if(fd == -1) {
		perror("error:");
		exit(errno);
	}
	
	int curr_pos = lseek(fd, 0, SEEK_CUR);
	int p_size;
	p_size = lseek(fd, 0, SEEK_END);
	lseek(fd, curr_pos, SEEK_SET);	

	lseek(fd, 1024, SEEK_CUR);

//block size and partition size is fixed only;
	
	for (i = 0; i < no_of_groups; i++){
		if (i == 0 || i == 1 || ispowerof(i, 3) || ispowerof(i, 3) || ispowerof(i, 3)){		
	
			sb.s_inodes_count = sb.s_inodes_per_group * no_of_bgroups;		
			sb.s_blocks_count = p.size / bs;				



			sb.s_r_blocks_count = (5 * sb.s_blocks_count) / 100;	   /* Reserved blocks count */
			sb.s_free_blocks_count;			/* Free blocks count */
			sb.s_free_inodes_count = sb.s_inodes_count - 11;	/* Free inodes count */
			sb.s_first_data_block = 0;    // 	/* First Data Block */
			sb.s_log_block_size = bs;	/* Block size */
			sb.s_log_cluster_size = bs;	/* Allocation cluster size */
			sb.s_blocks_per_group = bs * 8;	/* # Blocks per group */

			no_of_groups = sb.s_blocks_count / sb.s_blocks_per_group;	

//-11 right shift	
			sb.s_clusters_per_group = sb.s_blocks_per_group;	/* # Fragments per group */
		
			inode_bpg = bs / 8;
	
		--	sb.s_inode_size = 256;		/* size of inode structure */
			sb.s_inodes_per_group = inode_bpg * bs / sb.s_inode_size;	/* # Inodes per group */
			sb.s_mtime = 0;		/* Mount time */
			sb.s_wtime = time(NULL);		/* Write time */
	
//time null

			sb.s_mnt_count = 0;		/* Mount count */
			sb.s_max_mnt_count = -1;	/* Maximal mount count */
	
			sb.sb.s_magic = EXT2_SUPER_MAGIC;		/* Magic signature */
	
//ext	
			sb.s_state = 1;		/* File system state */
			sb.s_errors = 1;		/* Behaviour when detecting errors */
			sb.s_minor_rev_level = 0;	/* minor revision level */
			sb.s_lastcheck;		/* time of last check */
	

			sb.s_checkinterval = 0;	/* max. time between checks */
			sb.s_creator_os = 0;		/* OS */
			sb.s_rev_level = 1;		/* Revision level */
			sb.s_def_resuid = 0 (user root);		/* Default uid for reserved blocks */
			sb.s_def_resgid = 0 (group root);		/* Default gid for reserved blocks */



--			sb.s_first_ino = 11;		/* First non-reserved inode */

			sb.s_block_group_nr = 0 or 1;	/* block group # of this superblock */
			sb.s_feature_compat = 0;	/* compatible feature set */
			sb.s_feature_incompat = 0;	/* incompatible feature set */
			sb.s_feature_ro_compat = 0;  	/* readonly-compatible feature set */
			sb.s_uuid[16];		/* 128-bit uuid for volume */
			char	s_volume_name[EXT2_LABEL_LEN];	/* volume name */
			char	s_last_mounted[64];	/* directory where last mounted */
			s_algorithm_usage_bitmap = 0; /* For compression */
	


			sb.s_prealloc_blocks = 0;	/* Nr of blocks to try to preallocate*/
			sb.s_prealloc_dir_blocks = 0;	/* Nr to preallocate for dirs */
			sb.s_reserved_gdt_blocks = 127;	/* Per group table for online growth */

			sb.s_journal_uuid[16] = 0;	/* uuid of journal superblock */				
			sb.s_journal_inum = 0;		/* inode number of journal file */
			sb.s_journal_dev = 0;		/* device number of journal file */
			sb.s_last_orphan = 0;		/* start of list of inodes to delete */
			sb.s_hash_seed[4] = 0;		/* HTREE hash seed */
			sb.s_def_hash_version = 0;	/* Default hash version to use */
			sb.s_jnl_backup_type = 0; 	/* Default type of journal backup */		
			sb.s_desc_size = 0;		/* Group desc. size: INCOMPAT_64BIT */
			sb.s_default_mount_opts = 0;
			sb.s_first_meta_bg = 0;	/* First metablock group */
			sb.s_mkfs_time = 0;		/* When the filesystem was created */
			sb.s_jnl_blocks[17] = 0; 	/* Backup of the journal inode */
			sb.s_blocks_count_hi = 0;	/* Blocks count high 32bits */
			sb.s_r_blocks_count_hi = 0;	/* Reserved blocks count high 32 bits*/
			sb.s_free_blocks_hi = 0; 	/* Free blocks count */
			sb.s_min_extra_isize = 0;	/* All inodes have at least # bytes */
			sb.s_want_extra_isize = 0; 	/* New inodes should reserve # bytes */
			sb.s_flags = 0;		/* Miscellaneous flags */
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
			sb.s_snapshot_r_blocks_count = 0; /* reserved blocks for active
					      snapshot's future use */
			sb.s_snapshot_list = 0;	/* inode number of the head of the on-disk snapshot list */
	
			sb.s_error_count = 0;		/* number of fs errors */
			sb.s_first_error_time = 0;	/* first time an error happened */
			sb.s_first_error_ino = 0;	/* inode involved in first error */
			sb.s_first_error_block = 0;	/* block involved of first error */
			sb.s_first_error_func[32] = 0;	/* function where the error happened */
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

`			write(fd, sb, sizeof(ext2_super_block));
			
// for group 0
			int skip;
	
			skip = 32768 * i;
			bgdesc.bg_block_bitmap = skip + 1 + 1 + sb.s_reserved_gdt_blocks;
			bgdesc.bg_inode_bitmap = bgdesc.bg_block_bitmap + 1;
			bgdesc.bg_inode_table = bgdesc.bg_block_bitmap + 2;
							
			if (i == 0){
				bgdesc.bg_free_blocks_count = sb.s_blocks_per_group;		
				bgdesc.bg_used_dirs_count = 2;
				bgdesc.bg_free_inodes_count = sb.s_inodes_count - 11;
			}			
			else {
				bgdesc.bg_used_dirs_count = 0;
				bgdesc.bg_free_inodes_count = sb.s_inodes_count;			
			}
			
			bgdesc.bg_flags = 0;
			bgdesc.bg_exclude_bitmap_lo = 0;
			bgdesc.bg_block_bitmap_csum_lo = 0;
			bgdesc.bg_inode_bitmap_csum_lo = 0;
			bgdesc.bg_itable_unused = 0;
			bgdesc.bg_checksum = 0;
			write(fd, bgdesc, sizeof(ext2_group_desc));
		}	
	}
// need to define inode no
	if (inode_no == 2){	
		 inodes.i_mode = 16877;
--		inodes.i_size = 1 * bs;		
		inodes.i_atime = time(NULL);	
		inodes.i_ctime = time(NULL);	
		inodes.i_mtime = time(NULL);	
		inodes.i_links_count = 3;	
		inodes.i_blocks = inodes.i_size / 512;	
	}
	if (inode_no == 11){	
		inodes.i_mode = 16832;
		inodes.i_size = 4 * bs;		
		inodes.i_atime = time(NULL);	
		inodes.i_ctime = time(NULL);	
		inodes.i_mtime = time(NULL);	
		inodes.i_links_count = 2;	
		inodes.i_blocks = inodes.i_size / 512;	
	}
		inodes.i_uid = 0;		
		inodes.i_dtime = 0;	
		inodes.i_gid = 0;	
		inodes.i_flags = 0;	
		
	union {
		struct {
			inodes.l_i_version = 0; 
		} linux1;
		struct {
			inodes.h_i_translator = 0;
		} hurd1;
	} osd1;				
	
	if (inode_no == 2)				
		inodes.i_block[0] = 1 + 1 + sb.s_reserved_gdt_blocks + 1 + 1 + ((sb.s_inodes_per_group * sb.s_inode_size) / bs);    // 2.15
	else if (inode_no == 11){
		inodes.i_block[0] = 1 + 1 + sb.s_reserved_gdt_blocks + 1 + 1 + ((sb.s_inodes_per_group * sb.s_inode_size) / bs) + 1;    
		inodes.i_block[1] = inodes.i_block[0] + 1;	
		inodes.i_block[2] = inodes.i_block[0] + 2;	
		inodes.i_block[3] = inodes.i_block[0] + 3;
	}

	inodes.i_generation = 0;	
	inodes.i_file_acl = 0;	
	inodes.i_size_high = 0;
	inodes.i_faddr = 0;	
	union {
		struct {
			inodes.l_i_blocks_hi = 0;
			inodes.l_i_file_acl_high = 0;
			inodes.l_i_uid_high = 0;	
			inodes.l_i_gid_high = 0;	
			inodes.l_i_checksum_lo = 0; 
			inodes.l_i_reserved = 0;
		} linux2;
		struct {
			inodes.h_i_frag = 0;	
			inodes.h_i_fsize = 0;	
			inodes.h_i_mode_high = 0;
			inodes.h_i_uid_high = 0;
			inodes.h_i_gid_high = 0;
			inodes.h_i_author = 0;
		} hurd2;
	} osd2;				

	// to be written for inode 2
	
	set_dir_entry(int inode, int record_length, int name_length, int ftype, char * str){
		dirent.inode = inode;
		dirent.rec_len = record_length;
		dirent.name_len = name_length;
		dirent.file_type = ftype;
		strcpy(dirent->name,str);
	}
	
	//for inode 2 (following part is to be written for group 0 only)
	
	//go to the block descriptor and read it
	
	lseek(fd, block_size, SEEK_SET);
	read(fd, &bgdesc, sizeof(struct ext2_group_desc));
	
	//read inodetable location and read inode
	
	lseek(fd, bgdesc.bg_inode_table * block_size + 2*sizeof(struct ext2_inode), SEEK_SET);
	read(fd, &inode, sizeof(struct ext2_inode));
	
	//goto data block location and write structures
	lseek(fd, inode.i_block[0] * block_size, SEEK_SET);
	set_dir_entry(2, 12, 1, 2, ".\0\0\0");	
	write(fd, dirent, sizeof(struct ext2_dir_entry_2));
	
	lseek(fd, dirent.rec_len - sizeof(struct ext2_dir_entry_2), SEEK_CUR);
	set_dir_entry(2, 12, 2, 2, "..\0\0");
	write(fd, dirent, sizeof(struct ext2_dir_entry_2));		
	
	lseek(fd, dirent.rec_len - sizeof(struct ext2_dir_entry_2), SEEK_CUR);
	set_dir_entry(2, 20, 10, 2,"lost+found\0\0");	
	write(fd, dirent, sizeof(struct ext2_dir_entry_2));
	
	
	//for inode 11
	
	//read inodetable location and read inode
	
	lseek(fd, bgdesc.bg_inode_table * block_size + 11*sizeof(struct ext2_inode), SEEK_SET);
	read(fd, &inode, sizeof(struct ext2_inode));
	
	lseek(fd, inode.i_block[0] * block_size, SEEK_SET);
	set_dir_entry(11, 12, 1, 2, ".\0\0\0");
	write(fd, dirent, sizeof(struct ext2_dir_entry_2));
	
	lseek(fd, dirent.rec_len - sizeof(struct ext2_dir_entry_2), SEEK_CUR);
	set_dir_entry(2, 12, 2, 2, "..\0\0");
	write(fd, dirent, sizeof(struct ext2_dir_entry_2));
