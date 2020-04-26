#include "Classify.hpp"

namespace CLNSIH001{
    using namespace std;

    //Default Constructor - *CONSIDER POINTERS*
    Classify::Classify(string input):imageFolder(input), outFile(""), numClusters(10), width(1){}
    //Destructor
    Classify::~Classify(){}
    //Copy Constructor - *FIX*
    Classify::Classify(const Classify & other):imageFolder(other.imageFolder), outFile(other.outFile), numClusters(other.numClusters), width(other.width) {}
    //Move Constructor - *DONT FORGET DELETE*
    Classify::Classify(Classify && other)/*:init variables*/{}
    //Copy Assignment Operator - *INCOMPLETE*
    Classify & Classify::operator=(const Classify & other){}
    //Move Assignment Operator
    Classify & Classify::operator=(Classify && other){
        imageFolder = move(other.imageFolder);
        outFile = move(other.outFile);
        numClusters = move(other.numClusters);
        width = move(other.width);
    }
    
    //methods
    bool readImages(std::string folderName);
}