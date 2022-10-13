TARGET_EXECUTABLE=nes

all:
	g++ -Wall Nes.cpp Cpu.cpp -o $(TARGET_EXECUTABLE) -I/Library/Frameworks/SDL2.framework/Headers -F/Library/Frameworks -framework SDL2

clean:
	rm -rf $(TARGET_EXECUTABLE)
