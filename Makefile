Lab5.o:
		g++ -o out Lab5.cpp -std=gnu++11 -fopenmp
clean:
		rm -f out *.o
