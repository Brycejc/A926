################################################################################
#
# wifi_common
#
################################################################################
WIFI_COMMON_VERSION = 1.0

WIFI_COMMON_SITE_METHOD = local
WIFI_COMMON_SITE = ingenic/wifi/wifi_common

WIFI_COMMON_INSTALL_STAGING = YES
WIFI_COMMON_INSTALL_TARGET = YES
WIFI_COMMON_DEPENDENCIES = host-pkgconf

WIFIMAC_ADDR_PATH := $(BR2_PACKAGE_WIFIMAC_ADDR_PATH)

define WIFI_COMMON_INSTALL_TARGET_CMDS
	$(TARGET_MAKE_ENV) $(MAKE) -C $(@D) SRCDIR=$(TOPDIR)/$(WIFI_COMMON_OVERRIDE_SRCDIR) \
		DESTDIR=$(TARGET_DIR) install \
		WIFIMAC_ADDR_PATH=$(WIFIMAC_ADDR_PATH) \
		BCM43362_A2=$(BR2_PACKAGE_WIFI_COMMON_BCM43362_A2) \
		BCM43438_A1=$(BR2_PACKAGE_WIFI_COMMON_BCM43438_A1) \
		BCM43438_A0=$(BR2_PACKAGE_WIFI_COMMON_BCM43438_A0) \
		BCM4345=$(BR2_PACKAGE_WIFI_COMMON_BCM4345)	\
		RTL8723DS=$(BR2_PACKAGE_WIFI_COMMON_RTL8723DS)
endef

$(eval $(generic-package))