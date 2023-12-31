g++ -c src/*.cpp 
rm main.o
ar r libmystack_without_info.a *.o
rm *.o

g++ -c -DINFO src/*.cpp 
rm main.o
ar r libmystack_with_info.a *.o
rm *.o



