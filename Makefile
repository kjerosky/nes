TARGET_EXECUTABLE=nes

all:
	g++ -Wall Nes.cpp Cpu.cpp Bus.cpp -o $(TARGET_EXECUTABLE) -I/Library/Frameworks/SDL2.framework/Headers -F/Library/Frameworks -framework SDL2 -lncurses

clean:
	rm -rf $(TARGET_EXECUTABLE)
