##===- tools/driver/Makefile -------------------------------*- Makefile -*-===##
#
#                     The LLVM Compiler Infrastructure
#
# This file is distributed under the University of Illinois Open Source
# License. See LICENSE.TXT for details.
#
##===----------------------------------------------------------------------===##
LLDB_LEVEL := ../..

TOOLNAME = lldb

NO_PEDANTIC = 1

LLVMLibsOptions += -ledit -llldb -llldbUtility

include $(LLDB_LEVEL)/Makefile

ifeq ($(HOST_OS),Darwin)
	LLVMLibsOptions += -Wl,-rpath,@loader_path/../lib/
	LLVMLibsOptions += -Wl,-sectcreate -Wl,__TEXT -Wl,__info_plist -Wl,"$(PROJ_SRC_DIR)/lldb-Info.plist"
endif

ifeq ($(HOST_OS),Linux)
	LLVMLibsOptions += -Wl,-rpath,$(LibDir)
endif

ifeq ($(HOST_OS),FreeBSD)
	CPP.Flags += -I/usr/include/edit #-v
	LLVMLibsOptions += -Wl,-rpath,$(LibDir)
endif

