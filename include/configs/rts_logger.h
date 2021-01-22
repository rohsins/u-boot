/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright 2014-2015 Freescale Semiconductor, Inc.
 * Copyright Rohit Singh
 * Configuration settings for the RTS LOGGER board.
 */

#ifndef __CONFIG_H
#define __CONFIG_H

#include "mx6_common.h"

#include "imx6_spl.h"

/* Size of malloc() pool */
#define CONFIG_SYS_MALLOC_LEN		(3 * SZ_1M)

/* MMC Configuration */
#define CONFIG_SYS_FSL_ESDHC_ADDR	USDHC2_BASE_ADDR

/* Command definition */
#define CONFIG_MXC_UART_BASE		UART1_BASE


#ifdef CONFIG_IMX_BOOTAUX
/* Set to QSPI2 B flash at default */
#define CONFIG_SYS_AUXCORE_BOOTDATA 0x78000000

#define UPDATE_M4_ENV \
	"m4image=m4_firmware.bin\0" \
	"loadm4image=fatload mmc ${mmcdev}:${mmcpart} ${loadaddr} ${m4image}\0" \
	"update_m4_from_sd=" \
		"if sf probe 1:0; then " \
			"if run loadm4image; then " \
				"setexpr fw_sz ${filesize} + 0xffff; " \
				"setexpr fw_sz ${fw_sz} / 0x10000; "	\
				"setexpr fw_sz ${fw_sz} * 0x10000; "	\
				"sf erase 0x0 ${fw_sz}; " \
				"sf write ${loadaddr} 0x0 ${filesize}; " \
			"fi; " \
		"fi\0" \
	"m4boot=sf probe 1:0; bootaux "__stringify(CONFIG_SYS_AUXCORE_BOOTDATA)"\0"
#else
#define UPDATE_M4_ENV ""
#endif

#define CONFIG_EXTRA_ENV_SETTINGS \
	UPDATE_M4_ENV \
	"script=boot.scr\0" \
	"image=zImage\0" \
	"console=ttymxc0\0" \
	"fdt_high=0xffffffff\0" \
	"initrd_high=0xffffffff\0" \
	"fdt_file=dtb/rts-logger.dtb\0" \
	"fdt_addr=0x83000000\0" \
	"loadaddr=0x82000000\0" \
	"boot_fdt=try\0" \
	"ip_dyn=yes\0" \
	"videomode=video=ctfb:x:800,y:480,depth:24,pclk:29850,le:89,ri:164,up:23,lo:10,hs:10,vs:10,sync:0,vmode:0\0" \
	"mmcdev=0\0" \
	"mmcpart=1\0" \
	"finduuid=part uuid mmc ${mmcdev}:2 uuid\0" \
	"mmcargs=setenv bootargs console=${console},${baudrate} " \
		"root=PARTUUID=${uuid} rootwait rw\0" \
	"loadbootscript=" \
		"fatload mmc ${mmcdev}:${mmcpart} ${loadaddr} ${script};\0" \
	"bootscript=echo Running bootscript from mmc ...; " \
		"source\0" \
	"loadimage=fatload mmc ${mmcdev}:${mmcpart} ${loadaddr} ${image}\0" \
	"loadfdt=fatload mmc ${mmcdev}:${mmcpart} ${fdt_addr} ${fdt_file}\0" \
	"mmcboot=echo Booting from mmc ...; " \
		"run finduuid; " \
		"run mmcargs; " \
		"if test ${boot_fdt} = yes || test ${boot_fdt} = try; then " \
			"if run loadfdt; then " \
				"bootz ${loadaddr} - ${fdt_addr}; " \
			"else " \
				"if test ${boot_fdt} = try; then " \
					"bootz; " \
				"else " \
					"echo WARN: Cannot load the DT; " \
				"fi; " \
			"fi; " \
		"else " \
			"bootz; " \
		"fi;\0" \
	"netargs=setenv bootargs console=${console},${baudrate} " \
		"root=/dev/nfs " \
	"ip=dhcp nfsroot=${serverip}:${nfsroot},v3,tcp\0" \
		"netboot=echo Booting from net ...; " \
		"run netargs; " \
		"if test ${ip_dyn} = yes; then " \
			"setenv get_cmd dhcp; " \
		"else " \
			"setenv get_cmd tftp; " \
		"fi; " \
		"${get_cmd} ${image}; " \
		"if test ${boot_fdt} = yes || test ${boot_fdt} = try; then " \
			"if ${get_cmd} ${fdt_addr} ${fdt_file}; then " \
				"bootz ${loadaddr} - ${fdt_addr}; " \
			"else " \
				"if test ${boot_fdt} = try; then " \
					"bootz; " \
				"else " \
					"echo WARN: Cannot load the DT; " \
				"fi; " \
			"fi; " \
		"else " \
			"bootz; " \
		"fi;\0" \
	"findfdt="\
		"if test test $board_rev = Logger ; then " \
			"setenv fdt_file rts-logger.dtb; fi; " \

#define CONFIG_BOOTCOMMAND \
	   "run findfdt; " \
	   "mmc dev ${mmcdev}; if mmc rescan; then " \
		   "if run loadbootscript; then " \
			   "run bootscript; " \
		   "else " \
			   "if run loadimage; then " \
				   "run mmcboot; " \
			   "else run netboot; " \
			   "fi; " \
		   "fi; " \
	   "else run netboot; fi"

// /* Linux only */
// #define CONFIG_EXTRA_ENV_SETTINGS \
// 	"console=ttymxc0,115200\0" \
// 	"fdt_high=0xffffffff\0" \
// 	"initrd_high=0xffffffff\0" \
// 	"fdtfile=undefined\0" \
// 	"fdt_addr=0x83000000\0" \
// 	"fdt_addr_r=0x83000000\0" \
// 	"ip_dyn=yes\0" \
// 	"mmcdev=0\0" \
// 	"mmcrootfstype=ext4\0" \
// 	"findfdt=setenv fdtfile rts-logger.dtb\0" \
// 	"kernel_addr_r=" __stringify(CONFIG_LOADADDR) "\0" \
// 	"pxefile_addr_r=" __stringify(CONFIG_LOADADDR) "\0" \
// 	"ramdisk_addr_r=0x84000000\0" \
// 	"scriptaddr=" __stringify(CONFIG_LOADADDR) "\0" \
// 	BOOTENV

// #define BOOT_TARGET_DEVICES(func) \
// 	func(MMC, mmc, 0) \
// 	func(DHCP, dhcp, na)

// #include <config_distro_bootcmd.h>

/* Miscellaneous configurable options */

/* Physical Memory Map */
#define PHYS_SDRAM			MMDC0_ARB_BASE_ADDR
#define CONFIG_SYS_SDRAM_BASE		PHYS_SDRAM
#define CONFIG_SYS_INIT_RAM_ADDR	IRAM_BASE_ADDR
#define CONFIG_SYS_INIT_RAM_SIZE	IRAM_SIZE

#define CONFIG_SYS_INIT_SP_OFFSET \
	(CONFIG_SYS_INIT_RAM_SIZE - GENERATED_GBL_DATA_SIZE)
#define CONFIG_SYS_INIT_SP_ADDR \
	(CONFIG_SYS_INIT_RAM_ADDR + CONFIG_SYS_INIT_SP_OFFSET)

/* I2C configs */
#define CONFIG_SYS_I2C
#define CONFIG_SYS_I2C_MXC
#define CONFIG_SYS_I2C_MXC_I2C1
#define CONFIG_SYS_I2C_SPEED		100000

/* PMIC */
#define CONFIG_POWER
#define CONFIG_POWER_I2C
#define CONFIG_POWER_PFUZE3000
#define CONFIG_POWER_PFUZE3000_I2C_ADDR	0x08
#define PFUZE3000_I2C_BUS	0

/* Network */
#define CONFIG_FEC_MXC

#define CONFIG_FEC_ENET_DEV 0
#define IMX_FEC_BASE			ENET_BASE_ADDR
#define CONFIG_FEC_MXC_PHYADDR          0x0

#define CONFIG_FEC_XCV_TYPE             RMII
#define CONFIG_ETHPRIME                 "FEC0"

#endif				/* __CONFIG_H */
