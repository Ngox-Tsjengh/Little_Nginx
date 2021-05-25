V		= @

CC		= gcc
PP		= g++
CPPFLAGS = -Wall -Wextra -ggdb \
		   -Wno-c++11-compat-deprecated-writable-strings

LD		= ld

INCDIR	= headers
SRCDIR	= core	\
		  net	\
		  log
INCDIR += $(SRCDIR)
OBJDIR	= obj
BINDIR	= 

SLASH	= /
#TYPE	= c cpp
#SRCA	= $(foreach c_dir,$(SRCDIR),$(call listf,$(c_dir),$(TYPE)))
CSRCS	= $(shell find $(SRCDIR) -name '*.c')
CPPSRCS	= $(shell find $(SRCDIR) -name '*.cpp')
COBJS	= $(addsuffix .c.o, $(basename $(CSRCS)))
CPPOBJS	= $(addsuffix .cpp.o, $(basename $(CPPSRCS)))

HEAD	= $(call listf,headers,h)
CPPFLAGS += $(addprefix -I,$(INCDIR))

TARGET	= nginx

# ---------------------------------------------------------------

all: $(BINDIR)$(TARGET) tags

$(BINDIR)$(TARGET): $(COBJS) $(CPPOBJS)
	$(PP) $(addprefix $(OBJDIR)/,$(notdir $^)) -o $@
	
%.c.o: %.c
	$(CC) $(CPPFLAGS) -c $< -o $(addprefix $(OBJDIR)/,$(notdir $@))

%.cpp.o: %.cpp
	$(PP) $(CPPFLAGS) -c $< -o $(addprefix $(OBJDIR)/,$(notdir $@))

#$(OBJS): $(SRCS)
#	$(CC) $(CPPFLAGS) -c $< -o $(addprefix $(OBJDIR)/,$(notdir $@))

.PHONY: tags clean

tags:
	$(V)rm -f cscope.files cscope.in.out cscope.out cscope.po.out tags
	$(V)find . -type f -name "*.[chS]" >cscope.files
	$(V)cscope -q -R -b
	$(V)ctags -R -L cscope.files

clean:
	rm -f obj/* $(TARGET) cscope.* tags

#list all files in some directories
listf = $(filter $(if $(2),$(addprefix %.,$(2)),),\
		$(wildcard $(addsuffix $(SLASH)*,$(1))))
