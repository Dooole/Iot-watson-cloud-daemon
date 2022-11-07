#include <syslog.h>

#include "iotc.h"

int iotc_init(struct iotc_handler *ih, struct iotc_config *cfg)
{
    int rc;

    memset(ih, 0, sizeof(struct iotc_handler));

    rc = IoTPConfig_setLogHandler(IoTPLog_FileDescriptor, stdout);
    if (rc) {
        syslog(LOG_ERR, "Failed to set IoTP Client log handler: %d\n", rc);
        return -1;
    }

    rc = IoTPConfig_create(&ih->config, NULL);
    if (rc) {
        syslog(LOG_ERR, "Failed to initialize configuration: %d\n", rc);
        return -1;
    }

    rc = IoTPConfig_setProperty(ih->config, IoTPConfig_identity_orgId, cfg->orgid);
    if (rc) {
        syslog(LOG_ERR, "Failed to set organisation ID: %d\n", rc);
        return -1;
    }

    rc = IoTPConfig_setProperty(ih->config, IoTPConfig_identity_typeId, cfg->typeid);
    if (rc) {
        syslog(LOG_ERR, "Failed to set type ID: %d\n", rc);
        return -1;
    }

    rc = IoTPConfig_setProperty(ih->config, IoTPConfig_identity_deviceId, cfg->deviceid);
    if (rc) {
        syslog(LOG_ERR, "Failed to set device ID: %d\n", rc);
        return -1;
    }

    rc = IoTPConfig_setProperty(ih->config, IoTPConfig_auth_token, cfg->token);
    if (rc) {
        syslog(LOG_ERR, "Failed to set token: %d\n", rc);
        return -1;
    }

    rc = IoTPConfig_setProperty(ih->config, IoTPConfig_options_mqtt_port, cfg->port);
    if (rc) {
        syslog(LOG_ERR, "Failed to set port: %d\n", rc);
        return -1;
    }

    syslog(LOG_INFO, "Configuration created succesfully\n");

    rc = IoTPDevice_create(&ih->device, ih->config);
    if (rc) {
        syslog(LOG_ERR, "Failed to create IoTP device: %d\n", rc);
        return -1;
    }

    rc = IoTPDevice_connect(ih->device);
    if (rc) {
        syslog(LOG_ERR, "Failed to connect to Watson: %s (%d)\n",
            IOTPRC_toString(rc), rc);
        return -1;
    }

    return 0;
}

int iotc_send(struct iotc_handler *ih, char *msg)
{
    return IoTPDevice_sendEvent(ih->device, "status", msg, "json", QoS0, NULL);
}

void iotc_destroy(struct iotc_handler *ih)
{
    if (ih) {
        if (ih->device) {
            IoTPDevice_disconnect(ih->device);
            IoTPDevice_destroy(ih->device);
        }
        if (ih->config) {
            IoTPConfig_clear(ih->config);
        }
        memset(ih, 0, sizeof(struct iotc_handler));
    }
}
