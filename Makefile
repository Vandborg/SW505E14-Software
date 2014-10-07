# Target specific macros
TARGET = main_OSEK
TARGET_SOURCES = \
    main.c \
    boot_programs/color_measurement/color_measurement.c \
    boot_programs/distance_measurement/distance_measurement.c \
    boot_programs/fork_reset/fork_reset.c \
    utility/utility_lcd/utility_lcd.c \
    utility/utility_movement/utility_braking.c \
    utility/utility_string/utility_string.c \
    utility/utility_movement/utility_distance.c \
    utility/utility_sound/utility_sound.c \
    utility/utility_speed/utility_speed.c \
    utility/utility_fork/utility_fork.c \
    utility/utility_movement/utility_rotate_degrees.c \
    boot_programs/boot_programs.c \
    utility/utility_movement/utility_drive_straight.c \

TOPPERS_OSEK_OIL_SOURCE = ./main.oil

# Don't modify below part
O_PATH ?= build
include ../ecrobot/ecrobot.mak
