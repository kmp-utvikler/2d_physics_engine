SRC=$(wildcard src/*.cpp)
OBJ=$(SRC:src/%.cpp=build/%.o)

TARGET=build/2d_physics_engine.exe

INC=-Iinclude
WFLAG=-municode -ld2d1

$(TARGET): $(OBJ)
	g++ $(OBJ) -o $@ $(WFLAG)

build/%.o: src/%.cpp | build
	g++ $(INC) -c $< -o $@

build:
	mkdir -p build

run: $(TARGET)
	$(TARGET)

clean:
	rm -f build/*

.PHONY: run clean