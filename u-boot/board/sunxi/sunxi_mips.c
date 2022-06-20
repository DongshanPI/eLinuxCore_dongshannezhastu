/*
 * (C) Copyright 2018-2020
 * Allwinner Technology Co., Ltd. <www.allwinnertech.com>
 * ouyangkun <ouyangkun@allwinnertech.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */
#include <common.h>
#include <asm/io.h>
#include <smc.h>
#include <fdt_support.h>
#include <sys_partition.h>
#include <memalign.h>
#include <sunxi_image_verifier.h>

#define DEBUG_PRINT 0

enum MIPS_DATA_TYPE_t { MIPS_CODE, MIPS_DATABASE, MIPS_XML };
const char *mips_names[] = { [MIPS_CODE]     = "mips_code",
			     [MIPS_DATABASE] = "mips_database",
			     [MIPS_XML]	     = "mips_xml" };
struct mips_data_info_t {
	char *file_name;
	uint32_t file_len;
	char *cert_name;
	uint32_t cert_len;
	uint32_t load_addr;
	uint8_t flag;
};
extern int do_sunxi_flash(cmd_tbl_t *cmdtp, int flag, int argc,
			  char *const argv[]);

static int load_mips_data(uint32_t *run_addr, uint8_t *cert_tmp)
{
#define FILE_NAME_NOT_FOUNT (1 << 0)
#define CERT_NAME_NOT_FOUNT (1 << 1)
#define LOAD_ADDR_NOT_FOUNT (1 << 2)
	int i, node;
	char node_name[64];
	char addr_tmp[32];
	char cert_addr[32];
	struct mips_data_info_t mips_data_info[ARRAY_SIZE(mips_names)];

	char part_info[16] = { 0 };
	int partno	   = -1;
	char *read_argv[6];
	/*prepare params for file fsload*/
	partno = sunxi_partition_get_partno_byname("mips"); /*android*/
	if (partno < 0) {
		partno = sunxi_partition_get_partno_byname("mips");
		if (partno < 0) {
			pr_err("Get mips partition number fail!\n");
			return -1;
		}
	}
	snprintf(part_info, 16, "0:%x", partno);
	read_argv[0] = "fatload";
	read_argv[1] = "sunxi_flash";
	read_argv[2] = part_info;
	read_argv[5] = NULL;

	/*prepare buffer for cert verify*/
	if (sunxi_get_secureboard()) {
		if (!cert_tmp) {
			pr_err("no memory for cert tmp\n");
			return -1;
		} else {
			sprintf(cert_addr, "%x", (uint32_t)cert_tmp);
#if DEBUG_PRINT == 1
			pr_err(" cert_addr	:%s\n", cert_addr);
#endif
		}
	}

	memset(&mips_data_info, 0, sizeof(mips_data_info));
	for (i = 0; i < ARRAY_SIZE(mips_names); i++) {
		/*load setting from fdt*/
		sprintf(node_name, "/firmware/mips/%s", mips_names[i]);
		node = fdt_path_offset(working_fdt, node_name);
		if (fdt_getprop_string(working_fdt, node, "file_name",
				       &mips_data_info[i].file_name) < 0)
			mips_data_info[i].flag |= FILE_NAME_NOT_FOUNT;
		if (fdt_getprop_string(working_fdt, node, "cert_name",
				       &mips_data_info[i].cert_name) < 0)
			mips_data_info[i].flag |= CERT_NAME_NOT_FOUNT;
		if (fdt_getprop_u32(working_fdt, node, "load_addr",
				    &mips_data_info[i].load_addr) < 0)
			mips_data_info[i].flag |= LOAD_ADDR_NOT_FOUNT;
		else
			sprintf(addr_tmp, "%x", mips_data_info[i].load_addr);

#if DEBUG_PRINT == 1
		pr_err(" file_name:%s\n", mips_data_info[i].file_name);
		pr_err(" cert_name:%s\n", mips_data_info[i].cert_name);
		pr_err(" load_addr:%x\n", mips_data_info[i].load_addr);
		pr_err(" flag		:%x\n", mips_data_info[i].flag);
#endif
		if (mips_data_info[i].flag &
		    (FILE_NAME_NOT_FOUNT | LOAD_ADDR_NOT_FOUNT)) {
			pr_err("no info for %s\n", mips_names[i]);
			return -1;
		}
		/*load file*/
		read_argv[3] = addr_tmp;
		read_argv[4] = mips_data_info[i].file_name;
		if (do_fat_fsload(0, 0, 5, read_argv)) {
			pr_err("unable to open %s\n", read_argv[4]);

			return -1;
		}
		mips_data_info[i].file_len =
			simple_strtoul(env_get("filesize"), NULL, 16);
#if DEBUG_PRINT == 1
		pr_err(" file_len	:%x\n", mips_data_info[i].file_len);
#endif

		/*load cert and verify, xml do not need verify*/
		if (sunxi_get_secureboard() && i != MIPS_XML) {
			if (mips_data_info[i].flag & CERT_NAME_NOT_FOUNT)
				pr_err("no cert for %s\n", mips_names[i]);
			read_argv[3] = cert_addr;
			read_argv[4] = mips_data_info[i].cert_name;
			if (do_fat_fsload(0, 0, 5, read_argv)) {
				pr_err("unable to open %s\n", read_argv[4]);

				return -1;
			}

			mips_data_info[i].cert_len =
				simple_strtoul(env_get("filesize"), NULL, 16);
#if DEBUG_PRINT == 1
			pr_err(" cert_len	:%x\n",
			       mips_data_info[i].cert_len);
#endif
			if (sunxi_verify_mips(
				    (uint8_t *)mips_data_info[i].load_addr,
				    mips_data_info[i].file_len, cert_tmp,
				    mips_data_info[i].cert_len)) {
				pr_err("verify %s failed\n",
				       mips_data_info[i].file_name);
				return -1;
			}
		}
	}

	*run_addr = mips_data_info[MIPS_CODE].load_addr;
	return 0;
}

int do_boot_mips(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
	uint8_t *cert_tmp = NULL;
	uint32_t img_addr;
	int ret = -1;
	if (sunxi_get_secureboard()) {
		cert_tmp = memalign(CACHE_LINE_SIZE, 8192);
		if (!cert_tmp) {
			pr_err("no memory for cert tmp\n");
			return -1;
		}
	}

	if (strcmp("start", argv[1]) == 0) {
		if (load_mips_data(&img_addr, cert_tmp) == 0) {
			uint32_t mips_only_size, shm_size;
#if defined(FPGA_PLATFORM)
			/*init uart gpio*/
			writel(0x00000200, SUNXI_PIO_BASE + 0x34);
#endif
			int node;
			/*read memory layout setting*/
			mips_only_size = 0;
			shm_size       = 0;
			node	       = fdt_path_offset(working_fdt,
						 "/firmware/mips/mips_memory");
			if (node >= 0) {
				fdt_getprop_u32(working_fdt, node,
						"mips_only_size",
						&mips_only_size);
				fdt_getprop_u32(working_fdt, node, "shm_size",
						&shm_size);
#if DEBUG_PRINT == 1
				pr_err("mips_only_size %x\n", mips_only_size);
				pr_err("shm_size %x\n", shm_size);
#endif
			}
			if (!mips_only_size && !shm_size) {
				pr_err("no info for mips memory\n");
				ret = -1;
			} else {
				smc_tee_setup_mips(img_addr, mips_only_size);
				ret = fdt_add_mem_rsv(working_fdt, img_addr,
						      mips_only_size +
							      shm_size);
				if (ret) {
					pr_err("##add mem rsv error: %s : %s\n",
					       __func__, fdt_strerror(ret));
				}
			}
		}
	}
	if (cert_tmp)
		free(cert_tmp);
	return ret;
}

static char boot_mips_help[] = "start - boot mips application\n";

U_BOOT_CMD(sunxi_mips, CONFIG_SYS_MAXARGS, 1, do_boot_mips,
	   "boot application image from memory", boot_mips_help);
