# elinuxCore  最小哪吒STU D1 嵌入式Linux系统

## 配置Host开发环境



## 设置交叉编译工具链

```ba
export PATH=$PATH:/home/book/eLinuxCore_dongshannezhastu/toolchain/riscv64-glibc-gcc-thead_20200702/bin
export ARCH=rscv
export CROSS_COMPILE=riscv64-unknown-linux-gnu-
```



## 编译 bootloader



### 编译 boot0 spl



### 编译opensbi

```bash
book@100ask:~/eLinuxCore_dongshannezhastu/opensbi$ 
```



### 编译 uboot

```bash
book@100ask:~/eLinuxCore_dongshannezhastu/u-boot$ make sun20iw1p1_defconfig
book@100ask:~/eLinuxCore_dongshannezhastu/u-boot$ make -j8
book@100ask:~/eLinuxCore_dongshannezhastu/u-boot$ cp u-boot-sun20iw1p1.bin ../
```



## 编译kernel和设备树

### 编译内核镜像

```
book@100ask:~/eLinuxCore_dongshannezhastu/linux$ make sun20iw1p1_d1_defconfig
book@100ask:~/eLinuxCore_dongshannezhastu/linux$ make  -j8
book@100ask:~/eLinuxCore_dongshannezhastu/linux$ cp arch/riscv/boot/Image ../
```

* 打包生成特定格式

```bash
#buildroot kernel boot images.
mkbootimg --kernel  Image  --ramdisk  ramdisk.img --board  d1-nezha_min --base  0x40200000 --kernel_offset  0x0 --ramdisk_offset  0x01000000 -o  boot.img
```

### 编译设备树文件

```bash
book@100ask:~/eLinuxCore_dongshannezhastu/linux$ make dtbs
  DTC     arch/riscv/boot/dts/board.dtb
  
book@100ask:~/eLinuxCore_dongshannezhastu/linux$ cp  arch/riscv/boot/dts/board.dtb ../
```

### 编译内核模块

```bash
book@virtual-machine:~/eLinuxCore_dongshannezhastu/linux$ make modules -j8
```



## 分阶段打包

### 打包boot_package.fex



dragonsecboot  -pack boot_package.cfg

```bash
[package]
;item=Item_TOC_name,         Item_filename,
;item=scp,                    scp.fex
item=opensbi,                 fw_jump.bin
item=u-boot,                u-boot-sun20iw1p1.bin
item=dtb,                    board.dtb
;item=logo,                   bootlogo.bmp.lzma
;item=shutdowncharge,         bempty.bmp.lzma
;item=androidcharge,          battery_charge.bmp.lzma

```



### 生成 env.fex

* TF卡镜像 启动环境变量

```bash
#kernel command arguments
console=ttyS0,115200
root_partition=rootfs
boot_partition=boot
keybox_list=widevine,ec_key,ec_cert1,ec_cert2,ec_cert3,rsa_key,rsa_cert1,rsa_cert2,rsa_cert3
#set kernel cmdline if boot.img or recovery.img has no cmdline we will use this
bootargs=earlyprintk=sunxi-uart,0x02500000 clk_ignore_unused initcall_debug=0 console=ttyS0,115200 loglevel=8 root=/dev/mmcblk0p4  init=/sbin/init partitions=ext4 cma=8M  gpt=1
#uboot system env config
bootdelay=1
kernel=boot.img
mmc_boot_part=3
mmc_dev=0
boot_check=sunxi_card0_probe;mmcinfo;mmc part
boot_mmc=fatload mmc ${mmc_dev}:${mmc_boot_part} 45000000 ${kernel}; bootm 45000000
bootcmd=run boot_check boot_mmc

```


mkenvimage -r -p 0x00 -s 0x20000 -o env.fex env-sd.cfg

* spi nand启动环境变量

```bash


```

mkenvimage -r -p 0x00 -s 0x20000 -o env.fex env-spinand.cfg



## 编译busybox最小文件系统



## 完整打包生成 TF卡 sdcard.img系统镜像并烧写

### 打包系统镜像

genimage

```bash
image sdcard.img {
        hdimage{
                partition-table-type = "hybrid"
                gpt-location = 1M
        }
        partition boot0 {
                in-partition-table = "no"
                image = "boot0_sdcard.fex"
                offset = 8K
        }
        partition boot-packages {
                in-partition-table = "no"
                image = "boot_package.fex"
                offset = 16400K
        }
        partition env {
                image = "env.fex"
                size = 128k
        }
        partition env-redund {
                image = "env.fex"
                size = 128k
        }
        partition boot {
                partition-type = 0xC
                bootable = "true"
                image = "boot.vfat"
        }
        partition rootfs {
                partition-type = 0x83
                image = "rootfs.ext4"
        }
}

image boot.vfat {
        vfat {
        files = {
                "boot.img",
                "Image",
                "board.dtb"
        }
        }
        size = 32M
}
```



### 烧写生成的系统镜像并启动

wind32diskimage



## 完整打包生成SPI NAND系统镜像 并烧写

### 打包Spi NAND系统镜像



### 烧写生成的系统镜像 并启动





