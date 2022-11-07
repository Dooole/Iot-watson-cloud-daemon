#include <string.h>

#include "ubus.h"

static void board_cb(struct ubus_request *req, int type, struct blob_attr *msg) {
    struct ubus_handler *uh = (struct ubus_handler *)req->priv;
    struct blob_attr *tb[INFO_MAX];

    blobmsg_parse(uh->info_policy, INFO_MAX, tb, blob_data(msg), blob_len(msg));
    if (!tb[MEMORY_DATA])
        return;

    blobmsg_parse(uh->memory_policy, MEMORY_MAX, uh->memory,
        blobmsg_data(tb[MEMORY_DATA]), blobmsg_data_len(tb[MEMORY_DATA]));
}

int ubus_init(struct ubus_handler *uh)
{
    memset(uh, 0, sizeof(struct ubus_handler));

    uh->ubus = ubus_connect(NULL);
    if (!uh->ubus)
        return -1;

    uh->info_policy[MEMORY_DATA].type = BLOBMSG_TYPE_TABLE;
    uh->info_policy[MEMORY_DATA].name = "memory";

    uh->memory_policy[TOTAL_MEMORY].type = BLOBMSG_TYPE_INT64;
    uh->memory_policy[TOTAL_MEMORY].name = "total";

    uh->memory_policy[FREE_MEMORY].type = BLOBMSG_TYPE_INT64;
    uh->memory_policy[FREE_MEMORY].name = "free";

    uh->memory_policy[SHARED_MEMORY].type = BLOBMSG_TYPE_INT64;
    uh->memory_policy[SHARED_MEMORY].name = "shared";

    uh->memory_policy[BUFFERED_MEMORY].type = BLOBMSG_TYPE_INT64;
    uh->memory_policy[BUFFERED_MEMORY].name = "buffered";

    return 0;
}

int ubus_update(struct ubus_handler *uh)
{
    uint32_t ubus_id;

    if (ubus_lookup_id(uh->ubus, "system", &ubus_id))
        return -1;

    if (ubus_invoke(uh->ubus, ubus_id, "info", NULL, board_cb, uh, 3000))
        return -1;

    return 0;
}

unsigned long long ubus_mem_total(struct ubus_handler *uh)
{
    return blobmsg_get_u64(uh->memory[TOTAL_MEMORY]);
}

unsigned long long ubus_mem_free(struct ubus_handler *uh)
{
    return blobmsg_get_u64(uh->memory[FREE_MEMORY]);
}

void ubus_destroy(struct ubus_handler *uh)
{
    if (uh) {
        if (uh->ubus)
            ubus_free(uh->ubus);

        memset(uh, 0, sizeof(struct ubus_handler));
    }
}
