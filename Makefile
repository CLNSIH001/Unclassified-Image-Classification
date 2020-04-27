KMC: KMC.o Driver.o
	g++ Driver.o KMC.o -o clusterer

Driver.o: Driver.cpp
	g++ Driver.cpp --std=c++11

KMC.o: KMC.cpp
	g++ KMC.cpp --std=c++11

clean:
	rm  *.o clusterer.exe