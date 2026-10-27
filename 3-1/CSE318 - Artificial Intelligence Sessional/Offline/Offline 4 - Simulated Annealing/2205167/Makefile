CXX = g++
CXXFLAGS = -O3 -std=c++17 -Wall -Wextra

TARGET = 2205167
SRC = 2205167.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

run: $(TARGET)
	./$(TARGET) < input_sample1.txt

clean:
	rm -f $(TARGET) run_experiments *.aux *.log *.out

.PHONY: all run clean
