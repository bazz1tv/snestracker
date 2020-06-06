Building
========
You may perform any of the following compilation scenarios:

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

Here's a rough guide to establish a 64-bit cross build environment on Gentoo Linux:

```
emerge -av sys-devel/crossdev
crossdev --target x86_64-w64-mingw32
x86_64-w64-mingw32-emerge -av media-libs/libsdl2 dev-libs/boost
```

If you had previously built for Linux, clean that up before building the Windows version.

```
make clean
```

Run the following command from the snestracker top-level directory

```
prefix=/usr/x86_64-w64-mingw32/usr CROSS_COMPILE=x86_64-w64-mingw32- make
```

Provided that compilation was successful, the following DLLs need to be copied into the directory where the EXE will be located.

```
cp /usr/x86_64-w64-mingw32/usr/bin/libSDL2-2-0-0.dll \
/usr/x86_64-w64-mingw32/usr/lib/libboost_filesystem.dll \
/usr/lib/gcc/x86_64-w64-mingw32/9.2.0/{libgcc_s_seh-1.dll,libstdc++-6.dll} pc/bin
```

Some of these library files are version dependent (eg. 9.2.0), so until an
automation strategy is discovered (static build?), be careful.


Internal Dependencies
---------------------

The snestracker build system will automatically compile the following internally packaged deps:

- libjdkmidi (packaged internally, slightly modified to support build process)
- rtmidi (packaged internally)
- NativeFileDialog (packaged internally, and modified for cross-platform compilation)
- 7zDec (packaged internally, and modified for cross-platform compilation)
- unrar (packaged internally, and modified for cross-platform compilation)
