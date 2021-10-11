CXX = gcc
OPTS = -Wall
SOURCE_PATH = mywc.c
TARGET = mywc

.PHONY: all install uninstall

all: $(TARGET)
$(TARGET): mywc.c
			$(CXX) -o mywc $(OPTS) mywc.c
install:
			install $(TARGET)
uninstall:
			rm -rf $(TARGET)