
SHELL = bash

# Project Name
PROJECT = planet_ring

SRCDIR = src
OUTDIR = build

INCDIRS  = $(wildcard $(SRCDIR)/*/include)
INCDIRS  := $(patsubst %,-I%, $(INCDIRS))

OUT_NAME = $(OUTDIR)/$(PROJECT)

# Sources
SRC := $(wildcard $(SRCDIR)/**/*.cpp) \
	   $(wildcard $(SRCDIR)/**/*.c)

SHADERS := $(wildcard $(SRCDIR)/shaders/*)

# Objects
OBJ = $(SRC:%.cpp=%.o)
OBJ := $(OBJ:%.c=%.o)

# Libraries

LIBS := -lglfw -lGL -ldl -larmadillo
LIB_SEARCHDIRs := -L

# Flags
DEBUG_FLAGS = -g
ASFLAGS = 
CFLAGS = 
LDFLAGS = $(LIBS)

# Rules
all : $(OBJ) $(OUT_NAME) shaders
	@echo "All targets up to date."

shaders : 
	cp $(SHADERS) $(OUTDIR)

$(OUT_NAME) : $(OBJ)
	@echo $(SRC)
	$(CXX) $(OBJ) $(DEBUG_FLAGS) -o $@ $(LDFLAGS)

%.o : %.cpp
	$(CXX) -c $(CFLAGS) $(DEBUG_FLAGS) $(INCDIRS) $< -o $@

%.o : %.c
	$(CC) -c $(CFLAGS) $(DEBUG_FLAGS) $(INCDIRS) $< -o $@

clean :
	rm -f $(OBJ)
	rm -f $(OUTDIR)/$(PROJECT)

# Create output directories if they don't already exist.
$(shell mkdir -p $(OUTDIR))
