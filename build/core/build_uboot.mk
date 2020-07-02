# Makefile define rules which make uboot.
#	Vars used by this Makefile,
#
#	UBOOT_BUILD_CONFIG: # define uboot config files
#	UBOOT_TARGET_FILE:  # eg:u-boot-with-spl.bin ?
#	UBOOT_PATH:   # e:$(TOPDIR)/uboot ??
#
#	support make:
#	make $(LOCALMODULE) # eg: make uboot
#	make $(LOCALMODULE)-clean # eg: make uboot-clean
#
#


# TODO: add ENV check and warning

ifeq ($(strip $(UBOOT_BUILD_CONFIG)),)
$(error *** unspecified UBOOT_BUILD_CONFIG: $(UBOOT_BUILD_CONFIG). ***)
endif

ifeq ($(strip $(UBOOT_TARGET_FILE)),)
$(error **** unspecified UBOOT_TARGET_FILE: $(UBOOT_TARGET_FILE). ***)
endif

BUILD_UBOOT_TARGET_$(LOCAL_MODULE):=$(OUT_IMAGE_DIR)/$(LOCAL_MODULE)
BUILD_UBOOT_INTERMEDIATE:=$(TOP_DIR)/$(OUT_DEVICE_OBJ_DIR)/$(LOCAL_MODULE)-intermediate
BUILD_UBOOT_INTERMEDIATE_OBJ:=$(BUILD_UBOOT_INTERMEDIATE)/$(UBOOT_TARGET_FILE)

MAKE_JLEVEL ?= 1
UBOOT_PATH  ?= $(TOP_DIR)/u-boot

#$(BUILD_UBOOT_TARGET_$(LOCAL_MODULE)):BUILD_UBOOT_INTERMEDIATE_OBJ:=$(BUILD_UBOOT_INTERMEDIATE_OBJ)
$(BUILD_UBOOT_TARGET_$(LOCAL_MODULE)):$(BUILD_UBOOT_INTERMEDIATE_OBJ)
	cp -f $^ $@


ifeq ($(UBOOT_PATH)/Build.mk, $(wildcard $(UBOOT_PATH)/Build.mk))
$(error ======== u-boot/Build.mk found, remove it before going ========)
endif

# localize VARS.
$(BUILD_UBOOT_INTERMEDIATE_OBJ):UBOOT_PATH:=$(UBOOT_PATH)
$(BUILD_UBOOT_INTERMEDIATE_OBJ):BUILD_UBOOT_INTERMEDIATE:=$(BUILD_UBOOT_INTERMEDIATE)
$(BUILD_UBOOT_INTERMEDIATE_OBJ):UBOOT_BUILD_CONFIG:=$(UBOOT_BUILD_CONFIG)

$(BUILD_UBOOT_INTERMEDIATE_OBJ):
	make -C $(UBOOT_PATH) O=$(BUILD_UBOOT_INTERMEDIATE) $(UBOOT_BUILD_CONFIG) -j$(MAKE_JLEVEL)


LOCAL_MODULE_BUILD=$(LOCAL_MODULE)
include $(BUILD_SYSTEM)/module_install.mk
ifneq ($(LOCAL_FILTER_MODULE),)
ALL_MODULES += $(LOCAL_FILTER_MODULE)
ALL_BUILD_MODULES += $(BUILD_UBOOT_TARGET_$(LOCAL_MODULE))
endif

$(LOCAL_MODULE):$(BUILD_UBOOT_TARGET_$(LOCAL_MODULE))


$(LOCAL_MODULE)-clean:BBB0:=$(BUILD_UBOOT_TARGET_$(LOCAL_MODULE))
$(LOCAL_MODULE)-clean:BBB1:=$(BUILD_UBOOT_INTERMEDIATE)
$(LOCAL_MODULE)-clean:
	rm -rf $(BBB0)
	rm -rf $(BBB1)

.PHONY: $(BUILD_UBOOT_INTERMEDIATE_OBJ)

UBOOT_PATH:=
BUILD_UBOOT_INTERMEDIATE:=
UBOOT_BUILD_CONFIG:=