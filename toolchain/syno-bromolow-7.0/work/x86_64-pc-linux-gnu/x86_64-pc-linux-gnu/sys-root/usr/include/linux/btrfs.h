/*
 * Copyright (C) 2007 Oracle.  All rights reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License v2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 021110-1307, USA.
 */

#ifndef _LINUX_BTRFS_H
#define _LINUX_BTRFS_H
#include <linux/types.h>
#include <linux/ioctl.h>


#define BTRFS_IOCTL_MAGIC 0x94
#define BTRFS_VOL_NAME_MAX 255

/* this should be 4k */
#define BTRFS_PATH_NAME_MAX 4087
struct btrfs_ioctl_vol_args {
	__s64 fd;
	char name[BTRFS_PATH_NAME_MAX + 1];
};

#define BTRFS_DEVICE_PATH_NAME_MAX 1024

#define BTRFS_SUBVOL_CREATE_ASYNC	(1ULL << 0)
#define BTRFS_SUBVOL_RDONLY		(1ULL << 1)
#define BTRFS_SUBVOL_QGROUP_INHERIT	(1ULL << 2)
#ifdef CONFIG_SYNO_BTRFS_SUBVOLUME_HIDE
#define BTRFS_SUBVOL_HIDE (1ULL << 32)
#endif /* CONFIG_SYNO_BTRFS_SUBVOLUME_HIDE */

#define BTRFS_FSID_SIZE 16
#define BTRFS_UUID_SIZE 16
#define BTRFS_UUID_UNPARSED_SIZE	37

#define BTRFS_QGROUP_INHERIT_SET_LIMITS	(1ULL << 0)

struct btrfs_qgroup_limit {
	__u64	flags;
	__u64	max_rfer;
	__u64	max_excl;
	__u64	rsv_rfer;
	__u64	rsv_excl;
};

struct btrfs_qgroup_inherit {
	__u64	flags;
	__u64	num_qgroups;
	__u64	num_ref_copies;
	__u64	num_excl_copies;
	struct btrfs_qgroup_limit lim;
	__u64	qgroups[0];
};

struct btrfs_ioctl_qgroup_limit_args {
	__u64	qgroupid;
	struct btrfs_qgroup_limit lim;
};

#define BTRFS_SUBVOL_NAME_MAX 4039
struct btrfs_ioctl_vol_args_v2 {
	__s64 fd;
	__u64 transid;
	__u64 flags;
	union {
		struct {
			__u64 size;
			struct btrfs_qgroup_inherit *qgroup_inherit;
		};
		__u64 unused[4];
	};
	char name[BTRFS_SUBVOL_NAME_MAX + 1];
};

/*
 * structure to report errors and progress to userspace, either as a
 * result of a finished scrub, a canceled scrub or a progress inquiry
 */
struct btrfs_scrub_progress {
	__u64 data_extents_scrubbed;	/* # of data extents scrubbed */
	__u64 tree_extents_scrubbed;	/* # of tree extents scrubbed */
	__u64 data_bytes_scrubbed;	/* # of data bytes scrubbed */
	__u64 tree_bytes_scrubbed;	/* # of tree bytes scrubbed */
	__u64 read_errors;		/* # of read errors encountered (EIO) */
	__u64 csum_errors;		/* # of failed csum checks */
	__u64 verify_errors;		/* # of occurences, where the metadata
					 * of a tree block did not match the
					 * expected values, like generation or
					 * logical */
	__u64 no_csum;			/* # of 4k data block for which no csum
					 * is present, probably the result of
					 * data written with nodatasum */
	__u64 csum_discards;		/* # of csum for which no data was found
					 * in the extent tree. */
	__u64 super_errors;		/* # of bad super blocks encountered */
	__u64 malloc_errors;		/* # of internal kmalloc errors. These
					 * will likely cause an incomplete
					 * scrub */
	__u64 uncorrectable_errors;	/* # of errors where either no intact
					 * copy was found or the writeback
					 * failed */
	__u64 corrected_errors;		/* # of errors corrected */
	__u64 last_physical;		/* last physical address scrubbed. In
					 * case a scrub was aborted, this can
					 * be used to restart the scrub */
	__u64 unverified_errors;	/* # of occurences where a read for a
					 * full (64k) bio failed, but the re-
					 * check succeeded for each 4k piece.
					 * Intermittent error. */
};

#define BTRFS_SCRUB_READONLY	1
struct btrfs_ioctl_scrub_args {
	__u64 devid;				/* in */
	__u64 start;				/* in */
	__u64 end;				/* in */
	__u64 flags;				/* in */
	struct btrfs_scrub_progress progress;	/* out */
	/* pad to 1k */
	__u64 unused[(1024-32-sizeof(struct btrfs_scrub_progress))/8];
};

#define BTRFS_IOCTL_DEV_REPLACE_CONT_READING_FROM_SRCDEV_MODE_ALWAYS	0
#define BTRFS_IOCTL_DEV_REPLACE_CONT_READING_FROM_SRCDEV_MODE_AVOID	1
struct btrfs_ioctl_dev_replace_start_params {
	__u64 srcdevid;	/* in, if 0, use srcdev_name instead */
	__u64 cont_reading_from_srcdev_mode;	/* in, see #define
						 * above */
	__u8 srcdev_name[BTRFS_DEVICE_PATH_NAME_MAX + 1];	/* in */
	__u8 tgtdev_name[BTRFS_DEVICE_PATH_NAME_MAX + 1];	/* in */
};

#define BTRFS_IOCTL_DEV_REPLACE_STATE_NEVER_STARTED	0
#define BTRFS_IOCTL_DEV_REPLACE_STATE_STARTED		1
#define BTRFS_IOCTL_DEV_REPLACE_STATE_FINISHED		2
#define BTRFS_IOCTL_DEV_REPLACE_STATE_CANCELED		3
#define BTRFS_IOCTL_DEV_REPLACE_STATE_SUSPENDED		4
struct btrfs_ioctl_dev_replace_status_params {
	__u64 replace_state;	/* out, see #define above */
	__u64 progress_1000;	/* out, 0 <= x <= 1000 */
	__u64 time_started;	/* out, seconds since 1-Jan-1970 */
	__u64 time_stopped;	/* out, seconds since 1-Jan-1970 */
	__u64 num_write_errors;	/* out */
	__u64 num_uncorrectable_read_errors;	/* out */
};

#define BTRFS_IOCTL_DEV_REPLACE_CMD_START			0
#define BTRFS_IOCTL_DEV_REPLACE_CMD_STATUS			1
#define BTRFS_IOCTL_DEV_REPLACE_CMD_CANCEL			2
#define BTRFS_IOCTL_DEV_REPLACE_RESULT_NO_ERROR			0
#define BTRFS_IOCTL_DEV_REPLACE_RESULT_NOT_STARTED		1
#define BTRFS_IOCTL_DEV_REPLACE_RESULT_ALREADY_STARTED		2
struct btrfs_ioctl_dev_replace_args {
	__u64 cmd;	/* in */
	__u64 result;	/* out */

	union {
		struct btrfs_ioctl_dev_replace_start_params start;
		struct btrfs_ioctl_dev_replace_status_params status;
	};	/* in/out */

	__u64 spare[64];
};

struct btrfs_ioctl_dev_info_args {
	__u64 devid;				/* in/out */
	__u8 uuid[BTRFS_UUID_SIZE];		/* in/out */
	__u64 bytes_used;			/* out */
	__u64 total_bytes;			/* out */
	__u64 unused[379];			/* pad to 4k */
	__u8 path[BTRFS_DEVICE_PATH_NAME_MAX];	/* out */
};

struct btrfs_ioctl_fs_info_args {
	__u64 max_id;				/* out */
	__u64 num_devices;			/* out */
	__u8 fsid[BTRFS_FSID_SIZE];		/* out */
	__u32 nodesize;				/* out */
	__u32 sectorsize;			/* out */
	__u32 clone_alignment;			/* out */
	__u32 reserved32;
	__u64 reserved[122];			/* pad to 1k */
};

/*
 * feature flags
 *
 * Used by:
 * struct btrfs_ioctl_feature_flags
 */
#define BTRFS_FEATURE_COMPAT_RO_FREE_SPACE_TREE	(1ULL << 0)

/*
 * Older kernels (< 4.9) on big-endian systems produced broken free space tree
 * bitmaps, and btrfs-progs also used to corrupt the free space tree (versions
 * < 4.7.3).  If this bit is clear, then the free space tree cannot be trusted.
 * btrfs-progs can also intentionally clear this bit to ask the kernel to
 * rebuild the free space tree, however this might not work on older kernels
 * that do not know about this bit. If not sure, clear the cache manually on
 * first mount when booting older kernel versions.
 */
#define BTRFS_FEATURE_COMPAT_RO_FREE_SPACE_TREE_VALID	(1ULL << 1)

#define BTRFS_FEATURE_INCOMPAT_MIXED_BACKREF	(1ULL << 0)
#define BTRFS_FEATURE_INCOMPAT_DEFAULT_SUBVOL	(1ULL << 1)
#define BTRFS_FEATURE_INCOMPAT_MIXED_GROUPS	(1ULL << 2)
#define BTRFS_FEATURE_INCOMPAT_COMPRESS_LZO	(1ULL << 3)
#define BTRFS_FEATURE_INCOMPAT_COMPRESS_ZSTD	(1ULL << 4)

/*
 * older kernels tried to do bigger metadata blocks, but the
 * code was pretty buggy.  Lets not let them try anymore.
 */
#define BTRFS_FEATURE_INCOMPAT_BIG_METADATA	(1ULL << 5)

#define BTRFS_FEATURE_INCOMPAT_EXTENDED_IREF	(1ULL << 6)
#define BTRFS_FEATURE_INCOMPAT_RAID56		(1ULL << 7)
#define BTRFS_FEATURE_INCOMPAT_SKINNY_METADATA	(1ULL << 8)
#define BTRFS_FEATURE_INCOMPAT_NO_HOLES		(1ULL << 9)

struct btrfs_ioctl_feature_flags {
	__u64 compat_flags;
	__u64 compat_ro_flags;
	__u64 incompat_flags;
};

/* balance control ioctl modes */
#define BTRFS_BALANCE_CTL_PAUSE		1
#define BTRFS_BALANCE_CTL_CANCEL	2

/*
 * this is packed, because it should be exactly the same as its disk
 * byte order counterpart (struct btrfs_disk_balance_args)
 */
struct btrfs_balance_args {
	__u64 profiles;
	__u64 usage;
	__u64 devid;
	__u64 pstart;
	__u64 pend;
	__u64 vstart;
	__u64 vend;

	__u64 target;

	__u64 flags;

	__u64 limit;		/* limit number of processed chunks */
	__u64 unused[7];
} __attribute__ ((__packed__));

/* report balance progress to userspace */
struct btrfs_balance_progress {
	__u64 expected;		/* estimated # of chunks that will be
				 * relocated to fulfill the request */
	__u64 considered;	/* # of chunks we have considered so far */
	__u64 completed;	/* # of chunks relocated so far */
};

#define BTRFS_BALANCE_STATE_RUNNING	(1ULL << 0)
#define BTRFS_BALANCE_STATE_PAUSE_REQ	(1ULL << 1)
#define BTRFS_BALANCE_STATE_CANCEL_REQ	(1ULL << 2)

struct btrfs_ioctl_balance_args {
	__u64 flags;				/* in/out */
	__u64 state;				/* out */

	struct btrfs_balance_args data;		/* in/out */
	struct btrfs_balance_args meta;		/* in/out */
	struct btrfs_balance_args sys;		/* in/out */

	struct btrfs_balance_progress stat;	/* out */

	__u64 unused[72];			/* pad to 1k */
};

#define BTRFS_INO_LOOKUP_PATH_MAX 4080
struct btrfs_ioctl_ino_lookup_args {
	__u64 treeid;
	__u64 objectid;
	char name[BTRFS_INO_LOOKUP_PATH_MAX];
};

struct btrfs_ioctl_search_key {
	/* which root are we searching.  0 is the tree of tree roots */
	__u64 tree_id;

	/* keys returned will be >= min and <= max */
	__u64 min_objectid;
	__u64 max_objectid;

	/* keys returned will be >= min and <= max */
	__u64 min_offset;
	__u64 max_offset;

	/* max and min transids to search for */
	__u64 min_transid;
	__u64 max_transid;

	/* keys returned will be >= min and <= max */
	__u32 min_type;
	__u32 max_type;

	/*
	 * how many items did userland ask for, and how many are we
	 * returning
	 */
	__u32 nr_items;

	/* align to 64 bits */
	__u32 unused;

	/* some extra for later */
	__u64 unused1;
	__u64 unused2;
	__u64 unused3;
	__u64 unused4;
};

struct btrfs_ioctl_search_header {
	__u64 transid;
	__u64 objectid;
	__u64 offset;
	__u32 type;
	__u32 len;
};

#define BTRFS_SEARCH_ARGS_BUFSIZE (4096 - sizeof(struct btrfs_ioctl_search_key))
/*
 * the buf is an array of search headers where
 * each header is followed by the actual item
 * the type field is expanded to 32 bits for alignment
 */
struct btrfs_ioctl_search_args {
	struct btrfs_ioctl_search_key key;
	char buf[BTRFS_SEARCH_ARGS_BUFSIZE];
};

struct btrfs_ioctl_search_args_v2 {
	struct btrfs_ioctl_search_key key; /* in/out - search parameters */
	__u64 buf_size;		   /* in - size of buffer
					    * out - on EOVERFLOW: needed size
					    *       to store item */
	__u64 buf[0];                       /* out - found items */
};

struct btrfs_ioctl_clone_range_args {
  __s64 src_fd;
  __u64 src_offset, src_length;
  __u64 dest_offset;
};

#ifdef CONFIG_SYNO_BTRFS_CLONE_RANGE_V2
#define BTRFS_CLONE_RANGE_V2_AUTO_REWRITE_SRC (1 << 0)
#define BTRFS_CLONE_RANGE_V2_AUTO_REWRITE_DST (1 << 1)

struct btrfs_ioctl_syno_clone_range_args_v2 {
	__s64 src_fd;
	/*
	 * If the ref_limit is reached,
	 * src_offset and src_lenght will be set to appropriate file
	 * offset and lenght to indicate which part of the file reaches
	 * the extent reflink limit. errno EMLINK is returned.
	 * When EMLINK, src_offset and src_length will be set to the
	 * position and lenght of file that could not be cloned,
	 * and ref_limit is set to the current reflinnk count of
	 * that extent.
	 */
	__u64 src_offset;	/* in/out */
	__u64 src_length;	/* in/out */
	__u64 dest_offset;
	__u64 ref_limit;	/* in/out */
	__u32 flag;
	__u32 reserved32;
	__u64 reserved[2];
};
#endif /* CONFIG_SYNO_BTRFS_CLONE_RANGE_V2 */
/*
 * flags definition for the defrag range ioctl
 *
 * Used by:
 * struct btrfs_ioctl_defrag_range_args.flags
 */
#define BTRFS_DEFRAG_RANGE_COMPRESS 1
#define BTRFS_DEFRAG_RANGE_START_IO 2
#ifdef CONFIG_SYNO_BTRFS_RECLAIM_SPACE
#define BTRFS_DEFRAG_RANGE_SYNO_DEFRAG  (1ULL << 2)
#define BTRFS_DEFRAG_RANGE_PRINT_STDOUT (1ULL << 3)
#define BTRFS_DEFRAG_RANGE_SKIP_FAST_SNAPSHOT_CHECK  (1ULL << 4)
#endif /* CONFIG_SYNO_BTRFS_RECLAIM_SPACE */
struct btrfs_ioctl_defrag_range_args {
	/* start of the defrag operation */
	__u64 start;

	/* number of bytes to defrag, use (u64)-1 to say all */
	__u64 len;

	/*
	 * flags for the operation, which can include turning
	 * on compression for this one defrag
	 */
	__u64 flags;

	/*
	 * any extent bigger than this will be considered
	 * already defragged.  Use 0 to take the kernel default
	 * Use 1 to say every single extent must be rewritten
	 */
	__u32 extent_thresh;

	/*
	 * which compression method to use if turning on compression
	 * for this defrag operation.  If unspecified, zlib will
	 * be used
	 */
	__u32 compress_type;

#ifdef CONFIG_SYNO_BTRFS_RECLAIM_SPACE
	/*
	 * This value is multiple of 4K
	 */
	__u16 syno_thresh;
	/*
	 * The ratio between number of bytes of file extent data that
	 * point to processing extent and extent disk byte number.
	 * If actual ratio is below this condition, defrag would be
	 * triggered. For example syno_ratio_denom=3 and syno_ratio_nom=2,
	 * if the (file extent num) <= (extent disk byte num) * 2/3,
	 * this extent would be defragged.
	 */
	__u8 syno_ratio_denom;
	__u8 syno_ratio_nom;
	__u32 unused[3];
#else
	/* spare for later */
	__u32 unused[4];
#endif /* CONFIG_SYNO_BTRFS_RECLAIM_SPACE */
};

#define BTRFS_SAME_DATA_DIFFERS	1
/* For extent-same ioctl */
struct btrfs_ioctl_same_extent_info {
	__s64 fd;		/* in - destination file */
	__u64 logical_offset;	/* in - start of extent in destination */
	__u64 bytes_deduped;	/* out - total # of bytes we were able
				 * to dedupe from this file */
	/* status of this dedupe operation:
	 * 0 if dedup succeeds
	 * < 0 for error
	 * == BTRFS_SAME_DATA_DIFFERS if data differs
	 */
	__s32 status;		/* out - see above description */
	__u32 reserved;
};

struct btrfs_ioctl_same_args {
	__u64 logical_offset;	/* in - start of extent in source */
	__u64 length;		/* in - length of extent */
	__u16 dest_count;	/* in - total elements in info array */
	__u16 reserved1;
	__u32 reserved2;
	struct btrfs_ioctl_same_extent_info info[0];
};

struct btrfs_ioctl_space_info {
	__u64 flags;
	__u64 total_bytes;
	__u64 used_bytes;
};

struct btrfs_ioctl_space_args {
	__u64 space_slots;
	__u64 total_spaces;
	struct btrfs_ioctl_space_info spaces[0];
};

struct btrfs_data_container {
	__u32	bytes_left;	/* out -- bytes not needed to deliver output */
	__u32	bytes_missing;	/* out -- additional bytes needed for result */
	__u32	elem_cnt;	/* out */
	__u32	elem_missed;	/* out */
	__u64	val[0];		/* out */
};

struct btrfs_ioctl_ino_path_args {
	__u64				inum;		/* in */
	__u64				size;		/* in */
	__u64				reserved[4];
	/* struct btrfs_data_container	*fspath;	   out */
	__u64				fspath;		/* out */
};

struct btrfs_ioctl_logical_ino_args {
	__u64				logical;	/* in */
	__u64				size;		/* in */
	__u64				reserved[4];
	/* struct btrfs_data_container	*inodes;	out   */
	__u64				inodes;
};

enum btrfs_dev_stat_values {
	/* disk I/O failure stats */
	BTRFS_DEV_STAT_WRITE_ERRS, /* EIO or EREMOTEIO from lower layers */
	BTRFS_DEV_STAT_READ_ERRS, /* EIO or EREMOTEIO from lower layers */
	BTRFS_DEV_STAT_FLUSH_ERRS, /* EIO or EREMOTEIO from lower layers */

	/* stats for indirect indications for I/O failures */
	BTRFS_DEV_STAT_CORRUPTION_ERRS, /* checksum error, bytenr error or
					 * contents is illegal: this is an
					 * indication that the block was damaged
					 * during read or write, or written to
					 * wrong location or read from wrong
					 * location */
	BTRFS_DEV_STAT_GENERATION_ERRS, /* an indication that blocks have not
					 * been written */

	BTRFS_DEV_STAT_VALUES_MAX
};

/* Reset statistics after reading; needs SYS_ADMIN capability */
#define	BTRFS_DEV_STATS_RESET		(1ULL << 0)

struct btrfs_ioctl_get_dev_stats {
	__u64 devid;				/* in */
	__u64 nr_items;				/* in/out */
	__u64 flags;				/* in/out */

	/* out values: */
	__u64 values[BTRFS_DEV_STAT_VALUES_MAX];

	__u64 unused[128 - 2 - BTRFS_DEV_STAT_VALUES_MAX]; /* pad to 1k */
};

#define BTRFS_QUOTA_CTL_ENABLE	1
#define BTRFS_QUOTA_CTL_DISABLE	2
#define BTRFS_QUOTA_CTL_RESCAN__NOTUSED	3
struct btrfs_ioctl_quota_ctl_args {
	__u64 cmd;
	__u64 status;
};

struct btrfs_ioctl_quota_rescan_args {
	__u64	flags;
	__u64   progress;
	__u64   reserved[6];
};

struct btrfs_ioctl_qgroup_assign_args {
	__u64 assign;
	__u64 src;
	__u64 dst;
};

struct btrfs_ioctl_qgroup_create_args {
	__u64 create;
	__u64 qgroupid;
};

#ifdef CONFIG_SYNO_BTRFS_QGROUP_QUERY
struct btrfs_ioctl_qgroup_query_args {
	/* state */
	__u64 rfer;       // disk size (could be smaller than rfer_cmpr due to compression)
	__u64 rfer_cmpr;  // original size before compression (bad naming due to compatibility)
	__u64 excl;
	__u64 excl_cmpr;

	/* limit */
	__u64 max_rfer;
	__u64 max_excl;
	__u64 rsv_rfer;
	__u64 rsv_excl;

	/* reservation tracking */
	__u64 reserved;
};
#endif /* CONFIG_SYNO_BTRFS_QGROUP_QUERY */

struct btrfs_ioctl_timespec {
	__u64 sec;
	__u32 nsec;
};

struct btrfs_ioctl_received_subvol_args {
	char	uuid[BTRFS_UUID_SIZE];	/* in */
	__u64	stransid;		/* in */
	__u64	rtransid;		/* out */
	struct btrfs_ioctl_timespec stime; /* in */
	struct btrfs_ioctl_timespec rtime; /* out */
	__u64	flags;			/* in */
#ifdef CONFIG_SYNO_BTRFS_SEND
	struct btrfs_ioctl_timespec otime; /* in */
	__u64	reserved[14];		/* in */
#else
	__u64	reserved[16];		/* in */
#endif /* CONFIG_SYNO_BTRFS_SEND */
};

#ifdef CONFIG_SYNO_BTRFS_SEND
struct btrfs_ioctl_subvol_info_args {
	/* this root's id */
	__u64 root_id;
	/* flags of the root */
	__u64 flags;
	/* generation when the root is created or last updated */
	__u64 gen;
	/* creation generation of this root in sec*/
	__u64 ogen;
	__u8 uuid[BTRFS_UUID_SIZE];
	__u8 puuid[BTRFS_UUID_SIZE];
	__u8 ruuid[BTRFS_UUID_SIZE];
};
#endif /* CONFIG_SYNO_BTRFS_SEND */

#ifdef CONFIG_SYNO_BTRFS_SNAPSHOT_SIZE_CALCULATION

#define BTRFS_SNAP_SIZE_SHOW_EXCL_SIZE 0x1
#define BTRFS_SNAP_SIZE_SHOW_PROCESSED_SIZE 0x2
#define BTRFS_SNAP_SIZE_SHOW_MARGINAL_SIZE 0x4

struct btrfs_ioctl_snapshot_size_id_size_map {
	__u64 snap_id;
	__u64 marginal_size;
};

struct btrfs_ioctl_snapshot_size_query_args {
	/* number of snap ids pointed by *snap_id */
	__u64 snap_count;
	__u64 flags;
	/* output exclusive size in progress to fd

		if BTRFS_SNAP_SIZE_QUERY_OUTPUT_READALBE_PRGRESS
		   BTRFS_SNAP_SIZE_SHOW_PROCESSED_SIZE
		   BTRFS_SNAP_SIZE_SHOW_MARGINAL_SIZE
		are all set
		the output format will like:

			subvol(360) 0 bytes
			subvol(361) 0 bytes
			subvol(362) 2048 bytes
			        .
			        .
			subvol(512) 40967 bytes
			exclusive  43014 bytes
			processed  203503 bytes

			subvol(360) 0 bytes
			subvol(361) 12802 bytes
			subvol(362) 4096 bytes
			        .
			        .
			subvol(512) 60967 bytes
			exclusive 77865 bytes
			processed 302122 bytes
		        .
		        .

	*/
	__s64 fd;
	/* address of snap ids/marginal size map to caluclate */
	struct btrfs_ioctl_snapshot_size_id_size_map *id_maps;
	/* exclusive size in byte */
	__u64 calc_size;
	__u64 processed_size;
};
#endif /* CONFIG_SYNO_BTRFS_SNAPSHOT_SIZE_CALCULATION */

#ifdef CONFIG_SYNO_BTRFS_LOG_TREE_RSV_METADATA
#define BTRFS_LOG_TREE_BG_RSV_ADD (1ULL << 0)
#define BTRFS_LOG_TREE_BG_RSV_REMOVE (1ULL << 1)
#define BTRFS_LOG_TREE_BG_RSV_FLAGS \
               (BTRFS_LOG_TREE_BG_RSV_ADD | \
                BTRFS_LOG_TREE_BG_RSV_REMOVE)

struct btrfs_ioctl_log_tree_reserve_bg_args {
	__u64 start;
	__u64 size;
	__u64 map_start;
	__u64 flags;
	__u64 reserved[4];
};
#endif /* CONFIG_SYNO_BTRFS_LOG_TREE_RSV_METADATA */

#ifdef CONFIG_SYNO_BTRFS_METADATA_RESERVE
struct btrfs_ioctl_syno_punch_check_args {
	__u64 offset;
	__u64 len;
	__u64 extent_offset;
	__u64 extent_len;
	__u64 reserved[4];
};
#endif /* CONFIG_SYNO_BTRFS_METADATA_RESERVE */

#ifdef CONFIG_SYNO_BTRFS_FREE_SPACE_ANALYZE
#define BTRFS_FREE_SPACE_ANALYZE_NR_INTERVAL	15
#define BTRFS_FREE_SPACE_ANALYZE_FLAG_FULL	(1 << 0)

struct btrfs_ioctl_free_space_analyze_args {
	/* arguments */
	__u64 flags;
	__u64 min_continuous_size;

	/* results */
	__u64 interval_cnts[BTRFS_FREE_SPACE_ANALYZE_NR_INTERVAL];  // For intervals from [4K,8K), [8K,16K) to [64M,128M)
	__u64 frag_cnts;              // Count of the free blocks which < min_continuous_size
	__u64 continuous_cnts;        // Count of the free blocks which >= min_continuous_size
	__u64 total_frag_size;        // The total size of the free blocks which < min_continuous_size
	__u64 total_continuous_size;  // The total size of the free blocks which >= min_continuous_size

	/* reserved */
	__u64 reserved[8];
};
#endif /* CONFIG_SYNO_BTRFS_FREE_SPACE_ANALYZE */

/*
 * Caller doesn't want file data in the send stream, even if the
 * search of clone sources doesn't find an extent. UPDATE_EXTENT
 * commands will be sent instead of WRITE commands.
 */
#define BTRFS_SEND_FLAG_NO_FILE_DATA		0x1

/*
 * Do not add the leading stream header. Used when multiple snapshots
 * are sent back to back.
 */
#define BTRFS_SEND_FLAG_OMIT_STREAM_HEADER	0x2

/*
 * Omit the command at the end of the stream that indicated the end
 * of the stream. This option is used when multiple snapshots are
 * sent back to back.
 */
#define BTRFS_SEND_FLAG_OMIT_END_CMD		0x4

#if defined(CONFIG_SYNO_BTRFS_SEND_CALCULATE_TOTAL_DATA_SIZE) || \
    defined(CONFIG_SYNO_BTRFS_SEND_SKIP_FIND_CLONE) || \
	defined(CONFIG_SYNO_BTRFS_SEND_FALLOCATE_SUPPORT) || \
	defined(CONFIG_SYNO_BTRFS_SEND_FALLBACK_COMPRESSION)
/*
 * In order to simplify how BTRFS_SEND_FLAG_MASK works with syno defined flags,
 * all syno defined flags are defiend if any of the corresponding config is
 * turn on.
 */
/*
 * Calculate the amount (in bytes) of new file data between the send and
 * parent snapshots, or in case of a full send, the total amount of file data
 * we will send.
 */
#define BTRFS_SEND_FLAG_CALCULATE_DATA_SIZE    0x8

/*
 * find_extent_clone in send could take a lot of time but end up find nothing
 * to clone, especially in iSCSI BLun thick provision. This flag is used
 * to indicate send to skip find_extent_clone.
 */
#define BTRFS_SEND_FLAG_SKIP_FIND_CLONE    0x10

/*
 * Use fallocate command to pre-allocate file extents and punch file holes,
 * instead of write commands with data buffers filled with 0 value bytes.
 */
#define BTRFS_SEND_FLAG_SUPPORT_FALLOCATE       0x20

/*
 * Convert value of xattr btrfs.compression of set_xattr from "zstd" to "lzo"
 */
#define BTRFS_SEND_FLAG_FALLBACK_COMPRESSION    0x40

#define BTRFS_SEND_FLAG_MASK \
	(BTRFS_SEND_FLAG_NO_FILE_DATA | \
	 BTRFS_SEND_FLAG_OMIT_STREAM_HEADER | \
	 BTRFS_SEND_FLAG_CALCULATE_DATA_SIZE | \
	 BTRFS_SEND_FLAG_SKIP_FIND_CLONE | \
	 BTRFS_SEND_FLAG_SUPPORT_FALLOCATE | \
	 BTRFS_SEND_FLAG_FALLBACK_COMPRESSION | \
	 BTRFS_SEND_FLAG_OMIT_END_CMD)
#else
#define BTRFS_SEND_FLAG_MASK \
	(BTRFS_SEND_FLAG_NO_FILE_DATA | \
	 BTRFS_SEND_FLAG_OMIT_STREAM_HEADER | \
	 BTRFS_SEND_FLAG_OMIT_END_CMD)
#endif /* CONFIG_SYNO_BTRFS_SEND_CALCULATE_TOTAL_DATA_SIZE || \
          CONFIG_SYNO_BTRFS_SEND_SKIP_FIND_CLONE || \
		  CONFIG_SYNO_BTRFS_SEND_FALLOCATE_SUPPORT || \
		  CONFIG_SYNO_BTRFS_SEND_FALLBACK_COMPRESSION */

struct btrfs_ioctl_send_args {
	__s64 send_fd;			/* in */
	__u64 clone_sources_count;	/* in */
	__u64 *clone_sources;	/* in */
	__u64 parent_root;		/* in */
	__u64 flags;			/* in */
#ifdef CONFIG_SYNO_BTRFS_SEND_CALCULATE_TOTAL_DATA_SIZE
	__u64 total_data_size;   /* out */
	__u32 g_verbose;
#endif
#ifdef CONFIG_SYNO_BTRFS_SEND_SUPPORT_PAUSE_RESUME
	__u64 skip_cmd_count;
#endif /* CONFIG_SYNO_BTRFS_SEND_SUPPORT_PAUSE_RESUME */

#if defined(CONFIG_SYNO_BTRFS_SEND_CALCULATE_TOTAL_DATA_SIZE) && defined(CONFIG_SYNO_BTRFS_SEND_SUPPORT_PAUSE_RESUME)
	__u32 reserved_u32;
	__u64 reserved[1];		/* in */
#elif defined(CONFIG_SYNO_BTRFS_SEND_CALCULATE_TOTAL_DATA_SIZE)
	__u32 reserved_u32;
	__u64 reserved[2];		/* in */
#elif defined(CONFIG_SYNO_BTRFS_SEND_SUPPORT_PAUSE_RESUME)
	__u64 reserved[3];		/* in */
#else
	__u64 reserved[4];		/* in */
#endif /* defined(CONFIG_SYNO_BTRFS_SEND_CALCULATE_TOTAL_DATA_SIZE) && defined(CONFIG_SYNO_BTRFS_SEND_SUPPORT_PAUSE_RESUME) */
};

#ifdef CONFIG_SYNO_BTRFS_COMPR_CTL
/* flags for the compression ioctl */
#define BTRFS_COMPR_CTL_SET			0x1
#define BTRFS_COMPR_CTL_COMPR_FL	0x2

struct btrfs_ioctl_compr_ctl_args {
	__u64	flags;				/* in/out */
	__u64	size;				/* out */
	__u64	compressed_size;	/* out */
	__u64	reserved[1];
};
#endif /* CONFIG_SYNO_BTRFS_COMPR_CTL */

/* Error codes as returned by the kernel */
enum btrfs_err_code {
	notused,
	BTRFS_ERROR_DEV_RAID1_MIN_NOT_MET,
	BTRFS_ERROR_DEV_RAID10_MIN_NOT_MET,
	BTRFS_ERROR_DEV_RAID5_MIN_NOT_MET,
	BTRFS_ERROR_DEV_RAID6_MIN_NOT_MET,
	BTRFS_ERROR_DEV_TGT_REPLACE,
	BTRFS_ERROR_DEV_MISSING_NOT_FOUND,
	BTRFS_ERROR_DEV_ONLY_WRITABLE,
	BTRFS_ERROR_DEV_EXCL_RUN_IN_PROGRESS
};
/* An error code to error string mapping for the kernel
*  error codes
*/
static __inline__ char *btrfs_err_str(enum btrfs_err_code err_code)
{
	switch (err_code) {
		case BTRFS_ERROR_DEV_RAID1_MIN_NOT_MET:
			return "unable to go below two devices on raid1";
		case BTRFS_ERROR_DEV_RAID10_MIN_NOT_MET:
			return "unable to go below four devices on raid10";
		case BTRFS_ERROR_DEV_RAID5_MIN_NOT_MET:
			return "unable to go below two devices on raid5";
		case BTRFS_ERROR_DEV_RAID6_MIN_NOT_MET:
			return "unable to go below three devices on raid6";
		case BTRFS_ERROR_DEV_TGT_REPLACE:
			return "unable to remove the dev_replace target dev";
		case BTRFS_ERROR_DEV_MISSING_NOT_FOUND:
			return "no missing devices found to remove";
		case BTRFS_ERROR_DEV_ONLY_WRITABLE:
			return "unable to remove the only writeable device";
		case BTRFS_ERROR_DEV_EXCL_RUN_IN_PROGRESS:
			return "add/delete/balance/replace/resize operation "\
				"in progress";
		default:
			return NULL;
	}
}


#define BTRFS_IOC_SNAP_CREATE _IOW(BTRFS_IOCTL_MAGIC, 1, \
				   struct btrfs_ioctl_vol_args)
#define BTRFS_IOC_DEFRAG _IOW(BTRFS_IOCTL_MAGIC, 2, \
				   struct btrfs_ioctl_vol_args)
#define BTRFS_IOC_RESIZE _IOW(BTRFS_IOCTL_MAGIC, 3, \
				   struct btrfs_ioctl_vol_args)
#define BTRFS_IOC_SCAN_DEV _IOW(BTRFS_IOCTL_MAGIC, 4, \
				   struct btrfs_ioctl_vol_args)
/* trans start and trans end are dangerous, and only for
 * use by applications that know how to avoid the
 * resulting deadlocks
 */
#define BTRFS_IOC_TRANS_START  _IO(BTRFS_IOCTL_MAGIC, 6)
#define BTRFS_IOC_TRANS_END    _IO(BTRFS_IOCTL_MAGIC, 7)
#define BTRFS_IOC_SYNC         _IO(BTRFS_IOCTL_MAGIC, 8)

#define BTRFS_IOC_CLONE        _IOW(BTRFS_IOCTL_MAGIC, 9, int)
#define BTRFS_IOC_ADD_DEV _IOW(BTRFS_IOCTL_MAGIC, 10, \
				   struct btrfs_ioctl_vol_args)
#define BTRFS_IOC_RM_DEV _IOW(BTRFS_IOCTL_MAGIC, 11, \
				   struct btrfs_ioctl_vol_args)
#define BTRFS_IOC_BALANCE _IOW(BTRFS_IOCTL_MAGIC, 12, \
				   struct btrfs_ioctl_vol_args)

#define BTRFS_IOC_CLONE_RANGE _IOW(BTRFS_IOCTL_MAGIC, 13, \
				  struct btrfs_ioctl_clone_range_args)

#define BTRFS_IOC_SUBVOL_CREATE _IOW(BTRFS_IOCTL_MAGIC, 14, \
				   struct btrfs_ioctl_vol_args)
#define BTRFS_IOC_SNAP_DESTROY _IOW(BTRFS_IOCTL_MAGIC, 15, \
				struct btrfs_ioctl_vol_args)
#define BTRFS_IOC_DEFRAG_RANGE _IOW(BTRFS_IOCTL_MAGIC, 16, \
				struct btrfs_ioctl_defrag_range_args)
#define BTRFS_IOC_TREE_SEARCH _IOWR(BTRFS_IOCTL_MAGIC, 17, \
				   struct btrfs_ioctl_search_args)
#define BTRFS_IOC_TREE_SEARCH_V2 _IOWR(BTRFS_IOCTL_MAGIC, 17, \
					   struct btrfs_ioctl_search_args_v2)
#define BTRFS_IOC_INO_LOOKUP _IOWR(BTRFS_IOCTL_MAGIC, 18, \
				   struct btrfs_ioctl_ino_lookup_args)
#define BTRFS_IOC_DEFAULT_SUBVOL _IOW(BTRFS_IOCTL_MAGIC, 19, u64)
#define BTRFS_IOC_SPACE_INFO _IOWR(BTRFS_IOCTL_MAGIC, 20, \
				    struct btrfs_ioctl_space_args)
#define BTRFS_IOC_START_SYNC _IOR(BTRFS_IOCTL_MAGIC, 24, __u64)
#define BTRFS_IOC_WAIT_SYNC  _IOW(BTRFS_IOCTL_MAGIC, 22, __u64)
#define BTRFS_IOC_SNAP_CREATE_V2 _IOW(BTRFS_IOCTL_MAGIC, 23, \
				   struct btrfs_ioctl_vol_args_v2)
#define BTRFS_IOC_SUBVOL_CREATE_V2 _IOW(BTRFS_IOCTL_MAGIC, 24, \
				   struct btrfs_ioctl_vol_args_v2)
#define BTRFS_IOC_SUBVOL_GETFLAGS _IOR(BTRFS_IOCTL_MAGIC, 25, __u64)
#define BTRFS_IOC_SUBVOL_SETFLAGS _IOW(BTRFS_IOCTL_MAGIC, 26, __u64)
#define BTRFS_IOC_SCRUB _IOWR(BTRFS_IOCTL_MAGIC, 27, \
			      struct btrfs_ioctl_scrub_args)
#define BTRFS_IOC_SCRUB_CANCEL _IO(BTRFS_IOCTL_MAGIC, 28)
#define BTRFS_IOC_SCRUB_PROGRESS _IOWR(BTRFS_IOCTL_MAGIC, 29, \
				       struct btrfs_ioctl_scrub_args)
#define BTRFS_IOC_DEV_INFO _IOWR(BTRFS_IOCTL_MAGIC, 30, \
				 struct btrfs_ioctl_dev_info_args)
#define BTRFS_IOC_FS_INFO _IOR(BTRFS_IOCTL_MAGIC, 31, \
			       struct btrfs_ioctl_fs_info_args)
#define BTRFS_IOC_BALANCE_V2 _IOWR(BTRFS_IOCTL_MAGIC, 32, \
				   struct btrfs_ioctl_balance_args)
#define BTRFS_IOC_BALANCE_CTL _IOW(BTRFS_IOCTL_MAGIC, 33, int)
#define BTRFS_IOC_BALANCE_PROGRESS _IOR(BTRFS_IOCTL_MAGIC, 34, \
					struct btrfs_ioctl_balance_args)
#define BTRFS_IOC_INO_PATHS _IOWR(BTRFS_IOCTL_MAGIC, 35, \
					struct btrfs_ioctl_ino_path_args)
#define BTRFS_IOC_LOGICAL_INO _IOWR(BTRFS_IOCTL_MAGIC, 36, \
					struct btrfs_ioctl_ino_path_args)
#define BTRFS_IOC_SET_RECEIVED_SUBVOL _IOWR(BTRFS_IOCTL_MAGIC, 37, \
				struct btrfs_ioctl_received_subvol_args)
#define BTRFS_IOC_SEND _IOW(BTRFS_IOCTL_MAGIC, 38, struct btrfs_ioctl_send_args)
#define BTRFS_IOC_DEVICES_READY _IOR(BTRFS_IOCTL_MAGIC, 39, \
				     struct btrfs_ioctl_vol_args)
#define BTRFS_IOC_QUOTA_CTL _IOWR(BTRFS_IOCTL_MAGIC, 40, \
			       struct btrfs_ioctl_quota_ctl_args)
#define BTRFS_IOC_QGROUP_ASSIGN _IOW(BTRFS_IOCTL_MAGIC, 41, \
			       struct btrfs_ioctl_qgroup_assign_args)
#define BTRFS_IOC_QGROUP_CREATE _IOW(BTRFS_IOCTL_MAGIC, 42, \
			       struct btrfs_ioctl_qgroup_create_args)
#define BTRFS_IOC_QGROUP_LIMIT _IOR(BTRFS_IOCTL_MAGIC, 43, \
			       struct btrfs_ioctl_qgroup_limit_args)
#define BTRFS_IOC_QUOTA_RESCAN _IOW(BTRFS_IOCTL_MAGIC, 44, \
			       struct btrfs_ioctl_quota_rescan_args)
#define BTRFS_IOC_QUOTA_RESCAN_STATUS _IOR(BTRFS_IOCTL_MAGIC, 45, \
			       struct btrfs_ioctl_quota_rescan_args)
#define BTRFS_IOC_QUOTA_RESCAN_WAIT _IO(BTRFS_IOCTL_MAGIC, 46)
#define BTRFS_IOC_GET_FSLABEL _IOR(BTRFS_IOCTL_MAGIC, 49, \
				   char[BTRFS_LABEL_SIZE])
#define BTRFS_IOC_SET_FSLABEL _IOW(BTRFS_IOCTL_MAGIC, 50, \
				   char[BTRFS_LABEL_SIZE])
#define BTRFS_IOC_GET_DEV_STATS _IOWR(BTRFS_IOCTL_MAGIC, 52, \
				      struct btrfs_ioctl_get_dev_stats)
#define BTRFS_IOC_DEV_REPLACE _IOWR(BTRFS_IOCTL_MAGIC, 53, \
				    struct btrfs_ioctl_dev_replace_args)
#define BTRFS_IOC_FILE_EXTENT_SAME _IOWR(BTRFS_IOCTL_MAGIC, 54, \
					 struct btrfs_ioctl_same_args)
#define BTRFS_IOC_GET_FEATURES _IOR(BTRFS_IOCTL_MAGIC, 57, \
				   struct btrfs_ioctl_feature_flags)
#define BTRFS_IOC_SET_FEATURES _IOW(BTRFS_IOCTL_MAGIC, 57, \
				   struct btrfs_ioctl_feature_flags[2])
#define BTRFS_IOC_GET_SUPPORTED_FEATURES _IOR(BTRFS_IOCTL_MAGIC, 57, \
				   struct btrfs_ioctl_feature_flags[3])

#ifdef CONFIG_SYNO_BTRFS_FREE_SPACE_ANALYZE
#define BTRFS_IOC_FREE_SPACE_ANALYZE	_IOWR(BTRFS_IOCTL_MAGIC, 240, \
				   struct btrfs_ioctl_free_space_analyze_args)
#endif /* CONFIG_SYNO_BTRFS_FREE_SPACE_ANALYZE */
#ifdef CONFIG_SYNO_BTRFS_METADATA_RESERVE
#define BTRFS_IOC_SYNO_PUNCH_CHECK	_IOWR(BTRFS_IOCTL_MAGIC, 243, \
				   struct btrfs_ioctl_syno_punch_check_args)
#endif /* CONFIG_SYNO_BTRFS_METADATA_RESERVE */
#ifdef CONFIG_SYNO_BTRFS_LOG_TREE_RSV_METADATA
#define BTRFS_IOC_SYNO_RESERVE_LOG_TREE_BLOCK_GROUP _IOWR(BTRFS_IOCTL_MAGIC, 244, \
				   struct btrfs_ioctl_log_tree_reserve_bg_args)
#endif /* CONFIG_SYNO_BTRFS_LOG_TREE_RSV_METADATA */
#ifdef CONFIG_SYNO_BTRFS_CLONE_RANGE_V2
#define BTRFS_IOC_SYNO_CLONE_RANGE_V2 _IOWR(BTRFS_IOCTL_MAGIC, 245, \
				    struct btrfs_ioctl_syno_clone_range_args_v2)
#endif /* CONFIG_SYNO_BTRFS_CLONE_RANGE */
#ifdef CONFIG_SYNO_BTRFS_IOC_SYNC_SYNO
#define BTRFS_IOC_SYNC_SYNO	_IO(BTRFS_IOCTL_MAGIC, 246)
#endif /* CONFIG_SYNO_BTRFS_IOC_SYNC_SYNO */

#ifdef CONFIG_SYNO_BTRFS_SNAPSHOT_SIZE_CALCULATION
#define BTRFS_IOC_SNAPSHOT_SIZE_QUERY _IOWR(BTRFS_IOCTL_MAGIC, 247, \
				   struct btrfs_ioctl_snapshot_size_query_args)
#endif /* CONFIG_SYNO_BTRFS_SNAPSHOT_SIZE_CALCULATION */

#ifdef CONFIG_SYNO_BTRFS_COMPR_CTL
#define BTRFS_IOC_COMPR_CTL _IOR(BTRFS_IOCTL_MAGIC, 248, \
									struct btrfs_ioctl_compr_ctl_args)
#endif /* CONFIG_SYNO_BTRFS_COMPR_CTL */

#ifdef CONFIG_SYNO_BTRFS_SEND
#define BTRFS_IOC_SUBVOL_GETINFO _IOR(BTRFS_IOCTL_MAGIC, 249, \
				   struct btrfs_ioctl_subvol_info_args)
#endif /* CONFIG_SYNO_BTRFS_SEND */

#ifdef CONFIG_SYNO_BTRFS_QGROUP_QUERY
#define BTRFS_IOC_QGROUP_QUERY _IOR(BTRFS_IOCTL_MAGIC, 253, \
                                    struct btrfs_ioctl_qgroup_query_args)
#endif /* CONFIG_SYNO_BTRFS_QGROUP_QUERY */

#endif /* _LINUX_BTRFS_H */
