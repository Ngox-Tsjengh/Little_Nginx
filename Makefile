CC		= g++
CPPFLAGS = -Wall -Wextra -ggdb

LD		= ld

SRCDIR	= core	
INCDIR	= headers
OBJDIR	= obj
BINDIR	= bin

#SLASH	= /
#TYPE	= c cpp
SRC	= $(foreach c_dir,$(SRCDIR),$(call listf,$(c_dir),$(TYPE)))
#SRCS = $(SRC)
SRCS	= $(shell find $(SRCDIR) -name '*.cpp' -o -name '*.c')
OBJS	= $(addsuffix .o, $(basename $(SRCS)))

HEAD	= $(call listf,headers,h)
CPPFLAGS += $(addprefix -I,$(HEAD))

TARGET	= nginx

# ---------------------------------------------------------------

all: $(BINDIR)/$(TARGET)

$(BINDIR)/$(TARGET): $(OBJS)
	@echo $(SRCS)
	@echo $(OBJS)
	@echo $(SRC)
	$(LD) $(OBJS) -o $@ $(LDFLAGS)
	
$(SRCDIR)/%.o: %.cpp
	echo haha

%.o: %.cpp
	echo there

.cpp.o:
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJS): $(SRCS)
	@echo $(SRCS)
	@echo $(OBJS)
	@echo $(SRC)
	$(CC) $(CPPFLAGS) $< 

.PHONY: clean

clean:
	rm -rf cscope& tags 


#list all files in some directories
listf = $(filter $(if $(2),$(addprefix %.,$(2)),),\
		$(wildcard $(addsuffix $(SLASH)*,$(1))))
