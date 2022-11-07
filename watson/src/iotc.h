#ifndef IOTC_H
#define IOTC_H

#include <iotp_device.h>

struct iotc_config {
    char *orgid;
    char *typeid;
    char *deviceid;
    char *token;
    char *port;
};

struct iotc_handler {
	IoTPConfig *config;
    IoTPDevice *device;
};

int iotc_init(struct iotc_handler *ih, struct iotc_config *cfg);
int iotc_send(struct iotc_handler *ih, char *msg);
void iotc_destroy(struct iotc_handler *ih);

#endif
