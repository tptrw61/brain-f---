
CC := gcc
CXX := g++
CFLAGS := -Wall
CXXFLAGS := -Wall

intdir := ../interpreter
int := bf bfscan

gendir := ../generators
gen := printString readLine string

vardir := ../variants
var := bfdump bfscan

exe := $(int) $(var) $(gen)

VPATH = $(intdir):$(vardir):$(gendir)

.PHONY: interpreters generators variants all clean 

.DEFAULT: $(int)

%: %.*
ifeq ($(suffix $<),.sh)
	cp $< $@ && chmod +x $@
else
ifeq ($(suffix $<),.c)
	$(CC) $(CFLAGS) $< -o $@
else
ifeq ($(suffix $<),.cpp)
	$(CXX) $(CXXFLAGS) $< -o $@
else
	$(warning suffix '$(suffix $<)' not recognised)
endif
endif
endif

interpreters: $(int)

generators: $(gen)

variants: $(var)

all: $(exe)

clean:
	rm -f $(exe)
