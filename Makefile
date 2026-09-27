CXX := clang++
CXXFLAGS := -std=c++14 -Wall -Wextra -pedantic -I/opt/homebrew/include
LDFLAGS := -L/opt/homebrew/lib -lraylib -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo

SRCS := main.cpp UTT.cpp

main: $(SRCS) UTT.h
	$(CXX) $(CXXFLAGS) $(SRCS) -o $@ $(LDFLAGS)

.PHONY: clean
clean:
	rm -f main
