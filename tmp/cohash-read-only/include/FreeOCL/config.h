/*
	FreeOCL - a free OpenCL implementation for CPU
	Copyright (C) 2011  Roland Brochard

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>
*/
#ifndef __FREEOCL_CONFIG_H__
#define __FREEOCL_CONFIG_H__

#define FREEOCL_CXX_COMPILER	"/usr/bin/c++"

#define FREEOCL_CXX_FLAGS_COMMON " " \
        " -shared -fpic -pipe" /* -Wall*/ \
		" -I./include" \
		" -g -O3" \
        " -fomit-frame-pointer -ftree-vectorize -funroll-loops" \
        " -fno-math-errno -fno-trapping-math" \
        " -rdynamic" \
        " -fno-exceptions -fno-rtti" \
		" -x c++ " \
        " -Wall -Wextra "\
        " -Wno-non-pod-varargs"

#if defined (FREEOCL_ARCH_i386)
#define FREEOCL_CXX_FLAGS FREEOCL_CXX_FLAGS_COMMON \
		" -m32 -march=native -mtune=native"
#elif defined (FREEOCL_ARCH_amd64)
#define FREEOCL_CXX_FLAGS FREEOCL_CXX_FLAGS_COMMON \
		" -m64 -march=native -mtune=native"
#elif defined (FREEOCL_ARCH_alpha)
#elif defined (FREEOCL_ARCH_arm)
#elif defined (FREEOCL_ARCH_mips)
#elif defined (FREEOCL_ARCH_ppc64)
#elif defined (FREEOCL_ARCH_ppc)
#elif defined (FREEOCL_ARCH_sparc)
#elif defined (FREEOCL_ARCH_parisc)
#elif defined (FREEOCL_ARCH_s390)
#elif defined (FREEOCL_ARCH_sh)
#endif

#ifndef FREEOCL_CXX_FLAGS
#define FREEOCL_CXX_FLAGS FREEOCL_CXX_FLAGS_COMMON
#endif
		
#define FREEOCL_VERSION_STRING "0.3.6"

#define FREEOCL_VERSION_HI 0
#define FREEOCL_VERSION_LO 3
#define FREEOCL_VERSION_PATCH 6
#define FREEOCL_VENDOR "FreeOCL developers"

/* #undef FREEOCL_RUN_GDB_ON_CRASH */

#endif
