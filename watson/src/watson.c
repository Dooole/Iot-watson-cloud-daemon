#include <stdio.h>
#include <signal.h>
#include <memory.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <syslog.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

#include <libubox/blobmsg_json.h>
#include <libubus.h>
#include <iotp_device.h>

#define PID_FILE_PATH "/var/run/watsond.pid"

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

struct watson_context {
    struct ubus_context *ubus;
    struct blob_attr *memory[MEMORY_MAX];
    IoTPConfig *config;
    IoTPDevice *device;
    int interrupt;
    char *path;
};

static struct watson_context ctx;

static void board_cb(struct ubus_request *req, int type, struct blob_attr *msg) {
    struct blob_buf *buf = (struct blob_buf *)req->priv;
    struct blob_attr *tb[INFO_MAX];

    struct blobmsg_policy info_policy[INFO_MAX] = {
        [MEMORY_DATA] = {.name = "memory", .type = BLOBMSG_TYPE_TABLE},
    };

    blobmsg_parse(info_policy, INFO_MAX, tb, blob_data(msg), blob_len(msg));
    if (!tb[MEMORY_DATA]) {
        syslog(LOG_ERR, "No memory data received\n");
        return;
    }

    struct blobmsg_policy memory_policy[MEMORY_MAX] = {
        [TOTAL_MEMORY] = {.name = "total", .type = BLOBMSG_TYPE_INT64},
        [FREE_MEMORY] = {.name = "free", .type = BLOBMSG_TYPE_INT64},
        [SHARED_MEMORY] = {.name = "shared", .type = BLOBMSG_TYPE_INT64},
        [BUFFERED_MEMORY] = {.name = "buffered", .type = BLOBMSG_TYPE_INT64},
    };

    blobmsg_parse(memory_policy, MEMORY_MAX, ctx.memory,
        blobmsg_data(tb[MEMORY_DATA]), blobmsg_data_len(tb[MEMORY_DATA]));
}

static void usage(void)
{
    fprintf(stderr, "usage: watsond [-h] <-c configuration>\n"
        "\t-h --help\t show this message\n"
        "\t-c --config\t specify config file\n"
        "\t-f --foreground\t do not daemonize\n");
}

static void signal_handler(int signo) {
    syslog(LOG_WARNING, "Received signal: %d, stopping\n", signo);
    ctx.interrupt = 1;
}

static void cleanup(void)
{
    if (ctx.ubus) {
        ubus_free(ctx.ubus);
        ctx.ubus = NULL;
    }

    if (ctx.device) {
        IoTPDevice_disconnect(ctx.device);
        IoTPDevice_destroy(ctx.device);
        ctx.device = NULL;
    }

    if (ctx.config) {
        IoTPConfig_clear(ctx.config);
        ctx.config = NULL;
    }

    closelog();
    unlink(PID_FILE_PATH);
}

static int init(void)
{
    int rc;

    setlogmask (LOG_UPTO (LOG_NOTICE));
    openlog("watsond", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);
    syslog(LOG_INFO, "Watson started...\n");

    ctx.ubus = ubus_connect(NULL);
    if (!ctx.ubus) {
        syslog(LOG_ERR, "Failed to connect to ubus\n");
        return -1;
    }

    rc = IoTPConfig_setLogHandler(IoTPLog_FileDescriptor, stdout);
    if (rc) {
        syslog(LOG_ERR, "Failed to set IoTP Client log handler: %d\n", rc);
        return -1;
    }

    rc = IoTPConfig_create(&ctx.config, ctx.path);
    if (rc) {
        syslog(LOG_ERR, "Failed to initialize configuration: %d\n", rc);
        return -1;
    }

    rc = IoTPDevice_create(&ctx.device, ctx.config);
    if (rc) {
        syslog(LOG_ERR, "Failed to configure IoTP device: %d\n", rc);
        return -1;
    }

    rc = IoTPDevice_connect(ctx.device);
    if (rc) {
        syslog(LOG_ERR, "Failed to connect to Watson: %s (%d)\n",
            IOTPRC_toString(rc), rc);
        return -1;
    }

    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);

    return 0;
}

static int daemonize(void)
{
    char pidstr[256];
    pid_t process_id = 0;
    pid_t sid = 0;
    int fd;

    process_id = fork();

    if (process_id < 0) {
        syslog(LOG_ERR, "Fork failed!\n");
        return -1;
    }

    if (process_id > 0)
        exit(0);

    umask(0);

    sid = setsid();
    if(sid < 0) {
        exit(1);
    }

    chdir("/");

    close(STDIN_FILENO);

    fd = open(PID_FILE_PATH, O_RDWR|O_CREAT, 0640);
    if (fd < 0) {
        syslog(LOG_ERR, "Failed to open pid file\n");
        return -1;
    }

    if (lockf(fd, F_TLOCK, 0) < 0) {
        syslog(LOG_ERR, "Failed to lock pid file, already running?\n");
        return -1;
    }

    sprintf(pidstr, "%d\n", getpid());

    write(fd, pidstr, strlen(pidstr));

    fd = open("/dev/null", O_RDWR);
    if (fd < 0) {
        syslog(LOG_ERR, "Failed to open /dev/null\n");
        return -1;
    }
    dup2(fd, STDOUT_FILENO);
    dup2(fd, STDERR_FILENO);

    return 0;
}

int main(int argc, char *argv[])
{
    int ubus_id, opt_id, opt, foreground = 0;
    char data[128];

    struct option opts[] = {
        {"help", no_argument, NULL, 'h'},
        {"config", required_argument, NULL, 'c'},
        {"foreground", no_argument, NULL, 'f'},
        {NULL, 0, NULL, 0}
    };

    memset(&ctx, 0, sizeof(struct watson_context));

    while (1) {
        if ((opt = getopt_long(argc, argv,
                "hc:", opts, &opt_id)) == -1)
            break;

        switch (opt)
        {
            case 'c':
                ctx.path = optarg;
                break;
            case 'f':
                foreground = 1;
                break;
            default:
                usage();
                exit(EXIT_FAILURE);
        }
    }

    if (argc < 2) {
        usage();
        exit(EXIT_FAILURE);
    }

    if (!foreground && daemonize()) {
        syslog(LOG_ERR, "Failed to daemonize watson\n");
        exit(EXIT_FAILURE);
    }

    if (init()) {
        syslog(LOG_ERR, "Failed to initialize watson\n");
        exit(EXIT_FAILURE);
    }

    while (!ctx.interrupt) {
        if (ubus_lookup_id(ctx.ubus, "system", &ubus_id) ||
            ubus_invoke(ctx.ubus, ubus_id, "info", NULL, board_cb, NULL, 3000)) {
                syslog(LOG_ERR, "cannot request memory info from procd\n");
        }

        sprintf(data, "{\"d\" : {\"freeMEM\": %llu, \"totalMEM\": %llu}}",
            blobmsg_get_u64(ctx.memory[FREE_MEMORY])),
            blobmsg_get_u64(ctx.memory[TOTAL_MEMORY]);

        IoTPDevice_sendEvent(ctx.device, "status", data, "json", QoS0, NULL);
        sleep(5);
    }

    cleanup();
    exit(EXIT_SUCCESS);
}
