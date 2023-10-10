# SPDX-License-Identifier: MIT

COMPONENT_NAME = QCA

SRC_FILES = \
	../src/qca.c \
	../src/mme.c \
	../src/eth.c \

TEST_SRC_FILES = \
	src/qca_test.cpp \
	src/test_all.cpp \

INCLUDE_DIRS = \
	$(CPPUTEST_HOME)/include \
	../include \

MOCKS_SRC_DIRS =
CPPUTEST_CPPFLAGS =

include runners/MakefileRunner
