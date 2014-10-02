# Target specific macros
TARGET = main_OSEK
TARGET_SOURCES = \
    main.c \
    boot_programs/boot_programs.c \
    boot_programs/color_measurement/color_measurement.c \
    boot_programs/distance_measurement/distance_measurement.c \
    utility/utility_lcd/utility_lcd.c \
    utility/utility_sound/utility_sound.c \
	utility/utility_movement/utility_distance.c \

TOPPERS_OSEK_OIL_SOURCE = ./main.oil

# Don't modify below part
O_PATH ?= build
include ../ecrobot/ecrobot.mak
