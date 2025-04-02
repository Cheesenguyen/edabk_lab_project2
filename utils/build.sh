cd ..

if [ ! -d "build" ]; then
	mkdir build
fi

gcc main.c -o build/chi_todolist_app-v0.0.1-ubuntu22.04-x64-gcc11.4.0 -std=c99 -Wpedantic -Wall -Wextra -Waggregate-return -Wbad-function-cast -Wcast-align -Wcast-qual -Wdeclaration-after-statement -Wfloat-equal -Wformat=2 -Wlogical-op -Wmissing-declarations -Wmissing-declarations -Wmissing-prototypes -Wmissing-include-dirs -Wnested-externs -Wpointer-arith -Wredundant-decls -Wsequence-point -Wshadow -Wstrict-prototypes -Wswitch -Wundef -Wunreachable-code -Wunused-but-set-parameter -Wwrite-strings
