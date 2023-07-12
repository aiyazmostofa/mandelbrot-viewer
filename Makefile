default: main

init:
ifeq ($(OS),Windows_NT)
	mkdir build -p; cd build; cmake ../ -G "MinGW Makefiles"
else
	mkdir build -p; cd build; cmake ../
endif
	[ -f compile_commands.json ] || cp ./build/compile_commands.json compile_commands.json

main:
ifeq ($(OS),Windows_NT)
	cd build; make; ./mandelbrot-viewer.exe
else
	cd build; make; ./mandelbrot-viewer
endif
