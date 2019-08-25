echo build
if [ -f test.exe ]; then rm test.exe; fi
gcc -O0 -g -Wall -Wextra -Isrc \
  src/util.c \
  src/input.c \
  src/token.c \
  src/parser.c \
  src/tree.c \
  src/main.c \
  -o test.exe

echo run
./test.exe
