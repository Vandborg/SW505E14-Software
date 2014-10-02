# Target specific macros
TARGET = main_OSEK
TARGET_SOURCES = \
    main.c \
    boot_programs/boot_programs.c \
    boot_programs/color_sensor_measurement/color_sensor_measurement.c \
    boot_programs/sonar_sensor.c \
    utility/utility_lcd/utility_lcd.c \
    utility/utility_sound/utility_sound.c \

TOPPERS_OSEK_OIL_SOURCE = ./main.oil

# Don't modify below part
O_PATH ?= build
include ../ecrobot/ecrobot.mak
