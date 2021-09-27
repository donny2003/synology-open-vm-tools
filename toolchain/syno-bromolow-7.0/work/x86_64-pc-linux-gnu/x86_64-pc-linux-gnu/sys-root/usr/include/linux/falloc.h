#ifndef _FALLOC_H_
#define _FALLOC_H_

#define FALLOC_FL_KEEP_SIZE	0x01 /* default is extend size */
#define FALLOC_FL_PUNCH_HOLE	0x02 /* de-allocates range */
#define FALLOC_FL_NO_HIDE_STALE	0x04 /* reserved codepoint */

#ifdef CONFIG_SYNO_BTRFS_FALLOCATE_MARK_WRITTEN
/*
 * FALLOC_FL_MARK_WRITTEN is used to mark the newly allocated range of
 * file as written instead of prealloc. Without this flag, each fallocated
 * area will be marked as prealloc, and changed to written status when
 * first write occured within that range. This update requires metadata
 * update. Since we don't initialize the fallocated range and the
 * status is already marked as written, we'll read garbage data until
 * we first write those blocks.
 */
#define FALLOC_FL_MARK_WRITTEN		0x1000
#endif /* CONFIG_SYNO_BTRFS_FALLOCATE_MARK_WRITTEN */

#endif /* _FALLOC_H_ */
