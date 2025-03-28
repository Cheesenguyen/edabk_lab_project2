cd ..

if [ ! -d "build" ]; then
	mkdir build
fi
gcc main.c -o build/main
