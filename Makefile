# Target specific macros
TARGET = main_OSEK
TARGET_SOURCES = \
	main.c \
	utility_sound/utility_sound.c \
	utility_lcd/utility_lcd.c \
	boot_programs/boot_programs.c \
	boot_programs/color_scanning.c \
	boot_programs/sonar_sensor.c \

TOPPERS_OSEK_OIL_SOURCE = ./main.oil

# Don't modify below part
O_PATH ?= build
include ../ecrobot/ecrobot.mak
