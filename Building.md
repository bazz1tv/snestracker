Building
========

Three flavours of pre-built SNES Tracker binaries are available for you!
- **OLD**: simply download from the [Releases Page](https://github.com/bazzinotti/snestracker/releases).
- **LATEST**: With any purchase from the [Gift Shop](https://shop.snestracker.com/) you'll receive a one-time instant download link to the latest version of SNES Tracker.
- **LATEST+INTERIM**: As a thank you to my [Patreon](https://patreon.snestracker.com/) Supporters, who help to keep this project going, the latest version of SNES Tracker is available exclusively via Patreon a few weeks before it becomes available on github. Support me and this project by becoming a Patreon Supporter and you will not only receive a download link to the latest stable version each time it is released - you'll also unlock access to the very latest interim builds, experience up-to-the-minute bug fixes, features and improvements weeks before anybody else. Aside from builds access, you can also earn privileged roles in our Discord community, receive generous shop discounts, exclusive behind the scenes updates, and more!

Alternatively, you may perform any of the following compilation scenarios:

- compile native on OS X
- compile native on Linux
- cross-compile for Windows from Linux

In order to compile, certain library dependencies must be installed on your system first. In general, it is preferred to install these libraries through your package manager if available.

Dependencies
------------

- [SDL2](https://www.libsdl.org/download-2.0.php) - >=2.0.3-r200 - newer versions may be compatible
- [Boost](http://www.boost.org/users/history/ "Boost") >=1.56.0-r1 - a newer version of Boost is likely compatible - need only compile the following: 
  - boost\_system
  - boost\_filesystem
- Linux-only deps
  - gtk 2 or 3 (`pc/Makefile` refers to gtk2 by default)
  - alsa-lib (rtmidi dep)
- wla-dx deps
  - cmake

### Mac OS

I recommend using [homebrew](https://brew.sh/) to install the system dependencies. Once homebrew is installed, simply run the following command from the terminal.

```
brew install sdl2 boost cmake
```

### Linux systems with apt (Debian, Ubuntu, etc)

```
apt install libasound2-dev libsdl2-dev libboost-filesystem-dev libgtk2.0-dev cmake
```

### Gentoo Linux

On Gentoo, I recommend putting the following packages into a set

**/etc/portage/sets/snestracker**

```
media-libs/libsdl2
dev-libs/boost
x11-libs/gtk+:2
media-libs/alsa-lib
dev-util/cmake
```

```
emerge --ask --verbose @snestracker
```

Should the time come to release the dependencies associated with snes
tracker, simply:

```
emerge --deselect @snestracker
# depclean as usual when you are ready to cleanup your system of unused dependencies
emerge --ask --depclean
```


Submodules
----------

Snestracker relies on several internal projects that are all conveniently packaged into the snestracker repo. Provided you have installed the necessary system dependencies listed in the previous section, execute the following.

```
git clone https://github.com/bazzinotti/snestracker.git
cd snestracker
./build-submodules.sh
```

Now that the dependent submodules have been built, you only need to run the following command to build snestracker.

```
make
```

Cross-Building for Windows
--------------------------

Here are 64-bit cross build instructions for Gentoo or Ubuntu Linux.

### Gentoo

#### 64-Bit

```
emerge -av sys-devel/crossdev
crossdev --target x86_64-w64-mingw32
x86_64-w64-mingw32-emerge -av dev-libs/boost
```

Then compile SDL2 manually. (Don't use `emerge` for sdl2 or you will miss out on video hardware acceleration)

```
wget http://libsdl.org/release/SDL2-2.0.10.tar.gz
tar -zxf SDL2-2.0.10.tar.gz ; cd SDL2-2.0.10
./configure --prefix=/usr/x86_64-w64-mingw32/usr --host=x86_64-w64-mingw32 \
--enable-sdl2-config=no --with-sysroot=/usr/x86_64-w64-mingw32/usr/include
make && sudo make install
```

Then, from snestracker folder:

```
prefix=/usr/x86_64-w64-mingw32/usr CROSS_COMPILE=x86_64-w64-mingw32- make
```

Provided that compilation was successful, the following DLLs need to be copied into the directory where the EXE will be located.

```
cp /usr/x86_64-w64-mingw32/usr/{bin/SDL2.dll,lib/libboost_filesystem.dll} \
/usr/lib/gcc/x86_64-w64-mingw32/9.2.0/{libgcc_s_seh-1.dll,libstdc++-6.dll} pc/bin
```

Some of these library files are version dependent (eg. 9.2.0), so until an
automation strategy is discovered (static build?), be careful.

#### 32-Bit

```
emerge -av sys-devel/crossdev
crossdev --target i686-w64-mingw32
i686-w64-mingw32-emerge -av dev-libs/boost
```

Then compile SDL2 manually. (Don't use `emerge` for sdl2 or you will miss out on video hardware acceleration)

```
wget http://libsdl.org/release/SDL2-2.0.10.tar.gz
tar -zxf SDL2-2.0.10.tar.gz ; cd SDL2-2.0.10
./configure --prefix=/usr/i686-w64-mingw32/usr --host=i686-w64-mingw32 \
--enable-sdl2-config=no --with-sysroot=/usr/i686-w64-mingw32/usr/include
make && sudo make install
```
Then, from snestracker folder:

```
prefix=/usr/i686-w64-mingw32/usr CROSS_COMPILE=i686-w64-mingw32- make
```

Provided that compilation was successful, the following DLLs need to be copied into the directory where the EXE will be located.

```
cp /usr/i686-w64-mingw32/usr/{bin/SDL2.dll,lib/libboost_filesystem.dll} /usr/lib/gcc/i686-w64-mingw32/9.3.0/{libgcc_s_sjlj-1.dll,libstdc++-6.dll} pc/bin
```

Some of these library files are version dependent (eg. 9.2.0), so until an
automation strategy is discovered (static build?), be careful.


### Ubuntu

#### 64-bit

(tested on 16.04), You can use the following commands to setup the build environment.

```
# System deps
sudo apt-get update && sudo apt-get install mingw-w64 pkg-config

# SDL2
wget http://libsdl.org/release/SDL2-2.0.12.tar.gz
tar -zxf SDL2-2.0.12.tar.gz
cd SDL2-2.0.12
./configure --prefix=/usr/x86_64-w64-mingw32 --host=x86_64-w64-mingw32 \
--with-sysroot=/usr/x86_64-w64-mingw32/include --enable-sdl2-config=no
make
sudo make install

# Boost
wget https://sourceforge.net/projects/boost/files/boost/1.72.0/boost_1_72_0.tar.bz2
tar -jxf boost_1_72_0.tar.bz2
cd boost_1_72_0/
echo "using gcc : : x86_64-w64-mingw32-g++ ;" > user-config.jam
./bootstrap.sh
sudo ./b2 --prefix=/usr/x86_64-w64-mingw32 --with-system --with-filesystem \
  --user-config=user-config.jam toolset=gcc target-os=windows \
  variant=debug link=shared threading=multi address-model=64 install
```

Then, from the snestracker folder:

```
prefix=/usr/x86_64-w64-mingw32 CROSS_COMPILE=x86_64-w64-mingw32- make
```

Provided that compilation was successful, the following DLLs need to be copied into the directory where the EXE will be located.

```
cp /usr/x86_64-w64-mingw32/{bin/SDL2.dll,lib/libboost_filesystem.dll} \
/usr/lib/gcc/x86_64-w64-mingw32/5.3-posix/{libgcc_s_seh-1.dll,libstdc++-6.dll} pc/bin
```

Some of these library files are version dependent (eg. 5.3-posix), so until an
automation strategy is discovered (static build?), be careful.

#### 32-bit

I haven't actually tested a 32-bit compile from Ubuntu, but it probably
involves installing `i686-w64-mingw32-g++` package and adapting the 64-bit
instructions in a similar fashion as can be observed from the Gentoo
instructions.

Internal Dependencies
---------------------

The snestracker build system will automatically compile the following internally packaged deps:

- libjdkmidi (packaged internally, slightly modified to support build process)
- rtmidi (packaged internally)
- NativeFileDialog (packaged internally, and modified for cross-platform compilation)
- 7zDec (packaged internally, and modified for cross-platform compilation)
- unrar (packaged internally, and modified for cross-platform compilation)
