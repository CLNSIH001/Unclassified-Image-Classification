#include "Classify.hpp"
#include <stdio.h>
#include <sstream>

namespace CLNSIH001{
    using namespace std;

    //Default Constructor - *CONSIDER POINTERS*
    Classify::Classify(string imageSet):imageFolder(imageSet), outFile(""), numClusters(10), width(1){
        string list = filesList(imageSet), name;
        istringstream iss(list);
        while (!iss.eof()){
            iss >> name;
            files.push_back(name);
        }
    }
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
    string Classify::filesList(std::string folderName){
        char buff[256];
        //for Linux
        string folder = "cd '" + folderName + "'; ls";
        string fList = "";
        
        //for Windows
        /*folder = "cd \"" + folderName + "\" && dir";
        pipe = popen(folder.c_str(), "r");*/

        FILE* pipe = popen(folder.c_str(), "r");
        if (!pipe) {
            folder = "cd \"" + folderName + "\" && dir";
            pipe = popen(folder.c_str(), "r");
            if (!pipe){return "failed";}
        }

        // read till end of process:
        while (!feof(pipe)) {

            // use buffer to read and add to result
            if (fgets(buff, 256, pipe) != NULL)
                fList += buff;
        }
        pclose(pipe);
        return fList;
    }

    void Classify::readImages(std::string fileName){
        //
    }
}