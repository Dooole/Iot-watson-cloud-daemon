#include <stdio.h>
#include <signal.h>
#include <memory.h>
#include <stdlib.h>
#include <unistd.h>
#include <syslog.h>
#include <argp.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

#include "ubus.h"
#include "iotc.h"

volatile static int interrupted = 0;

static error_t parse_opt(int key, char *arg, struct argp_state *state) {
    
    struct iotc_config *iotcfg = state->input;

    switch (key)
    {
        case 'o': 
            iotcfg->orgid = arg;
            break;
        case 't':
            iotcfg->typeid = arg;
            break;
        case 'd':
            iotcfg->deviceid = arg;
            break;
        case 'a':
            iotcfg->token = arg;
            break;
        case 'p':
            iotcfg->port = arg;
            break;
        default:
            return ARGP_ERR_UNKNOWN;
    }
    return 0;
}

static void signal_handler(int signo) {
    syslog(LOG_WARNING, "Received signal: %d, stopping\n", signo);
    interrupted = 1;
}

int main(int argc, char *argv[])
{
    struct ubus_handler uh;
    struct iotc_handler ih;
    struct iotc_config iotcfg;
    char data[128];

    struct argp_option options[] = 
    {
        {"orgid",    'o', "orgid",    0, "Organisation ID"},
        {"typeid",   't', "typeid",   0, "Type ID"},
        {"deviceid", 'd', "deviceid", 0, "Device ID"},
        {"token",    'a', "token",    0, "Authentication token"},
        {"port",     'p', "port",     0, "Port number"},
        {0}
    };
    struct argp argp = {options, parse_opt};

    if (argp_parse(&argp, argc, argv, 0, 0, &iotcfg)) {
        fprintf(stderr, "Failed to parse command line arguments\n");
        exit(EXIT_FAILURE);
    }

    setlogmask(LOG_UPTO(LOG_NOTICE));
    openlog("watsond", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);

    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);

    if (iotc_init(&ih, &iotcfg)) {
        fprintf(stderr, "Failed to initialize iotc\n");
        exit(EXIT_FAILURE);
    }

    if (ubus_init(&uh)) {
        fprintf(stderr, "Failed to initialize ubus\n");
        exit(EXIT_FAILURE);
    }

    while (!interrupted) {
        if (!ubus_update(&uh)) {
            sprintf(data, "{\"d\" : {\"freeMEM\": %llu, \"totalMEM\": %llu}}",
                ubus_mem_free(&uh),
                ubus_mem_total(&uh));

            if (iotc_send(&ih, data))
                syslog(LOG_ERR, "Failed to send iotc message\n");
        } else {
            syslog(LOG_ERR, "Failed to update ubus\n");
        }

        sleep(5);
    }

    closelog();

    iotc_destroy(&ih);
    ubus_destroy(&uh);

    exit(EXIT_SUCCESS);
}
