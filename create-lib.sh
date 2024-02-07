gcc -c src/*.c 
rm main.o
ar r libmystack_without_info.a *.o
rm *.o

gcc -c -DINFO src/*.c 
rm main.o
ar r libmystack_with_info.a *.o
rm *.o



