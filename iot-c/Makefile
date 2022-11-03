include $(TOPDIR)/rules.mk

PKG_NAME:=iot-c
PKG_RELEASE:=1
PKG_VERSION:=1.0.0

PKG_SOURCE_PROTO:=git
PKG_SOURCE_URL:=https://github.com/ibm-watson-iot/iot-c.git
PKG_SOURCE_VERSION:=f921dce616af815da2236954c608a2f377c928c5

PKG_BUILD_DEPENDS:=openssl

include $(INCLUDE_DIR)/package.mk

define Package/iot-c
	SECTION:=libs
	CATEGORY:=Libraries
	TITLE:=iot-c
	URL:=https://github.com/ibm-watson-iot/iot-c
	DEPENDS:=+libopenssl
endef

define Package/iot-c/description
	IBM Watson IoT Platform (IoTP) C Client SDK
endef

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/include
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/paho.mqtt.c/src/*.h $(1)/usr/include
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/src/wiotp/sdk/*.h $(1)/usr/include

	$(INSTALL_DIR) $(1)/usr/lib

	$(INSTALL_BIN) $(PKG_BUILD_DIR)/paho.mqtt.c/build/output/libpaho-mqtt3a.so.1.3 $(1)/usr/lib
	$(LN) libpaho-mqtt3a.so.1.3 $(1)/usr/lib/libpaho-mqtt3a.so
	$(LN) libpaho-mqtt3a.so.1.3 $(1)/usr/lib/libpaho-mqtt3a.so.1

	$(INSTALL_BIN) $(PKG_BUILD_DIR)/paho.mqtt.c/build/output/libpaho-mqtt3as.so.1.3 $(1)/usr/lib
	$(LN) libpaho-mqtt3as.so.1.3 $(1)/usr/lib/libpaho-mqtt3as.so
	$(LN) libpaho-mqtt3as.so.1.3 $(1)/usr/lib/libpaho-mqtt3as.so.1

	$(INSTALL_BIN) $(PKG_BUILD_DIR)/paho.mqtt.c/build/output/libpaho-mqtt3c.so.1.3 $(1)/usr/lib
	$(LN) libpaho-mqtt3c.so.1.3 $(1)/usr/lib/libpaho-mqtt3c.so
	$(LN) libpaho-mqtt3c.so.1.3 $(1)/usr/lib/libpaho-mqtt3c.so.1

	$(INSTALL_BIN) $(PKG_BUILD_DIR)/paho.mqtt.c/build/output/libpaho-mqtt3cs.so.1.3 $(1)/usr/lib
	$(LN) libpaho-mqtt3cs.so.1.3 $(1)/usr/lib/libpaho-mqtt3cs.so
	$(LN) libpaho-mqtt3cs.so.1.3 $(1)/usr/lib/libpaho-mqtt3cs.so.1

	$(INSTALL_BIN) $(PKG_BUILD_DIR)/build/libiotp-as-device.so.1.0 $(1)/usr/lib
	$(LN) libiotp-as-device.so.1.0 $(1)/usr/lib/libiotp-as-device.so
	$(LN) libiotp-as-device.so.1.0 $(1)/usr/lib/libiotp-as-device.so.1

	$(INSTALL_BIN) $(PKG_BUILD_DIR)/build/libiotp-as-application.so.1.0 $(1)/usr/lib
	$(LN) libiotp-as-application.so.1.0 $(1)/usr/lib/libiotp-as-application.so
	$(LN) libiotp-as-application.so.1.0 $(1)/usr/lib/libiotp-as-application.so.1

	$(INSTALL_BIN) $(PKG_BUILD_DIR)/build/libiotp-as-gateway.so.1.0 $(1)/usr/lib
	$(LN) libiotp-as-gateway.so.1.0 $(1)/usr/lib/libiotp-as-gateway.so
	$(LN) libiotp-as-gateway.so.1.0 $(1)/usr/lib/libiotp-as-gateway.so.1

	$(INSTALL_BIN) $(PKG_BUILD_DIR)/build/libiotp-as-managedDevice.so.1.0 $(1)/usr/lib
	$(LN) libiotp-as-managedDevice.so.1.0 $(1)/usr/lib/libiotp-as-managedDevice.so
	$(LN) libiotp-as-managedDevice.so.1.0 $(1)/usr/lib/libiotp-as-managedDevice.so.1

	$(INSTALL_BIN) $(PKG_BUILD_DIR)/build/libiotp-as-managedGateway.so.1.0 $(1)/usr/lib
	$(LN) libiotp-as-managedGateway.so.1.0 $(1)/usr/lib/libiotp-as-managedGateway.so
	$(LN) libiotp-as-managedGateway.so.1.0 $(1)/usr/lib/libiotp-as-managedGateway.so.1
endef

define Package/iot-c/install
	$(INSTALL_DIR) $(1)/usr/lib

	$(INSTALL_BIN) $(PKG_BUILD_DIR)/paho.mqtt.c/build/output/libpaho-mqtt3a.so.1.3 $(1)/usr/lib
	$(LN) libpaho-mqtt3a.so.1.3 $(1)/usr/lib/libpaho-mqtt3a.so
	$(LN) libpaho-mqtt3a.so.1.3 $(1)/usr/lib/libpaho-mqtt3a.so.1

	$(INSTALL_BIN) $(PKG_BUILD_DIR)/paho.mqtt.c/build/output/libpaho-mqtt3as.so.1.3 $(1)/usr/lib
	$(LN) libpaho-mqtt3as.so.1.3 $(1)/usr/lib/libpaho-mqtt3as.so
	$(LN) libpaho-mqtt3as.so.1.3 $(1)/usr/lib/libpaho-mqtt3as.so.1

	$(INSTALL_BIN) $(PKG_BUILD_DIR)/paho.mqtt.c/build/output/libpaho-mqtt3c.so.1.3 $(1)/usr/lib
	$(LN) libpaho-mqtt3c.so.1.3 $(1)/usr/lib/libpaho-mqtt3c.so
	$(LN) libpaho-mqtt3c.so.1.3 $(1)/usr/lib/libpaho-mqtt3c.so.1

	$(INSTALL_BIN) $(PKG_BUILD_DIR)/paho.mqtt.c/build/output/libpaho-mqtt3cs.so.1.3 $(1)/usr/lib
	$(LN) libpaho-mqtt3cs.so.1.3 $(1)/usr/lib/libpaho-mqtt3cs.so
	$(LN) libpaho-mqtt3cs.so.1.3 $(1)/usr/lib/libpaho-mqtt3cs.so.1

	$(INSTALL_BIN) $(PKG_BUILD_DIR)/build/libiotp-as-device.so.1.0 $(1)/usr/lib
	$(LN) libiotp-as-device.so.1.0 $(1)/usr/lib/libiotp-as-device.so
	$(LN) libiotp-as-device.so.1.0 $(1)/usr/lib/libiotp-as-device.so.1

	$(INSTALL_BIN) $(PKG_BUILD_DIR)/build/libiotp-as-application.so.1.0 $(1)/usr/lib
	$(LN) libiotp-as-application.so.1.0 $(1)/usr/lib/libiotp-as-application.so
	$(LN) libiotp-as-application.so.1.0 $(1)/usr/lib/libiotp-as-application.so.1

	$(INSTALL_BIN) $(PKG_BUILD_DIR)/build/libiotp-as-gateway.so.1.0 $(1)/usr/lib
	$(LN) libiotp-as-gateway.so.1.0 $(1)/usr/lib/libiotp-as-gateway.so
	$(LN) libiotp-as-gateway.so.1.0 $(1)/usr/lib/libiotp-as-gateway.so.1

	$(INSTALL_BIN) $(PKG_BUILD_DIR)/build/libiotp-as-managedDevice.so.1.0 $(1)/usr/lib
	$(LN) libiotp-as-managedDevice.so.1.0 $(1)/usr/lib/libiotp-as-managedDevice.so
	$(LN) libiotp-as-managedDevice.so.1.0 $(1)/usr/lib/libiotp-as-managedDevice.so.1

	$(INSTALL_BIN) $(PKG_BUILD_DIR)/build/libiotp-as-managedGateway.so.1.0 $(1)/usr/lib
	$(LN) libiotp-as-managedGateway.so.1.0 $(1)/usr/lib/libiotp-as-managedGateway.so
	$(LN) libiotp-as-managedGateway.so.1.0 $(1)/usr/lib/libiotp-as-managedGateway.so.1
endef

$(eval $(call BuildPackage,iot-c))
