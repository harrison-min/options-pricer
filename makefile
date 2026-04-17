COMPILER = g++
CFLAGS =  -Wall
LDFLAGS = 
LIBS = 
TARGET = bin/main
SOURCE = src/main.cpp src/parser.cpp src/engine.cpp src/request.cpp

$(TARGET): $(SOURCE)
	mkdir -p bin data token request
	$(COMPILER) $(CFLAGS) $(LDFLAGS) -o $(TARGET) $(SOURCE) $(LIBS)

clean:
	rm -rf bin 
	rm -f data/*
	rm -f token/*
	rm -f request/*