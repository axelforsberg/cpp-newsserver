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
PROGRAM   = src/newsserver_hdd src/newsserver_ram src/newsclient
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
	mkdir -p $(BINDIR)
	mv src/newsserver_hdd $(BINDIR)
	mv src/newsserver_ram $(BINDIR)
	mv src/newsclient $(BINDIR)

.PHONY: all clean cleaner cleanest

# Linking
src/newsserver_hdd: src/newsserver_hdd.o src/server.o src/connection.o src/servercoder.o src/cmdtranslate.o src/hddstorage.o src/article.o src/newsgroup.o

src/newsserver_ram: src/newsserver_ram.o src/server.o src/connection.o src/servercoder.o src/cmdtranslate.o src/ramstorage.o src/article.o src/newsgroup.o

src/newsclient: src/newsclient.o src/connection.o src/clientcoder.o 

clean:
	$(RM) $(OBJS)
        
cleaner: clean
	$(RM) $(AGP)

cleanest: cleaner
	$(RM) $(PROGRAM).exe
	$(RM) $(PROGRAM)

%.d: %.cc
	@set -e; rm -f $@; \
	$(CXX) -M $(CXXFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

include $(AGP)
