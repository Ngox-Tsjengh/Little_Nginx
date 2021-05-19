V		= @

CC		= g++
CPPFLAGS = -Wall -Wextra -ggdb

LD		= ld

INCDIR	= headers
SRCDIR	= core	\
		  log
INCDIR += $(SRCDIR)
OBJDIR	= obj
BINDIR	= 

SLASH	= /
#TYPE	= c cpp
#SRCA	= $(foreach c_dir,$(SRCDIR),$(call listf,$(c_dir),$(TYPE)))
SRCS	= $(shell find $(SRCDIR) -name '*.cpp' -o -name '*.c')
OBJS	= $(addsuffix .o, $(basename $(SRCS)))

HEAD	= $(call listf,headers,h)
CPPFLAGS += $(addprefix -I,$(INCDIR))

TARGET	= nginx

# ---------------------------------------------------------------

all: $(BINDIR)$(TARGET) tags

$(BINDIR)$(TARGET): $(OBJS)
	$(CC) $(addprefix $(OBJDIR)/,$(notdir $^)) -o $@
	
%.o: %.cpp
	$(CC) $(CPPFLAGS) -c $< -o $(addprefix $(OBJDIR)/,$(notdir $@))

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
