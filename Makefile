# Target specific macros
TARGET = main_OSEK
TARGET_SOURCES = \
	main.c \
	utility_sound/utility_sound.c \
	utility_lcd/utility_lcd.c

TOPPERS_OSEK_OIL_SOURCE = ./main.oil

# Don't modify below part
O_PATH ?= build
include ../ecrobot/ecrobot.mak
