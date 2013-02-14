###################################################
# add your TestCase List Here
#
# e.g.,
# TC1 = utc_frameworkName_apiName_func
# TC2 = utc_ApplicationLib_recurGetDayOfWeek_func
TARGETS = utc_libjson_json_array_func \
	utc_libjson_json_node_func \
	utc_libjson_json_object_func
###################################################
# add your Package Config Info Here
#
# e.g.,
# PKGS=calendar
PKGS=json-glib-1.0

LIBS  = `pkg-config --libs $(PKGS)` 
LIBS += `pkg-config --libs glib-2.0` 
LIBS +=$(TET_ROOT)/lib/tet3/tcm_s.o
LIBS +=-L$(TET_ROOT)/lib/tet3 -ltcm_s
LIBS +=-L$(TET_ROOT)/lib/tet3 -lapi_s

INCS  =  -I. `pkg-config --cflags $(PKGS)`
INCS  += -I. `pkg-config --cflags glib-2.0`
INCS  += -I$(TET_ROOT)/inc/tet3 


CFLAGS = $(INCS)
CFLAGS += -D_TETWARE_MODE
CFLAGS += -Wall
LDFLAGS = $(LIBS)

###################################################
# Modify here 
# depending on the Test Case you want to build
# 
# e.g.,
# TCLIST = $(TC1) $(TC2)
all : $(TARGETS)

$(TARGETS) : %: %.c
	$(CC) -o $@ $< $(CFLAGS) $(LDFLAGS) 

clean :
	rm -rf $(TARGETS)

