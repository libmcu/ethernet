# SPDX-License-Identifier: MIT

ifneq ($(ETHERNET_ROOT),)
	eth-basedir := $(ETHERNET_ROOT)/
endif

ETHERNET_SRCS := \
$(eth-basedir)src/common.c \
$(eth-basedir)src/parser.c \
$(eth-basedir)src/decoder.c \
$(eth-basedir)src/encoder.c \
$(eth-basedir)src/helper.c \
$(eth-basedir)src/ieee754.c \

ETHERNET_INCS := $(eth-basedir)include
