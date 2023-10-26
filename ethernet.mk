# SPDX-License-Identifier: MIT

ifneq ($(ETHERNET_ROOT),)
	eth-basedir := $(ETHERNET_ROOT)/
endif

ETHERNET_SRCS := \
$(eth-basedir)src/qca.c \
$(eth-basedir)src/homeplug.c \
$(eth-basedir)src/mme.c \
$(eth-basedir)src/eth.c \

ETHERNET_INCS := $(eth-basedir)include
