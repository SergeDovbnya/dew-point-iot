CONTIKI_PROJECT = test-dht22

CONTIKI_TARGET_SOURCEFILES += dht22.c

all: $(CONTIKI_PROJECT)

PLATFORMS_ONLY = zoul

CONTIKI = ../..

include $(CONTIKI)/Makefile.dir-variables
MODULES += $(CONTIKI_NG_DRIVERS_SENSOR_DIR)/bme280

include $(CONTIKI)/Makefile.include
