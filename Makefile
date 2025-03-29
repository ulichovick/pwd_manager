CXX = g++
CXXFLAGS = -Ithird_party/sqlite -std=c++17 -Wall
LDFLAGS = third_party/sqlite/sqlite3.o  # Link to compiled object

SRC_DIR = src
BUILD_DIR = build
BIN_DIR = bin

# Debug Flag (Set DEBUG=1 to enable)
DEBUG ?= 0

# Conditional Compilation Flags
ifeq ($(DEBUG),1)
    CXXFLAGS += -g -O0
else
    CXXFLAGS += -O2
endif

SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))

TARGET = $(BIN_DIR)/sqlite_app

all: third_party/sqlite/sqlite3.o $(TARGET)

third_party/sqlite/sqlite3.o: third_party/sqlite/sqlite3.c
	$(CXX) -c third_party/sqlite/sqlite3.c -o third_party/sqlite/sqlite3.o

$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(OBJS) third_party/sqlite/sqlite3.o -o $(TARGET)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp #include/database.h
	@mkdir -p $(BUILD_DIR)
	$(CXX) -c $< $(CXXFLAGS) -o $@

# Debug Mode (Compile & Run with GDB)
debug: DEBUG=1
debug: all
	gdb $(TARGET)

# Run the Program
run: $(TARGET)
	./$(TARGET)


clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

rebuild: clean all
