#!/bin/sh

# Stop on first error
set -e #

# Print each command
set -x

NJOBS=4

MIPS_DIR="$HOME/mips"
BUILD_DIR="$MIPS_DIR/build"

BINUTILS_VERSION=2.26
GCC_VERSION=5.3.0

mkdir -p "$MIPS_DIR"
cd "$MIPS_DIR"

mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

# Download.
wget http://ftp.gnu.org/gnu/binutils/binutils-$BINUTILS_VERSION.tar.gz
wget ftp://ftp.mpi-sb.mpg.de/pub/gnu/mirror/gcc.gnu.org/pub/gcc/releases/gcc-$GCC_VERSION/gcc-$GCC_VERSION.tar.gz

# Extract and build binutils for MIPS and x86_64.
cd "$BUILD_DIR"
tar zxvf binutils-$BINUTILS_VERSION.tar.gz
mkdir build-binutils
cd build-binutils
CFLAGS='-Wno-unused-value -Wno-logical-not-parentheses' \
       ../binutils-$BINUTILS_VERSION/configure \
       --target=mips-elf --prefix="$MIPS_DIR"
       make -j $NJOBS
       make install > /dev/null
       cd ..

# Extract and build gcc for MIPS and x86_64.
cd "$BUILD_DIR"
tar xf gcc-$GCC_VERSION.tar.gz

# On Linux (and Maybe OS X), download GMP 4.3+, MPCGMP 4.2+,
# MPFR 2.4.0+, and MPC 0.8.0+.  These will then be automatically built
# before gcc.
cd "gcc-$GCC_VERSION"
./contrib/download_prerequisites
cd "$BUILD_DIR"

mkdir build-gcc
cd build-gcc
../gcc-$GCC_VERSION/configure --with-gnu-ld --with-gnu-as \
      --without-nls --enable-languages=c --disable-multilib \
      --disable-libssp --disable-libquadmath --target=mips-elf \
      --disable-shared --prefix="$MIPS_DIR"
make -j $NJOBS
make install > /dev/null

# PATH=$HOME/
export PATH="$MIPS_DIR/bin:$PATH"
