# SPDX-License-Identifier: MIT

COMPONENT_NAME = HomePlug

SRC_FILES = \
	../src/homeplug.c \
	../src/mme.c \
	../src/eth.c \

TEST_SRC_FILES = \
	src/homeplug_test.cpp \
	src/test_all.cpp \

INCLUDE_DIRS = \
	$(CPPUTEST_HOME)/include \
	../include \

MOCKS_SRC_DIRS =
CPPUTEST_CPPFLAGS =

include runners/MakefileRunner
