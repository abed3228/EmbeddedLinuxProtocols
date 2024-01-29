# Compiler settings - Can change to clang++ if preferred
CXX = g++

# Compiler flags
CXXFLAGS = 
#
FE=cpp

#Bulid dir
MAIN_DIR = ./build
OUT_DIR = $(MAIN_DIR)/out
BUILD_DIR = $(MAIN_DIR)/bin

#UART
UART_OBJ_DIR = ../protocols/low_level/uart/
UART_SOURCES = $(wildcard $(UART_OBJ_DIR)*.cpp)
UART_OBJ = uart.o
UART_READ = uart_read.cpp
UART_WRITE = uart_write.cpp
###


all: dirCreation UART

dirCreation:
	mkdir -p $(OUT_DIR)
	mkdir -p $(BUILD_DIR)
	
#UART
UART: UART_O
	$(CXX) $(CXXFLAGS) $(UART_READ)  $(BUILD_DIR)/$(UART_OBJ) -o $(OUT_DIR)/$(UART_READ:.$(FE)=.out) 
	$(CXX) $(CXXFLAGS) $(UART_WRITE) $(BUILD_DIR)/$(UART_OBJ) -o $(OUT_DIR)/$(UART_WRITE:.$(FE)=.out)

UART_O: $(UART_SOURCES)
	$(CXX) $(CXXFLAGS) -c $^  -o $(BUILD_DIR)/$(UART_OBJ)
	
# Clean up
clean:
	rm -rf $(MAIN_DIR)

.PHONY: all clean
