. ./flags

cd ..

# gcc -c src/stack.c 
# ar r libmystack_without_info.a *.o
# rm *.o
#
# gcc -c -DINFO src/stack.c 
# ar r libmystack_with_info.a *.o
# rm *.o

gcc -DINFO -DDEBUG $CFLAGS src/main.c src/stack.c src/crc32.c -lm -o main
# gcc -DINFO -DDEBUG src/main.c src/stack.c src/crc32.c -lm -o main
# gcc -DDEBUG src/main.c src/stack.c src/crc32.c -lm -o main
# gcc src/main.c src/stack.c -lm -o main



