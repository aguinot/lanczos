# This file is part of the Astrometry.net suite.
# Licensed under a 3-clause BSD style license - see LICENSE

BASEDIR := ..
COMMON := .

all:
.PHONY: all

# You can build stripped-down versions of the AN libraries that do not
# depend on libkd or libqfits by defining the following in your Makefile
# before calling "make" in the "util" directory:
#   NO_KDTREE := 1
#   export NO_KDTREE
#   NO_QFITS := 1
#   export NO_QFITS

# Redefine these (to be in the current directory) before including
# makefile.anfiles.
ANBASE_LIB_FILE := libanbase.a
ANUTILS_LIB_FILE := libanutils.a
ANFILES_LIB_FILE := libanfiles.a

#include $(COMMON)/makefile.common
#include $(COMMON)/makefile.anfiles

SO=$(SHAREDLIB_SUFFIX)


# D'oh
ifneq ($(MAKECMDGOALS),clean)
  ifneq ($(MAKECMDGOALS),reconfig)
    include $(COMMON)/makefile.os-features
  endif
endif

NETPBM_INC_ORIG := $(NETPBM_INC)
NETPBM_LIB_ORIG := $(NETPBM_LIB)

ifeq ($(HAVE_NETPBM),yes)
else
NETPBM_INC :=
NETPBM_LIB :=
endif

CFLAGS += $(CFLAGS_DEF)
CFLAGS += -I$(ANBASE_DIR)
CFLAGS += $(CAIRO_INC)
CFLAGS += $(JPEG_INC)
CFLAGS += $(PNG_INC)
CFLAGS += $(NETPBM_INC)
CFLAGS += $(ANFILES_CFLAGS)
CFLAGS += $(ANFILES_INC)
CFLAGS += -I.

%.o: %.c
	$(CC) -o $@ $(CPPFLAGS) $(CFLAGS) -c $<

LDFLAGS += $(LDFLAGS_DEF)

LDLIBS := $(LDLIBS_DEF)
LDLIBS += $(ANFILES_LIB)

SHAREDLIBFLAGS := $(SHAREDLIBFLAGS_DEF)

QFITS_UTILS := fits-column-merge subtable fitsgetext wcsinfo

PROGS := an-fitstopnm an-pnmtofits downsample-fits \
	fits-flip-endian hpsplit wcs-to-tan	\
	get-healpix $(QFITS_UTILS) pad-file wcs-match wcs-pv2sip

MAIN_PROGS := wcs-resample tabsort wcs-xy2rd wcs-rd2xy fit-wcs wcs-pv2sip

all: $(PROGS) $(MAIN_PROGS)


ALL_OBJ := $(MISC_OBJ)

DEP_OBJ := $(MISC_OBJ)
DEP_PREREQS :=

$(MAIN_PROGS): %: %-main.o rdlist.o xylist.o starxy.o $(ANUTILS_SLIB)
$(PROGS): %: %.o $(ANUTILS_SLIB)

DEP_OBJ += $(addsuffix -main.o,$(MAIN_PROGS))
DEP_OBJ += $(addsuffix .o,$(PROGS))

an-pnmtofits: an-pnmtofits.o $(ANUTILS_SLIB)
	$(CC) -o $@ $(LDFLAGS) $^ $(NETPBM_LIB) $(LDLIBS)
ALL_OBJ += an-pnmtofits.o

_lan$(PYTHON_SO_EXT): lanczos3.i lanczos.i $(ANFILES_SLIB)
	LDFLAGS="$(LDFLAGS)" LDLIBS="$(LDLIBS)" SLIB="$(ANFILES_SLIB)" \
	INC="$(ANFILES_INC)" CFLAGS="$(CFLAGS)" \
	$(PYTHON) setup.py build_ext -v --inplace --build-temp .
lan.py: lanczos3.i lanczos.i
	LDFLAGS="$(LDFLAGS)" LDLIBS="$(LDLIBS)" SLIB="$(ANFILES_SLIB)" \
	INC="$(ANFILES_INC)" CFLAGS="$(CFLAGS)" \
	$(PYTHON) setup.py build_ext -v --inplace --build-temp .

PYUTIL := _lan$(PYTHON_SO_EXT) lan.py
pyutil: $(PYUTIL)
.PHONY: pyutil
py: $(PYUTIL)
.PHONY: py

#PYTHON_INSTALL := image2pnm.py addpath.py \
#	starutil.py starutil_numpy.py \
#	shell.py __init__.py file.py run_command.py \
#	filetype.py fits.py fix_sdss_idr.py removelines.py \
#	uniformize.py casjobs.py \
#	usnob_catalog.py usnob_get_image.py usnob_get_region.py \
#	find_data_file.py plotshift.py plotutils.py miscutils.py multiproc.py \
#	ttime.py resample.py EXIF.py horizons.py \
#	imageutils.py siap.py stages.py timingpool.py

# celestial_mechanics.py (needs license)

#PYTHON_DRIVERS := image2pnm removelines uniformize degtohms hmstodeg \
#	text2fits votabletofits merge-columns

#PYTHON_EXECS := $(addprefix ../bin/,$(PYTHON_DRIVERS))

PY_INSTALL_DIR := $(PY_BASE_INSTALL_DIR)/lan
LINK_DIR := $(PY_BASE_LINK_DIR)/lan

install: install-core
	$(MAKE) install-core
	@echo "The following copy commands may fail; they are optional."
	-$(MAKE) py && $(CP) _lan$(PYTHON_SO_EXT) lan.py '$(PY_INSTALL_DIR)'
	@echo ok

.PHONY: install

pyinstall:
	$(MKDIR) '$(PY_INSTALL_DIR)'
	@for x in $(PYTHON_INSTALL); do \
		echo $(CP) $$x '$(PY_INSTALL_DIR)/'$$x; \
		$(CP) $$x '$(PY_INSTALL_DIR)/'$$x; \
	done
	$(MAKE) py && $(CP) _util$(PYTHON_SO_EXT) util.py '$(PY_INSTALL_DIR)'

.PHONY: pyinstall

install-core: $(PYTHON_EXECS) $(PROGS) $(MAIN_PROGS) $(ANUTILS_H_PATH) \
		$(ANFILES_LIB_FILE) $(ANUTILS_LIB_FILE) $(ANBASE_LIB_FILE)
	@echo Installing in base directory '$(INSTALL_DIR)'
	@echo Installing python in base directory '$(PY_INSTALL_DIR)'
	$(MKDIR) '$(PY_INSTALL_DIR)'
	@for x in $(PYTHON_INSTALL); do \
		echo $(CP) $$x '$(PY_INSTALL_DIR)/'$$x; \
		$(CP) $$x '$(PY_INSTALL_DIR)/'$$x; \
	done
	@echo Installing python scripts in directory '$(BIN_INSTALL_DIR)'
	@echo PYTHON_SCRIPT executable: '$(PYTHON_SCRIPT)'
	$(MKDIR) '$(BIN_INSTALL_DIR)'
	@for x in $(PYTHON_EXECS); do \
		echo $(SED) 's+$(PYTHON_SCRIPT_DEFAULT)+$(PYTHON_SCRIPT)+' $$x > '$(BIN_INSTALL_DIR)/'$$x; \
		$(SED) 's+$(PYTHON_SCRIPT_DEFAULT)+$(PYTHON_SCRIPT)+' $$x > '$(BIN_INSTALL_DIR)/'$$x; \
		echo $(CHMOD_EXECUTABLE) '$(BIN_INSTALL_DIR)/'$$x; \
		$(CHMOD_EXECUTABLE) '$(BIN_INSTALL_DIR)/'$$x; \
	done
	@echo Installing executables into '$(BIN_INSTALL_DIR)'...
	@for x in $(PROGS) $(MAIN_PROGS); do \
		echo $(CP) $$x '$(BIN_INSTALL_DIR)/'$$x; \
		$(CP) $$x '$(BIN_INSTALL_DIR)/'$$x; \
	done
	@echo Installing headers into '$(INCLUDE_INSTALL_DIR)'...
	$(MKDIR) '$(INCLUDE_INSTALL_DIR)'
	@for x in $(ANUTILS_H); do \
		echo $(CP) '$(INCLUDE_DIR)/'$$x '$(INCLUDE_INSTALL_DIR)/'$$x; \
		$(CP) '$(INCLUDE_DIR)/'$$x '$(INCLUDE_INSTALL_DIR)/'$$x; \
	done
	@echo Installing libraries into '$(LIB_INSTALL_DIR)'...
	$(MKDIR) '$(LIB_INSTALL_DIR)'
	@for x in $(ANFILES_LIB_FILE) $(ANUTILS_LIB_FILE) $(ANBASE_LIB_FILE); do \
		echo $(CP) $$x '$(LIB_INSTALL_DIR)/'$$x; \
		$(CP) $$x '$(LIB_INSTALL_DIR)/'$$x; \
	done

.PHONY: install-core

$(INCLUDE_DIR)/os-features-config.h: os-features-test.c
	@echo
	@echo "---- Error messages in the next few commands are not necessarily bugs ----"
	@echo "     (we're checking how things works on your computer)"
	-rm -f os-features.log
	-rm -f $@ $@.tmp
	@echo
	@echo "Testing canonicalize_file_name..."
	@echo "Testing canonicalize_file_name..." >> os-features.log
	(($(CC) -o os-features-test-canonicalize \
	  $(CPPFLAGS) $(CFLAGS) -DTEST_CANONICALIZE_FILE_NAME $^ $(LDFLAGS) >> os-features.log && \
	  ./os-features-test-canonicalize >> os-features.log && \
	  echo "#define NEED_CANONICALIZE_FILE_NAME 0") \
	|| echo "#define NEED_CANONICALIZE_FILE_NAME 1") >> $@.tmp
	@echo
	@echo "Testing declare qsort_r..."
	@echo "Testing declare qsort_r..." >> os-features.log
	(($(CC) -o os-features-test-declare-qsort \
	   $(CPPFLAGS) $(CFLAGS) -DTEST_DECLARE_QSORT_R $^ $(LDFLAGS) >> os-features.log && \
	   ./os-features-test-declare-qsort >> os-features.log && \
	   echo "#define NEED_DECLARE_QSORT_R 1") \
	|| echo "#define NEED_DECLARE_QSORT_R 0") >> $@.tmp
	@echo
	@echo "Testing qsort_r..."
	@echo "Testing qsort_r..." >> os-features.log
	(($(CC) -o os-features-test-qsort \
	   $(CPPFLAGS) $(CFLAGS) -DTEST_QSORT_R $^ $(LDFLAGS) >> os-features.log && \
	   ./os-features-test-qsort >> os-features.log && \
	   echo "#define NEED_QSORT_R 0") \
	|| echo "#define NEED_QSORT_R 1") >> $@.tmp
	@echo
	@echo "Testing swap qsort_r..."
	@echo "Testing swap qsort_r..." >> os-features.log
	(($(CC) -Werror -o os-features-test-swap-qsort \
	   $(CPPFLAGS) $(CFLAGS) -DTEST_SWAP_QSORT_R $^ $(LDFLAGS) >> os-features.log && \
	   ./os-features-test-swap-qsort >> os-features.log && \
	   echo "#define NEED_SWAP_QSORT_R 0") \
	|| echo "#define NEED_SWAP_QSORT_R 1") >> $@.tmp
	@echo
	@echo "Testing netpbm..."
	@echo "   NETPBM_INC is $(NETPBM_INC_ORIG)"
	@echo "   NETPBM_LIB is $(NETPBM_LIB_ORIG)"
	@echo "Testing netpbm..." >> os-features.log
	(($(CC) -o os-features-test-netpbm \
	   $(CPPFLAGS) $(CFLAGS) -DTEST_NETPBM $(NETPBM_INC_ORIG) $^ $(LDFLAGS) $(NETPBM_LIB_ORIG) >> os-features.log && \
	   ./os-features-test-netpbm >> os-features.log && \
	   echo "#define HAVE_NETPBM 1") \
	|| echo "#define HAVE_NETPBM 0") >> $@.tmp
	@echo "--------------- End of expected error messages -----------------"
	@echo
	mv $@.tmp $@
	@echo
	@echo "Config results:"
	@echo "------------------------------"
	@echo
	cat $@
	@echo
	@echo "------------------------------"
	@echo

makefile.os-features: os-features-test.c
	@echo
	@echo "---- Error messages in the next few commands are not necessarily bugs ----"
	@echo "     (we're checking how things works on your computer)"
	-rm -f os-features-makefile.log
	@echo "Testing netpbm..."
	@echo "   NETPBM_INC_ORIG is $(NETPBM_INC_ORIG)"
	@echo "   NETPBM_LIB_ORIG is $(NETPBM_LIB_ORIG)"
	( \
	 echo "# This file is generated by util/Makefile."; \
	 (($(CC) -o os-features-test-netpbm-make \
	   $(CPPFLAGS) $(CFLAGS) -DTEST_NETPBM_MAKE $(NETPBM_INC_ORIG) $^ $(LDFLAGS) $(NETPBM_LIB_ORIG) >> os-features-makefile.log && \
	   ./os-features-test-netpbm-make >> os-features-makefile.log && \
	   echo "HAVE_NETPBM := yes") \
	|| (echo "# Astrometry.net didn't find netpbm; not setting HAVE_NETPBM."; \
		echo "# See os-features-makefile.log for details."; \
		echo "# To re-run this test, do 'make reconfig; make makefile.os-features' (in the 'util' directory)"; \
		echo "# Or to do it yourself, just uncomment this line:"; \
		echo "# HAVE_NETPBM := yes")) \
	; \
	echo) > $@.tmp
	@echo "--------------- End of expected error messages -----------------"
	@echo
	mv $@.tmp $@
	@echo
	@echo "Config results:"
	@echo "------------------------------"
	@echo
	cat $@
	@echo "------------------------------"
	@echo
	@echo "And, by the way, is WCSlib support being compiled in?"
	@echo
	pkg-config --exists wcslib && echo yes || echo no
	@echo
	@echo "  WCSLIB_INC: $(WCSLIB_INC)"
	@echo "  WCSLIB_LIB: $(WCSLIB_LIB)"
	@echo
	@echo "------------------------------"

reconfig:
	-rm -f '$(INCLUDE_DIR)/'os-features-config.h makefile.os-features
.PHONY: reconfig

config: $(INCLUDE_DIR)/os-features-config.h makefile.os-features
.PHONY: config

ALL_OBJ += os-features.o
ALL_TARGETS := os-features-test.o \
	os-features-test-canonicalize \
	os-features-test-declare-qsort \
	os-features-test-qsort \
	os-features-test-netpbm \
	os-features-test-netpbm-make \
	$(INCLUDE_DIR)/os-features-config.h

ifndef NO_QFITS
DEP_PREREQS += $(QFITS_LIB)
endif

# we'll just tack this on here... It depends on cairo, libjpeg, etc,
# so it's not part of the anX.a libraries.
ifeq ($(MAKECMDGOALS),cairoutils.o)
  include cairoutils.dep
endif

.PHONY: clean

clean:
	rm -f $(ANUTILS_LIB_FILE) $(ANFILES_LIB_FILE) $(ANBASE_LIB_FILE) \
		$(ALL_OBJ) $(DEPS) deps cairoutils.o \
		grab-stellarium-constellations \
		$(PROGS) $(MAIN_PROGS) $(ALL_TARGETS) $(ALL_TESTS_CLEAN) \
		cairoutils.dep makefile.os-features *.o *~ *.dep *$(PYTHON_SO_EXT) deps \
		os-features.log os-features-makefile.log report.txt

check:
	echo PYTHON_SO_EXT is $(PYTHON_SO_EXT)

# Try to compensate for clang's lack of -MG to handle generated header
# files by making everything depend explicitly on os-features-config.h.

# the bar "|" indicates an "order-only" dependency: must be built
# before, but doesn't imply a rebuild is necessary.
# Should this perhaps go on the .dep files rather than the .c?
DEP_SRC := $(subst .o,.c,$(DEP_OBJ))
$(DEP_SRC): | $(INCLUDE_DIR)/os-features-config.h
os-features.h: | $(INCLUDE_DIR)/os-features-config.h
$(DEP_OBJ): $(INCLUDE_DIR)/os-features-config.h

#ifneq ($(MAKECMDGOALS),clean)
#  ifneq ($(MAKECMDGOALS),reconfig)
#    ifneq ($(MAKECMDGOALS),makefile.os-features)
#      include $(COMMON)/makefile.deps
#    endif
#  endif
#endif
