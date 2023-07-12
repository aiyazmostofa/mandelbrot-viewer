default: main
init:
	mkdir build -p; cd build; cmake ../
	[ -f compile_commands.json ] || ln -s $(PWD)/build/compile_commands.json compile_commands.json

main:
	cd build; make; ./mandelbrot-viewer