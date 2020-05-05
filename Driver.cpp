#include "Classify.hpp"
#include <sstream>

using namespace CLNSIH001;

int main(int argc, char * argv[]){
    std::string dataset = std::string(argv[1]);//folder location containing images
    //output file
    std::string output = "";
    //number of clusters generated by algorithm
    int n = 10;
    //histogram width
    int b = 1;
    //use colour histograms
    bool flag = false;
    if (argc == 2){
        //defualt
        n=10; b=1;
        CLNSIH001::Classify KMC(dataset, n, b, flag);
        std::cout << KMC;
    }
    else{
        int i = 2;
        while (i<argc)
        {
            std::string command = std::string(argv[i]);
            if (command == "-o"){
                //output filename without the extension
                output = std::string(argv[++i]);
            }
            else if (command == "-k"){
                //number of clusters generated by algorithm
                std::istringstream(std::string(argv[++i])) >> n;
            }
            else if (command == "-bin"){
                //histogram width
                std::istringstream(std::string(argv[++i])) >> b;
            }
            else if (command == "-color"){
                //DO NOT forget to include the spelling of colour in your readme.txt
                flag = true;
            }
            i++;
        }
        CLNSIH001::Classify KMC(dataset, n, b, flag);
        if (output == ""){ std::cout << KMC;}
        else{
            KMC.WriteTo(output);
            std::cout << "The Clustering has been written to file: " << output << ".txt" << std::endl;}
    }
    return 0;
}