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

int main(int argc, char *argv[]) {
	int fd = open(argv[1], O_RDONLY); // argv[1] = /dev/sdb1 
	
	int count, bs = 4096, i = 0;
	struct ext2_super_block sb; 
	struct ext2_group_desc bgdesc;
	struct ext2_inode inode;
	struct ext2_dir_entry dirent1, dirent2;
	unsigned int block_size;
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
		if (i == 0 || i == 1 || i % 3 == 0 || i % 5 == 0 || i % 7 == 0){		
	
			sb.s_inodes_count = sb.s_inodes_per_group * no_of_bgroups;		/* Inodes count */
			sb.s_blocks_count = p.size / bs;				/* Blocks count */



			sb.s_r_blocks_count = (5 * sb.s_blocks_count) / 100;	   /* Reserved blocks count */
			sb.s_free_blocks_count;			/* Free blocks count */
			sb.s_free_inodes_count = sb.s_inodes_count - 11;	/* Free inodes count */
			sb.s_first_data_block = 0;    // 	/* First Data Block */
			sb.s_log_block_size = bs;	/* Block size */
			sb.s_log_cluster_size = bs;	/* Allocation cluster size */
			sb.s_blocks_per_group = bs * 8;	/* # Blocks per group */

			no_of_groups = sb.s_blocks_count / sb.s_blocks_per_group;	

//-11 right shift	
			sb.s_clusters_per_group = 32768;	/* # Fragments per group */
		
			inode_bpg = bs / 8;
	
		--	sb.s_inode_size = 256;		/* size of inode structure */
			sb.s_inodes_per_group = inode_bpg * bs / sb.s_inode_size;	/* # Inodes per group */
			sb.s_mtime = 0;		/* Mount time */
			sb.s_wtime;		/* Write time */
	
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
			sb.s_feature_compat;	/* compatible feature set */
			sb.s_feature_incompat;	/* incompatible feature set */
			sb.s_feature_ro_compat;	/* readonly-compatible feature set */
			sb.s_uuid[16];		/* 128-bit uuid for volume */
			char	s_volume_name[EXT2_LABEL_LEN];	/* volume name */
			char	s_last_mounted[64];	/* directory where last mounted */
			s_algorithm_usage_bitmap; /* For compression */
	


			sb.s_prealloc_blocks;	/* Nr of blocks to try to preallocate*/
			sb.s_prealloc_dir_blocks;	/* Nr to preallocate for dirs */
			sb.s_reserved_gdt_blocks = 127;	/* Per group table for online growth */

			sb.s_journal_uuid[16];	/* uuid of journal superblock */				
			sb.s_journal_inum;		/* inode number of journal file */
			sb.s_journal_dev;		/* device number of journal file */
			sb.s_last_orphan;		/* start of list of inodes to delete */
			sb.s_hash_seed[4];		/* HTREE hash seed */
			sb.s_def_hash_version;	/* Default hash version to use */
			sb.s_jnl_backup_type; 	/* Default type of journal backup */		
			sb.s_desc_size;		/* Group desc. size: INCOMPAT_64BIT */
			sb.s_default_mount_opts;
			sb.s_first_meta_bg;	/* First metablock group */
			sb.s_mkfs_time;		/* When the filesystem was created */
			sb.s_jnl_blocks[17]; 	/* Backup of the journal inode */
			sb.s_blocks_count_hi;	/* Blocks count high 32bits */
			sb.s_r_blocks_count_hi;	/* Reserved blocks count high 32 bits*/
			sb.s_free_blocks_hi; 	/* Free blocks count */
			sb.s_min_extra_isize;	/* All inodes have at least # bytes */
			sb.s_want_extra_isize; 	/* New inodes should reserve # bytes */
			sb.s_flags;		/* Miscellaneous flags */
			sb.s_raid_stride;		/* RAID stride */
			sb.s_mmp_update_interval;  /* # seconds to wait in MMP checking */
			sb.s_mmp_block;            /* Block for multi-mount protection */
			sb.s_raid_stripe_width;    /* blocks on all data disks (N*stride)*/
			sb.s_log_groups_per_flex;	/* FLEX_BG group size */
			sb.s_checksum_type;	/* metadata checksum algorithm */
			sb.s_encryption_level;	/* versioning level for encryption */
	sb.s_reserved_pad;		/* Padding to next 32bits */
	sb.s_kbytes_written;	/* nr of lifetime kilobytes written */
	sb.s_snapshot_inum;	/* Inode number of active snapshot */
	sb.s_snapshot_id;		/* sequential ID of active snapshot */
	sb.s_snapshot_r_blocks_count; /* reserved blocks for active
					      snapshot's future use */
	sb.s_snapshot_list;	/* inode number of the head of the on-disk snapshot list */
#define EXT4_S_ERR_START ext4_offsetof(struct ext2_super_block, s_error_count)
	
	sb.s_error_count;		/* number of fs errors */
	sb.s_first_error_time;	/* first time an error happened */
	sb.s_first_error_ino;	/* inode involved in first error */
	sb.s_first_error_block;	/* block involved of first error */
	sb.s_first_error_func[32];	/* function where the error happened */
	sb.s_first_error_line;	/* line number where error happened */
	sb.s_last_error_time;	/* most recent time of an error */
	sb.s_last_error_ino;	/* inode involved in last error */
	sb.s_last_error_line;	/* line number where error happened */
	sb.s_last_error_block;	/* block involved of last error */
	sb.s_last_error_func[32];	/* function where the error happened */
#define EXT4_S_ERR_END ext4_offsetof(struct ext2_super_block, s_mount_opts)
	

	sb.s_mount_opts[64];
	sb.s_usr_quota_inum;	/* inode number of user quota file */
	sb.s_grp_quota_inum;	/* inode number of group quota file */
	sb.s_overhead_blocks;	/* overhead blocks/clusters in fs */
	sb.s_backup_bgs[2];	/* If sparse_super2 enabled */
	sb.s_encrypt_algos[4];	/* Encryption algorithms in use  */
	sb.s_encrypt_pw_salt[16];	/* Salt used for string2key algorithm */
	sb.s_lpf_ino;		/* Location of the lost+found inode */
	sb.s_prj_quota_inum;	/* inode for tracking project quota */
	sb.s_checksum_seed;	/* crc32c(orig_uuid) if csum_seed set */
	sb.s_reserved[98];		/* Padding to the end of the block */
	sb.s_checksum;		/* crc32c(superblock) */

// for group 0
	int skip;
	
			skip = 32768 * i;
			bgdesc.bg_block_bitmap = skip + 1 + 1 + sb.s_reserved_gdt_blocks;
			bgdesc.bg_inode_bitmap = bgdesc.bg_block_bitmap + 1;
			bgdesc.bg_inode_table = bgdesc.bg_block_bitmap + 2;
							
			if (i == 0){
				bgdesc.bg_free_blocks_count = sb.s_blocks_per_group - ;		
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
			write(fd, );
		}	
	}
// need to define inode no
	if (inode_no == 2){	
		inodes.i_mode = 16877;
--		inodes.i_size = 1 * bs;		
		inodes.i_atime;	
		inodes.i_ctime;	
		inodes.i_mtime;	
		inodes.i_links_count = 3;	
		inodes.i_blocks = 1 * bs / 512;	
	}
	if (inode_no == 11){	
		inodes.i_mode = 16832;
		inodes.i_size = 4 * bs;		
		inodes.i_atime;	
		inodes.i_ctime;	
		inodes.i_mtime;	
		inodes.i_links_count = 2;	
		inodes.i_blocks = 4 * bs / 512;	
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
	inodes.i_block[EXT2_N_BLOCKS] = 643;    // 2.15
	644-647
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


	dirent1.inode = 2;
	dirent1.rec_len = 12;
	dirent1.name_len = 1;
	strcpy(dirent1->name[0],".");

	dirent2.inode = 11;
	dirent2.rec_len = 12;
	dirent2.name_len = 2;
	strcpy(dirent2->name[0],"..")

