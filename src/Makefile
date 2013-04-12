#
# Makefile for CPP
#

# Compiler and compiler options:
DEBUG     = false
CXX       = g++
CC	  	  = g++
CXXFLAGS  = -pipe -O2 -Wall -W -ansi -pedantic-errors
CXXFLAGS += -std=c++11 -Wmissing-braces -Wparentheses -Wold-style-cast
ROOTDIR   = ./
BINDIR    = $(ROOTDIR)bin/
LIBDIR    = $(ROOTDIR)lib/
PROGRAM   = newsserver newsclient
SRCS      = $(wildcard *.cc)
OBJS      = $(SRCS:.cc=.o)
AGP       = $(SRCS:.cc=.d)

#Debug check
ifeq ($(DEBUG), true)
CXXFLAGS += -ggdb
LDFLAGS  += -ggdb
else
CXXFLAGS += -O2
endif

#Default target 'all', make everything
all: $(PROGRAM)
.PHONY: all clean cleaner cleanest install uninstall

# Linking
newsserver: newsserver.o server.o connection.o servercoder.o cmdtranslate.o ramstorage.o hddstorage.o article.o newsgroup.o

newsclient: newsclient.o connection.o clientcoder.o 

clean:
	$(RM) $(OBJS)
        
cleaner: clean
	$(RM) $(AGP)

cleanest: cleaner
	$(RM) $(PROGRAM).exe
	$(RM) $(PROGRAM)

install: all
	mv -f $(ROOTDIR)$(PROGRAM) $(BINDIR)
	mv -f $(ROOTDIR)$(OBJS) $(LIBDIR)

uninstall:
	(cd $(BINDIR) && $(RM) $(PROGRAM))
	(cd $(LIBDIR) && $(RM) $(OBJS))

%.d: %.cc
	@set -e; rm -f $@; \
	$(CXX) -M $(CXXFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

include $(AGP)
