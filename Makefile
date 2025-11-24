# COMMENTS BEGIN WITH A HASH

# THE NAME OF YOUR PROJECT
PROJECT = L6A
SRC_FILES = main.cpp test_suite.cpp
H_FILES = test_suite.h List.hpp Array.hpp LinkedList.hpp
REZ_FILES = README.txt LICENSE
USERNAME = santiago_sagastegui

CXX = g++
CXXFLAGS_DEBUG = -g
CXXFLAGS_WARN = -Wall -Wextra -Wconversion -Wdouble-promotion \
				-Wunreachable-code -Wshadow -Wpedantic \
				-Wno-unused-parameter
CPPVERSION = -std=c++17

ARCHIVE_EXTENSION = zip

MKDIR = mkdir build
ifeq ($(shell echo "Windows"), "Windows")
	TARGET = $(PROJECT).exe
	RM = del -Force $(TARGET) .\build\main.o .\build\test_suite.o
	ZIPPER = tar -a -c -f
	ZIP_NAME = $(PROJECT)_$(USERNAME).$(ARCHIVE_EXTENSION)
	Q =
else
	TARGET = $(PROJECT)
	RM = rm -f $(TARGET) ./build/main.o ./build/test_suite.o
	ZIPPER = tar -acf
	Q= "
	ifeq ($(shell tar --version | grep -o "GNU tar"), GNU tar)
		ARCHIVE_EXTENSION = tar.gz
	endif
	ZIP_NAME = $(PROJECT)_$(USERNAME).$(ARCHIVE_EXTENSION)
endif

BUILD_DIR := build
OBJECTS = $(SRC_FILES:%.cpp=$(BUILD_DIR)/%.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) -o $@ $^ $(CXXFLAGS_DEBUG)

# order only prerequisite: don't rebuild, but still require
$(BUILD_DIR)/%.o: %.cpp | $(BUILD_DIR)
	$(CXX) -o $@ -c $< $(CPPVERSION) $(CXXFLAGS_DEBUG) $(CXXFLAGS_WARN)

$(BUILD_DIR):
	$(MKDIR)

clean:
	$(RM)
#	$(DEL) $(TARGET) $(OBJECTS)

depend:
	@sed -i.bak '/^# DEPENDENCIES/,$$d' Makefile
	@$(DEL) sed*
	@echo $(Q)# DEPENDENCIES$(Q) >> Makefile
	@$(CXX) $(CPPVERSION) -MM $(SRC_FILES) | sed "s/.*:/build\/\0/" >> Makefile

submission:
	@echo "Creating submission file $(ZIP_NAME) ..."
	@echo "...Zipping source files:   $(SRC_FILES) ..."
	@echo "...Zipping header files:   $(H_FILES) ..."
	@echo "...Zipping resource files: $(REZ_FILES)..."
	@echo "...Zipping Makefile..."
	$(ZIPPER) $(ZIP_NAME) $(SRC_FILES) $(H_FILES) $(REZ_FILES) build Makefile
	@echo "...$(ZIP_NAME) done!"

.PHONY: all clean depend submission
.DEFAULT: all

# Note to self: -MM doesn't have an option for prefixing a build dir
# DEPENDENCIES
build/main.o: main.cpp test_suite.h List.hpp
build/test_suite.o: test_suite.cpp test_suite.h List.hpp Array.hpp \
 LinkedList.hpp
