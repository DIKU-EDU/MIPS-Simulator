// Simple ELF file parser.
//
// Copyright (c) 2016 DIKU.
// Copenhagen Liberal License - v0.6 - January 25, 2016.
//
// Authors (most recent contributors first):
//  Oleksandr Shturmov <oleks@oleks.info>

#ifndef DiMS_ELF_H
#define DiMS_ELF_H

#include <stddef.h> // size_t
#include <stdint.h> // uint32_t
#include "mem.h"

#define ELF_ERROR_IO_ERROR        (-1)
#define ELF_ERROR_OUT_OF_MEM      (-2)
#define ELF_ERROR_NOT_ELF         (-3)
#define ELF_ERROR_NOT_SUPPORTED   (-4)

int
elf_dump(const char *path, uint32_t *entry,
  mmu_t *mem);

#endif // DiMS_ELF_H
