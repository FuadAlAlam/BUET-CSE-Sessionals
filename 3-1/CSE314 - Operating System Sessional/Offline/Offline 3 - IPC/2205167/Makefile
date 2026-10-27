CXX = g++
CXXFLAGS = -pthread -Wall -Wextra -O2 -std=c++17
TARGET = 2205167.out
SRC = 2205167.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET) out.txt

run: $(TARGET)
	./$(TARGET) in.txt out.txt
