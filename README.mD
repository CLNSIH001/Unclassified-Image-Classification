Given that Classify.hpp, Driver.cpp, KMC.cpp, Makefile and a folder (or multiple folders) containing the images are within the same directory; The program should run as intended in the assignment scope. (wont work if the folder containing images is nested within other folders)

Type make from the current directory in terminal to compile my code.
typing: ./clusterer <dataset> [-o output] [-k n] [-bin b]
Will run the program classifying images (from folder <dataset>) - that have a histogram of length 256/b (in some cases (256/b)+1) - into n clusters, from 0 to (n-1), and storing these clusters, and their images, in a textfile called output (if that is the chosen name after "-o").

If the square brackets are not included then default values for n (10) and b (1) will be used and the results will be written to the console (terminal). Each of the square brackets are optional.

"make clean" will remove the executable and the created textfiles.

Classify.hpp is my header file where the declarations are made,
KMC.cpp is the source file containing all the definitions and
Driver.cpp contains the main method.
