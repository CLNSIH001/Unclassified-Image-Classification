KMC: KMC.o Driver.o
	g++ Driver.o KMC.o -o clusterer

Driver.o: Driver.cpp
	g++ Driver.cpp -c

KMC.o: KMC.cpp
	g++ KMC.cpp -c

clean:
	rm  Driver.o KMC.o clusterer.exe