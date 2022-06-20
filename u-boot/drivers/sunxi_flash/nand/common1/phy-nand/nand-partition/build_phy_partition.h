/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __BUILD_PHY_PARTITION_H__
#define __BUILD_PHY_PARTITION_H__

#include "phy.h"

uint64 bitmap_change(unsigned short SectBitmap);
void wait_all_rb(void);
int page_read(unsigned short nDieNum, unsigned short nBlkNum, unsigned short nPage, unsigned short SectBitmap, void *pBuf, void *pSpare);
int page_write(unsigned short nDieNum, unsigned short nBlkNum, unsigned short nPage, unsigned short SectBitmap, void *pBuf, void *pSpare);
int block_erase(unsigned short nDieNum, unsigned short nBlkNum);
int BlockCheck(unsigned short nDieNum, unsigned short nBlkNum);
int is_factory_bad_block(struct _nand_info *nand_info, unsigned short nDieNum, unsigned short nBlkNum);
int is_new_bad_block(struct _nand_info *nand_info, unsigned short nDieNum, unsigned short nBlkNum);
int add_new_bad_block(struct _nand_info *nand_info, unsigned short nDieNum, unsigned short nBlkNum);
int put_factory_bad_block(struct _nand_info *nand_info, struct _nand_phy_partition *phy_partition);
struct _nand_phy_partition *build_phy_partition(struct _nand_info *nand_info, uint32 logic_size, uint16 flag);
int free_phy_partition(struct _nand_phy_partition *phy_partition);
void print_phy_partition(struct _nand_phy_partition *phy_partition);
void print_all_bad_block(struct _nand_phy_partition *phy_partition);
struct _nand_phy_partition *get_next_phy_partition(struct _nand_phy_partition *phy_partition);
struct _nand_disk *get_disk_from_phy_partition(struct _nand_phy_partition *phy_partition);
uint16 get_partitionNO(struct _nand_phy_partition *phy_partition);
uint32 get_phy_partition_num(struct _nand_info *nand_info);
struct _nand_phy_partition *get_phy_partition_from_num(struct _nand_info *nand_info, uint32 num);
int erase_phy_partition(struct _nand_info *nand_info, struct _nand_phy_partition *phy_partition);
int erase_last_phy_partition(struct _nand_info *nand_info);

#endif /*BUILD_PHY_PARTITION_H*/
