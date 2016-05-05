// Simple ELF file parser.
//
// Copyright (c) 2016 DIKU.
// Copenhagen Liberal License - v0.6 - January 25, 2016.
//
// Authors (most recent contributors first):
//  Oleksandr Shturmov <oleks@oleks.info>

// Written while reading the [ELFspec].
//
// [ELFspec] TIS Committee. Tool Interface Standard (TIS) Executable and
// Linking Format (ELF) Specification. May 1999.  Retrieved from
// https://refspecs.linuxbase.org/elf/elf.pdf on November 19, 2015.
//    Archived by WebCite® at http://www.webcitation.org/6d9VYmYxi.

#include "elf.h"
#include "mips32.h"   // MIPS_RESERVE

#include <stdio.h>
#include <stdint.h>
#include <error.h>    // error
#include <errno.h>
#include <stdlib.h>   // exit
#include <string.h>   // memset

#include "mem.h"
#if 1
/* ------------- */
#undef _ERROR_H
#include "error.h"
/* ------------- */
#endif

#define EI_MAG0   (0)
#define EI_MAG1   (1)
#define EI_MAG2   (2)
#define EI_MAG3   (3)
#define EI_CLASS  (4)
#define EI_DATA   (5)
#define EI_NIDENT (16)

#define ELFCLASS32  (1)

#define ELFDATA2MSB (2)

#define ET_EXEC (2)

#define EM_MIPS (8)

typedef uint16_t Elf32_Half;

typedef uint32_t Elf32_Word;
typedef uint32_t Elf32_Addr;
typedef uint32_t Elf32_Off;

#define ET_EXEC   (2)

struct ehdr {
	unsigned char e_ident[EI_NIDENT];
	Elf32_Half    e_type;
	Elf32_Half    e_machine;
	Elf32_Word    e_version;        // Not used.
	Elf32_Addr    e_entry;
	Elf32_Off     e_phoff;
	Elf32_Off     e_shoff;          // Not used.
	Elf32_Addr    e_flags;
	Elf32_Half    e_ehsize;         // Not used.
	Elf32_Half    e_phentsize;
	Elf32_Half    e_phnum;
	Elf32_Half    e_shentsize;      // Not used.
	Elf32_Half    e_shnum;          // Not used.
	Elf32_Half    e_shstrndx;       // Not used.
};

struct elf_file {
	FILE *stream;
	struct ehdr ehdr;
};

#define PT_NULL           (0x0)
#define PT_LOAD           (0x1)
#define PT_NOTE           (0x4)

#define PT_MIPS_ABIFLAGS  (0x70000003)

struct p_header {
	Elf32_Word    p_type;
	Elf32_Off     p_offset;
	Elf32_Addr    p_vaddr;
	Elf32_Addr    p_paddr;
	Elf32_Word    p_filesz;
	Elf32_Word    p_memsz;
	Elf32_Word    p_flags;          // Not used.
	Elf32_Word    p_align;          // Not used.
};

static inline int
check_magic_number(struct ehdr *ehdr) {
	if (
	    (ehdr->e_ident[EI_MAG0] != 0x7F) ||
	    (ehdr->e_ident[EI_MAG1] != 'E') ||
	    (ehdr->e_ident[EI_MAG2] != 'L') ||
	    (ehdr->e_ident[EI_MAG3] != 'F')) {
		error(0, 0, "this doesn't look like an ELF-file.");
		return ELF_ERROR_NOT_ELF;
	}

	return 0;
}

static inline int
check_elf_class_32(struct ehdr *ehdr) {
	if (ehdr->e_ident[EI_CLASS] != ELFCLASS32) {
		error(0, 0, "only 32-bit ELF is supported.");
		return ELF_ERROR_NOT_SUPPORTED;
	}

	return 0;
}

static inline int
check_data_encoding(struct ehdr *ehdr) {
	if (ehdr->e_ident[EI_DATA] != ELFDATA2MSB) {
		error(0, 0, "only 2's complement, big endian is supported.");
		return ELF_ERROR_NOT_SUPPORTED;
	}

	return 0;
}

static inline int
read_elf_ident(struct elf_file *file) {
	struct ehdr *ehdr = &file->ehdr;
	int retval = 0;

	// It is straight-forward to read the ELF Identification. It is just an array
	// of 16 bytes. So there is no trouble with endianness; that comes later.

	if (fread(&ehdr->e_ident, sizeof(ehdr->e_ident), 1, file->stream) != 1) {
		error(0, errno, "couldn't read ELF Identification");
		return ELF_ERROR_IO_ERROR;
	}

	retval = check_magic_number(ehdr);
	if (retval != 0) return retval;

	retval = check_elf_class_32(ehdr);
	if (retval != 0) return retval;

	retval = check_data_encoding(ehdr);
	if (retval != 0) return retval;

	return 0;
}

// Cross-platform read of 2's complement, big endian, 32-bit half words.
static inline int
read_be_half_words(FILE *stream, uint16_t *dst, size_t n) {
	unsigned char buf[2];
	size_t i;

	for (i = 0; i < n; ++i) {
		if (fread(&buf, sizeof(buf), 1, stream) != 1) {
			error(0, 0, "couldn't read ELF32 half word.");
			return ELF_ERROR_IO_ERROR;
		}

		*(dst++) = (buf[0] << 8) | buf[1];
	}

	return 0;
}

// Cross-platform read of 2's complement, big endian, 32-bit full words.
static inline int
read_be_words(FILE *stream, uint32_t *dst, size_t n) {
	unsigned char buf[4];
	size_t i;

	for (i = 0; i < n; ++i) {
		if (fread(&buf, sizeof(buf), 1, stream) != 1) {
			error(0, 0, "couldn't read ELF32 word.");
			return ELF_ERROR_IO_ERROR;
		}

		*(dst++) = (buf[0] << 24) | (buf[1] << 16) | (buf[2] << 8) | buf[3];
	}

	return 0;
}

static inline int
check_type(struct ehdr *ehdr) {
	if (ehdr->e_type != ET_EXEC) {
		error(0, 0, "only executables are supported.");
		return ELF_ERROR_NOT_SUPPORTED;
	}

	return 0;
}

static inline int
check_machine(struct ehdr *ehdr) {
	if (ehdr->e_machine != EM_MIPS) {
		error(0, 0, "only MIPS RS3000 machine is supported.");
		return ELF_ERROR_NOT_SUPPORTED;
	}

	return 0;
}

static inline int
read_type_and_machine(struct elf_file *file) {
	struct ehdr *ehdr = &file->ehdr;
	int retval = 0;

	retval = read_be_half_words(file->stream, &ehdr->e_type, 2);
	if (retval != 0) {
		error(0, 0, "couldn't read type and machine.");
		return retval;
	}

	retval = check_type(ehdr);
	if (retval != 0) return retval;

	retval = check_machine(ehdr);
	if (retval != 0) return retval;

	return 0;
}

static inline int
read_ehdr(struct elf_file *file) {
	struct ehdr *ehdr = &file->ehdr;
	int retval = 0;

	retval = read_elf_ident(file);
	if (retval != 0) {
		error(0, 0, "couldn't read ELF ident array.");
		return retval;
	}

	retval = read_type_and_machine(file);
	if (retval != 0) {
		error(0, 0, "couldn't read ELF type and machine.");
		return retval;
	}

	retval = read_be_words(file->stream, &ehdr->e_version, 5);
	if (retval != 0) {
		error(0, 0, "couldn't read rest of ELF header.");
		return retval;
	}

	retval = read_be_half_words(file-> stream, &ehdr->e_ehsize, 6);
	if (retval != 0) {
		error(0, 0, "couldn't read rest of ELF header.");
		return retval;
	}

	return retval;
}

// Assumes first 6 words of p_header are set, i.e. p_type, p_offset, p_vaddr,
// p_paddr, p_filesz, p_memsz.
//
// After this function, stream will point to one past the end of the segment.
static inline int
copy_segment(FILE *stream, struct p_header *phdr,
	     memory_t *mem) {

	/* Translate address */
	uint32_t paddr = translate_vaddr(phdr->p_vaddr);
	uint8_t *segmem = translate_paddr(paddr, mem);

	DEBUG("ELF LOADING TO: 0x%08x to %p", phdr->p_vaddr, segmem);

	/*
	 * TODO: Verify size
	if (memsz < offset + phdr->p_memsz) {
		return ELF_ERROR_OUT_OF_MEM;
	}
	*/

	// Zero out the memory before copying segment from file.
	memset(segmem, 0, phdr->p_memsz);

	if (fseek(stream, phdr->p_offset, SEEK_SET) != 0) {
		error(0, 0, "couldn't seek to first byte of program segment.");
		return ELF_ERROR_IO_ERROR;
	}

	if (fread(segmem, phdr->p_filesz, 1, stream) != 1) {
		error(0, 0, "couldn't read program segment");
		return ELF_ERROR_IO_ERROR;
	}

	return 0;
}

// Assumes stream points to the first byte of a program header entry.
// After this function, stream points to one past the end of the associated
// segment. Also, the first 6 words of the p_header will be set.
static inline int
copy_cur_segment_aux(FILE *stream, memory_t *mem) {
	int retval = 0;

	struct p_header phdr;

	if (read_be_words(stream, &phdr.p_type, 6) != 0) {
		error(0, 0, "couldn't read program header entry.");
		return ELF_ERROR_IO_ERROR;
	}


	DEBUG("SECTION P_TYPE: %d", phdr.p_type);

	switch(phdr.p_type) {
	case PT_NULL:
	case PT_NOTE:
	case PT_MIPS_ABIFLAGS:
		// Naïvely skip the above.
		break;
	case PT_LOAD:
		retval = copy_segment(stream, &phdr, mem);
		break;
	default:
		DEBUG("unknown program header entry type 0x%x", phdr.p_type);
		retval = ELF_ERROR_NOT_SUPPORTED;
		break;
	}

	return retval;
}

// After this function, stream points to one past the end of the current program
// header entry. Also, the first 6 words of the p_header will be set.
static inline int
copy_cur_segment(struct elf_file *file, memory_t *mem) {
	struct ehdr *ehdr = &file->ehdr;
	long origin = 0;
	int retval = 0;

	origin = ftell(file->stream);
	if (origin == -1) {
		error(0, errno, "couldn't tell current position in file");
		return ELF_ERROR_IO_ERROR;
	}

	retval = copy_cur_segment_aux(file->stream, mem);

	// Restore stream to next program header entry (if any).
	if (fseek(file->stream, origin + ehdr->e_phentsize, SEEK_SET) != 0) {
		error(0, errno, "couldn't seek past program header entity");
		return ELF_ERROR_IO_ERROR;
	}

	return retval;
}

// After this function, stream point to one past the end of the last program
// header entry.
static inline int
copy_all_segments(struct elf_file *file, memory_t *mem) {
	struct ehdr *ehdr = &file->ehdr;
	size_t i = 0;
	int retval = 0;

	DEBUG("NUMBER OF ELF SEGMENTS TO COPY: %d", ehdr->e_phnum);

	if (fseek(file->stream, ehdr->e_phoff, SEEK_SET) != 0) {
		error(0, errno, "couldn't seek to program header table");
		return ELF_ERROR_IO_ERROR;
	}

	for (i = 0; i < ehdr->e_phnum; ++i) {
		retval = copy_cur_segment(file, mem);
		if (retval != 0) return retval;
	}

	return 0;
}

static inline int
elf_open(struct elf_file *file, const char *path) {
	int retval = 0;

	file->stream = fopen(path, "r");
	if (file->stream == NULL) {
		error(0, errno, "couldn't open file (%s) for reading", path);
		return ELF_ERROR_IO_ERROR;
	}

	retval = read_ehdr(file);
	if (retval != 0) {
		error(0, errno, "couldn't read ELF header");
		return retval;
	}

	return 0;
}

static inline int
elf_close(struct elf_file *file) {
	if (fclose(file->stream) != 0) {
		error(0, errno, "couldn't close opened file");
		return ELF_ERROR_IO_ERROR;
	}

	file->stream = NULL;

	return 0;
}

int
elf_dump(const char *path, uint32_t *entry,
	 memory_t *mem) {
	struct elf_file file;
	int retval = 0;

	retval = elf_open(&file, path);
	if (retval != 0) {
		error(0, 0, "couldn't get started on the ELF file.");
		return retval;
	}

	*entry = file.ehdr.e_entry;

	retval = copy_all_segments(&file, mem);
	if (retval != 0) {
		error(0, 0, "couldn't read prog segments.");
		return retval;
	}

	return elf_close(&file);
}
