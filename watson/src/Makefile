INSTALL_DIR ?= /usr/sbin/

CC ?= gcc
INSTALL_BIN ?= install -m 0755
RM ?= rm -f

CFLAGS += -std=c99 -Wall -D_DEFAULT_SOURCE -D_GNU_SOURCE -I/usr/local/include
LDFLAGS += -L/usr/local/lib

#Add Watson iot-c libraries
LDFLAGS += -liotp-as-application \
-liotp-as-device \
-liotp-as-gateway \
-liotp-as-managedDevice \
-liotp-as-managedGateway \
-lubus \
-lubox \
-largp

SRC := $(wildcard *.c)
OBJ := $(SRC:.c=.o)

TARGET := watsond

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $^ $(LDFLAGS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(TARGET) $(OBJ)

install:
	$(INSTALL_BIN) $(TARGET) $(INSTALL_DIR)
