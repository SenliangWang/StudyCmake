CC := g++
CXXFLAGS := -O2 -std=c++17
LDFLAGS := -lglut -lGL -lGLU

TARGET := sphere
SRC := main.cpp

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CXXFLAGS) -o $@ $(SRC) $(LDFLAGS)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)

