EXECUTABLE = nes
LIBRARIES = -framework OpenGL -framework GLUT -framework Carbon -lpng
SOURCES = main.cpp Nes.cpp Cpu.cpp Ppu.cpp Bus.cpp Cartridge.cpp TextUtils.cpp Mapper.cpp Mapper000.cpp Mapper002.cpp

all:
	g++ $(SOURCES) -o $(EXECUTABLE) -arch arm64 -std=c++17 -mmacosx-version-min=10.15 -Wall -L/opt/homebrew/Cellar/libpng/1.6.38/lib $(LIBRARIES) -I/opt/homebrew/Cellar/libpng/1.6.38/include

clean:
	rm -rf $(EXECUTABLE)
