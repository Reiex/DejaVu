###############################################################################
# ENVIRONMENT VARIABLES                                                       #
###############################################################################

AUTHOR = Reiex

# Source directories
SRC_LIBRARY_DIR = src/DejaVu
SRC_EXAMPLES_DIR = examples

# Include directory
INCLUDE_DIR = include

# .obj directories
OBJ_DIR = obj
OBJ_LIBRARY_DIR = $(OBJ_DIR)/DejaVu
OBJ_EXAMPLES_DIR = $(OBJ_DIR)/examples

# Compiled library directory
LIB_DIR = lib

# Built executables directory
BUILD_DIR = build

# .obj lists
LIBRARY_OBJS = $(OBJ_LIBRARY_DIR)/Core/Shape.o \
			   $(OBJ_LIBRARY_DIR)/Core/Img.o \
			   $(OBJ_LIBRARY_DIR)/Core/stb/stb_image.o \
			   $(OBJ_LIBRARY_DIR)/Core/stb/stb_image_write.o \
			   $(OBJ_LIBRARY_DIR)/ImageProcessing/Misc.o \
			   $(OBJ_LIBRARY_DIR)/ImageProcessing/FeatureDetection.o \
			   $(OBJ_LIBRARY_DIR)/ImageProcessing/Segmentation.o
EXAMPLES_OBJS = $(OBJ_EXAMPLES_DIR)/main.o


# Compiler
CC = g++
# Compiler options
LIBRARY_CFLAGS = -I$(INCLUDE_DIR) -Iexternal/include -fpic -fopenmp -std=c++17
EXAMPLES_CFLAGS = -I$(INCLUDE_DIR) -Iexternal/include -std=c++17
# Linker options
LDFLAGS = -L$(LIB_DIR) -Lexternal/lib -Wl,-rpath=$(LIB_DIR):external/lib
# Libraries linked
LIBRARY_LDLIBS = -lSciPP -lgomp
EXAMPLES_LDLIBS = -lDejaVu -lgomp


###############################################################################
# GENERAL TARGETS                                                             #
###############################################################################


all: folders DejaVu examples

docs:
	doxygen doc/doxyfilerc

clean:
	find $(OBJ_DIR) -type f -exec rm -rf \{\} \;
	find $(BUILD_DIR) -type f -exec rm -rf \{\} \;
	-rm $(LIB_DIR)/libDejaVu.so


###############################################################################
# GROUP TARGETS                                                               #
###############################################################################


DejaVu: $(LIB_DIR)

examples: $(LIB_DIR) $(EXAMPLES_OBJS)
	$(CC) $(EXAMPLES_OBJS) -o $(BUILD_DIR)/DejaVuExamples $(LDFLAGS) $(EXAMPLES_LDLIBS)

folders:
	-rm -rf $(LIB_DIR)/libDejaVu.so $(OBJ_DIR) $(BUILD_DIR)
	mkdir $(OBJ_DIR) $(OBJ_TESTS_DIR) $(OBJ_EXAMPLES_DIR) $(OBJ_LIBRARY_DIR)
	mkdir $(OBJ_LIBRARY_DIR)/Core $(OBJ_LIBRARY_DIR)/Core/stb $(OBJ_LIBRARY_DIR)/ImageProcessing
	mkdir $(BUILD_DIR)


###############################################################################
# LIBRARY BUILDING TARGETS                                                    #
###############################################################################


$(LIB_DIR): $(LIBRARY_OBJS)
	-rm -rf $(LIB_DIR)/libDejaVu.so
	$(CC) -shared $(LIBRARY_OBJS) -o $(LIB_DIR)/libDejaVu.so $(LDFLAGS) $(LIBRARY_LDLIBS)

$(OBJ_LIBRARY_DIR)/%.o: $(SRC_LIBRARY_DIR)/%.cpp
	$(CC) $(LIBRARY_CFLAGS) -c $< -o $@


###############################################################################
# COMPILATION DES EXEMPLES ET DES TESTS                                       #
###############################################################################


$(OBJ_TESTS_DIR)/%.o: $(SRC_TESTS_DIR)/%.cpp
	$(CC) $(TESTS_CFLAGS) -c $< -o $@

$(OBJ_EXAMPLES_DIR)/%.o: $(SRC_EXAMPLES_DIR)/%.cpp
	$(CC) $(EXAMPLES_CFLAGS) -c $< -o $@
