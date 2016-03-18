#!/bin/sh
set -e # Stop on first error.
set -x # Print each command when executing it.

if ! [ "$INSTALL_MIPS_UTILS" ]; then
INSTALL_MIPS_UTILS=true
fi
if ! [ "$NJOBS" ]; then # Passed as -j to all `make` runs
NJOBS=4
fi
if ! [ "$OSM_DIR" ]; then
OSM_DIR="$HOME/mips"
fi
BUILD_DIR="$OSM_DIR/build"

BINUTILS_VERSION=2.26
GCC_VERSION=5.3.0

mkdir -p "$OSM_DIR"
cd "$OSM_DIR"

mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

# Download.
if [ "$INSTALL_MIPS_UTILS" = true ]; then
wget http://ftp.gnu.org/gnu/binutils/binutils-$BINUTILS_VERSION.tar.gz
wget ftp://ftp.mpi-sb.mpg.de/pub/gnu/mirror/gcc.gnu.org/pub/gcc/releases/gcc-$GCC_VERSION/gcc-$GCC_VERSION.tar.gz
fi

if [ "$INSTALL_MIPS_UTILS" = true ]; then
# Extract and build binutils for MIPS and x86_64.
cd "$BUILD_DIR"
tar zxvf binutils-$BINUTILS_VERSION.tar.gz
mkdir build-binutils
cd build-binutils
CFLAGS='-Wno-unused-value -Wno-logical-not-parentheses' \
       ../binutils-$BINUTILS_VERSION/configure \
       --target=mips-elf --prefix="$OSM_DIR"
       make -j $NJOBS
       make install
       cd ..
       rm -rf build-binutils
       mkdir build-binutils
       cd build-binutils
CFLAGS='-Wno-unused-value -Wno-logical-not-parentheses' \
	   ../binutils-$BINUTILS_VERSION/configure \
	   --target=x86_64-elf --prefix="$OSM_DIR"
	   make -j $NJOBS
	   make install

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
      --disable-shared --prefix="$OSM_DIR"
make -j $NJOBS
make install
cd ..
rm -rf build-gcc
mkdir build-gcc
cd build-gcc
../gcc-$GCC_VERSION/configure --with-gnu-ld --with-gnu-as \
   --without-nls --enable-languages=c --disable-multilib \
   --disable-libssp --disable-libquadmath --target=x86_64-elf \
   --disable-shared --prefix="$OSM_DIR"
   make -j $NJOBS
   make install
fi



echo "Please run: export PATH=\"$HOME/osm/bin:$PATH\""
