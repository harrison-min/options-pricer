COMPILER = g++
CFLAGS =  -Wall
LDFLAGS = 
LIBS = 
TARGET = bin/main
SOURCE = src/main.cpp

$(TARGET): $(SOURCE)
	mkdir -p bin
	$(COMPILER) $(CFLAGS) $(LDFLAGS) -o $(TARGET) $(SOURCE) $(LIBS)

clean:
	rm -rf bin