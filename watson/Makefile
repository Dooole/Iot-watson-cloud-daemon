include $(TOPDIR)/rules.mk

PKG_NAME:=watson
PKG_RELEASE:=1
PKG_VERSION:=1.0.0

include $(INCLUDE_DIR)/package.mk

define Package/watson
	CATEGORY:=Base system
	TITLE:=watson
	DEPENDS:=+iot-c +libubus +libubox +libblobmsg-json
endef

define Package/watson/description
	Daeomon to send data to IBM Watson IoT Platform (IoTP) cloud
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)
endef

define Package/watson/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_DIR) $(1)/etc/config
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/watsond $(1)/usr/sbin/
	$(INSTALL_BIN) ./files/watson.init $(1)/etc/init.d/watson
	$(INSTALL_CONF) ./files/watson.config $(1)/etc/config/watson
endef

$(eval $(call BuildPackage,watson))
