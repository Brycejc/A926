# Makefile define rulles which make kernel.
#	Vars we need from local module:
#
#    KERNEL_BUILD_CONFIG: 	# mandatory. which %_defconfig files to configure kernel.
#    KERNEL_IMAGE_PATH:  	# mandatory. default arch/mips/boot, PATH store target image, relative to kernel path.
#    KERNEL_TARGET_IMAGE: 	# mandatory. default uImage, kernel target image name, eg: uImage, xImage.
#    KERNEL_PATH:	     	# optional.  default $(TOP_DIR)/kernel
#    KERNEL_CONFIG_PATH:  	# optional.  default arch/mips/configs, can be defined anywhere.
#


# TODO: add Vars check.
ifeq ($(KERNEL_BUILD_CONFIG),)
$(error **** KERNEL_BUILD_CONFIG $(KERNEL_BUILD_CONFIG) must be defined! ****)
endif

ifeq ($(KERNEL_TARGET_IMAGE),)
$(warning ***** KERNEL_TARGET_IMAGE $(KERNEL_TARGET_IMAGE) default to 'uImage' ****)
endif

ifeq ($(findstring -,$(LOCAL_MODULE)),-)
$(error **** find '-' in $(LOCAL_PATH)/$(LOCAL_MODULE), "+-*/" is not allowed in LOCAL_MODULE)
endif

KERNEL_PATH ?=$(TOP_DIR)/kernel
KERNEL_IMAGE_PATH ?=arch/mips/boot/
KERNEL_TARGET_IMAGE ?=uImage
# Default values

ifeq ($(KERNEL_PATH)/Build.mk, $(wildcard $(KERNEL_PATH)/Build.mk))
$(error ======== $(KERNEL_PATH)/Build.mk found, remove it before going ========)
endif

# each LOCAL_MODULE defines a target.
BUILD_KERNEL_TARGET_$(LOCAL_MODULE) :=$(OUT_IMAGE_DIR)/$(LOCAL_MODULE)


BUILD_KERNEL_INTERMEDIATE:=$(TOP_DIR)/$(OUT_DEVICE_OBJ_DIR)/$(LOCAL_MODULE)-intermediate
BUILD_KERNEL_INTERMEDIATE_DOT_CONFIG:=$(BUILD_KERNEL_INTERMEDIATE)/.config
BUILD_KERNEL_INTERMEDIATE_OBJ:=$(BUILD_KERNEL_INTERMEDIATE)/$(KERNEL_IMAGE_PATH)/$(KERNEL_TARGET_IMAGE)

# if KERNEL_CONFIG_PATH has value, rename defconfig. else use config under kernel src.
ifneq ($(KERNEL_CONFIG_PATH),)
KERNEL_DEFCONFIG:=ingenic_build_$(TARGET_BOARD_PLATFORM)_$(TARGET_PRODUCT_BOARD)_$(LOCAL_MODULE)_defconfig
$(shell cp $(KERNEL_CONFIG_PATH)/$(KERNEL_BUILD_CONFIG) $(KERNEL_PATH)/arch/mips/configs/$(KERNEL_DEFCONFIG))
else
KERNEL_DEFCONFIG:=$(KERNEL_BUILD_CONFIG)
endif

#$(BUILD_KERNEL_TARGET_$(LOCAL_MODULE)):BUILD_KERNEL_INTERMEDIATE_OBJ:=$(BUILD_KERNEL_INTERMEDIATE_OBJ)
$(BUILD_KERNEL_TARGET_$(LOCAL_MODULE)):$(BUILD_KERNEL_INTERMEDIATE_OBJ)
	cp -f $^ $@

# localize vars for this rules.
$(BUILD_KERNEL_INTERMEDIATE_OBJ):KERNEL_PATH:=$(KERNEL_PATH)
$(BUILD_KERNEL_INTERMEDIATE_OBJ):BUILD_KERNEL_INTERMEDIATE:=$(BUILD_KERNEL_INTERMEDIATE)
$(BUILD_KERNEL_INTERMEDIATE_OBJ):KERNEL_IMAGE_PATH:=$(KERNEL_IMAGE_PATH)
$(BUILD_KERNEL_INTERMEDIATE_OBJ):KERNEL_TARGET_IMAGE:=$(KERNEL_TARGET_IMAGE)
$(BUILD_KERNEL_INTERMEDIATE_OBJ):KERNEL_DEFCONFIG:=$(KERNEL_DEFCONFIG)
$(BUILD_KERNEL_INTERMEDIATE_OBJ):$(BUILD_KERNEL_INTERMEDIATE_DOT_CONFIG)
	make -C $(KERNEL_PATH) O=$(BUILD_KERNEL_INTERMEDIATE) $(KERNEL_TARGET_IMAGE) -j$(MAKE_JLEVEL)

$(BUILD_KERNEL_INTERMEDIATE_DOT_CONFIG):BUILD_KERNEL_INTERMEDIATE:=$(BUILD_KERNEL_INTERMEDIATE)

$(BUILD_KERNEL_INTERMEDIATE_DOT_CONFIG):
	make -C $(KERNEL_PATH) O=$(BUILD_KERNEL_INTERMEDIATE) $(KERNEL_DEFCONFIG)

LOCAL_MODULE_BUILD=$(LOCAL_MODULE)
include $(BUILD_SYSTEM)/module_install.mk
ifneq ($(LOCAL_FILTER_MODULE),)
ALL_MODULES += $(LOCAL_FILTER_MODULE)
ALL_BUILD_MODULES += $(BUILD_KERNEL_TARGET_$(LOCAL_MODULE))
endif


$(LOCAL_MODULE):$(BUILD_KERNEL_TARGET_$(LOCAL_MODULE))

$(LOCAL_MODULE)-clean:BBB0:=$(BUILD_KERNEL_TARGET_$(LOCAL_MODULE))
$(LOCAL_MODULE)-clean:BBB1:=$(BUILD_KERNEL_INTERMEDIATE)

$(LOCAL_MODULE)-clean:
	rm -rf $(BBB0)
	rm -rf $(BBB1)


# localize VARIABLES
$(LOCAL_MODULE)-%:KERNEL_PATH:=$(KERNEL_PATH)
$(LOCAL_MODULE)-%:BUILD_KERNEL_INTERMEDIATE:=$(BUILD_KERNEL_INTERMEDIATE)
$(LOCAL_MODULE)-%:LOCAL_MODULE:=$(LOCAL_MODULE)
$(LOCAL_MODULE)-%:
	echo $@
	make -C $(KERNEL_PATH) O=$(BUILD_KERNEL_INTERMEDIATE) $(subst $(LOCAL_MODULE)-,,$@)

.PHONY: $(BUILD_KERNEL_INTERMEDIATE_OBJ)

# clear VARS
KERNEL_PATH :=
KERNEL_IMAGE_PATH :=
KERNEL_TARGET_IMAGE :=
KERNEL_CONFIG_PATH :=
BUILD_KERNEL_INTERMEDIATE :=
BUILD_KERNEL_INTERMEDIATE_OBJ:=