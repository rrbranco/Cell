DIRS := spu
CFLAGS := -g
LDFLAGS += -Wl,-Map,bsdaemon.map
PROGRAM_ppu := debug_ppu
PROGRAM_spu = debug_spu
IMPORTS := spu/debug_spu.a -lspe
include $(TOP)/make.footer
