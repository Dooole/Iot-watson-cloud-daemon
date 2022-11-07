#ifndef UBUS_H
#define UBUS_H

#include <libubus.h>
#include <libubox/blobmsg_json.h>

enum {
    TOTAL_MEMORY,
    FREE_MEMORY,
    SHARED_MEMORY,
    BUFFERED_MEMORY,
    MEMORY_MAX,
};

enum {
    MEMORY_DATA,
    INFO_MAX,
};

struct ubus_handler {
    struct ubus_context *ubus;
    struct blob_attr *memory[MEMORY_MAX];
    struct blobmsg_policy info_policy[INFO_MAX];
    struct blobmsg_policy memory_policy[MEMORY_MAX];
};

int ubus_init(struct ubus_handler *uh);
int ubus_update(struct ubus_handler *uh);
unsigned long long ubus_mem_total(struct ubus_handler *uh);
unsigned long long ubus_mem_free(struct ubus_handler *uh);
void ubus_destroy(struct ubus_handler *uh);

#endif
