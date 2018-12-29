echo build
gcc -O0 -g -Isrc \
  src/util.c \
  src/input.c \
  src/token.c \
  src/parser.c \
  src/tree.c \
  src/main.c \
  -o test.exe

echo run
./test.exe
