# Makefile for the SGE library

include Makefile.conf

CFLAGS += $(SGE_CFLAGS) -fPIC $(FT_CFLAGS)
LIBS =$(SGE_LIBS)

SGE_VER = 030809
API_VER = 0

UNAME = $(shell uname)
ARCH = $(shell uname -m)
ifeq ($(UNAME), Darwin)
	LIBNAME = libSGE.dylib
	LIBNAMEAPI = libSGE.$(API_VER).dylib
	LIBNAMEVER = libSGE.$(API_VER).$(SGE_VER).dylib
	LIBDIR = lib
	LIBFLAG = -install_name
else
	LIBNAME = libSGE.so
	LIBNAMEAPI = libSGE.so.$(API_VER)
	LIBNAMEVER = libSGE.so.$(API_VER).$(SGE_VER)
	LIBFLAG = -soname
	ifeq ($(FORCE32), y)
		LIBDIR = lib
	else
		ifeq ($(ARCH), x86_64)
			LIBDIR = lib64
		else
			LIBDIR = lib
		endif
	endif
endif


OBJECTS=sge_surface.o sge_primitives.o sge_tt_text.o sge_bm_text.o sge_misc.o sge_textpp.o sge_blib.o sge_rotation.o sge_collision.o sge_shape.o

all:	config $(OBJECTS) 
	@ar rsc libSGE.a $(OBJECTS)

$(OBJECTS):	%.o:%.cpp %.h   #Each object depends on thier .cpp and .h file
	$(CXX) $(CFLAGS) -c $<

shared: all
	$(CXX) $(CFLAGS) -Wl,$(LIBFLAG),$(LIBNAMEAPI) -fpic -fPIC -shared -o $(LIBNAME) $(OBJECTS) $(LIBS)

shared-strip:	shared
	@strip $(LIBNAME)

# Building a dll... I have no idea how to do this, but it should be something like below.
dll:	config $(OBJECTS)
	dlltool --output-def SGE.def $(OBJECTS)
	dllwrap --driver-name $(CXX) -o SGE.dll --def SGE.def --output-lib libSGE.a --dllname SGE.dll $(OBJECTS) $(LIBS)

dll-strip:	dll
	@strip SGE.dll

clean:
	@rm -f *.o *.so *.a *.dll *.def

config:
	@echo "/* SGE Config header (generated automatically) */" >sge_config.h
	@echo "#define SGE_VER $(SGE_VER)" >>sge_config.h	
ifeq ($(C_COMP),y)
	@echo "#define _SGE_C_AND_CPP" >>sge_config.h
endif
ifeq ($(USE_FT),n)
	@echo "#define _SGE_NOTTF" >>sge_config.h
endif
ifeq ($(USE_IMG),y)
	@echo "#define _SGE_HAVE_IMG" >>sge_config.h
endif
ifeq ($(NO_CLASSES),y)
	@echo "#define _SGE_NO_CLASSES" >>sge_config.h
endif

ifneq ($(QUIET),y)
	@echo "== SGE r$(SGE_VER)"
ifeq ($(C_COMP),y)
	@echo "== Note: Trying to be C friendly."
endif
ifeq ($(USE_FT),n)
	@echo "== FreeType2 support disabled."
else
	@echo "== FreeType2 support enabled."
endif
ifeq ($(USE_IMG),y)
	@echo "== SDL_Image (SFont) support enabled."
else
	@echo "== SDL_Image (SFont) support disabled."
endif
ifeq ($(NO_CLASSES),y)
	@echo "== Warning: No C++ classes will be build!"
endif
	@echo ""	
endif

install:	shared
	@mkdir -p $(PREFIX_H)
	install -c -m 644 sge*.h $(PREFIX_H)
	@mkdir -p $(PREFIX)/$(LIBDIR)
	install -c -m 644 libSGE.a $(PREFIX)/$(LIBDIR)
	install -c $(LIBNAME) $(PREFIX)/$(LIBDIR)/$(LIBNAMEVER)
	@cd $(PREFIX)/$(LIBDIR);\
	ln -sf $(LIBNAMEVER) $(LIBNAMEAPI);\
	ln -sf $(LIBNAMEAPI) $(LIBNAME)
	@echo "** Headerfiles installed in $(PREFIX_H)"
	@echo "** Library files installed in $(PREFIX)/$(LIBDIR)"
