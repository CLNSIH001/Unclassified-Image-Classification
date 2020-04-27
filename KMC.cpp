#include "Classify.hpp"
#include <stdio.h>
#include <sstream>
#include <fstream>

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
        for (string image : files)
        {
            readImages(image);
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
        string command = "cd '" + folderName + "'; ls";
        string fList = "";
        
        //for Windows
        //string command = "cd \"" + folderName + "\" && dir";

        FILE* pipe = popen(command.c_str(), "r");
        if (!pipe) {
            return "failed";
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

    void Classify::readImages(string fileName){
        int maxVal;
        rows, colms, maxVal = 0;
        fileName = imageFolder + "/" + fileName;
        ifstream image(fileName.c_str(), ios::binary);
        if (!image){
            cout << "failed" << endl;
            return;
        }
        string dump;
        getline(image, dump);
        //getting rid of comments
        while (image.peek() == '#'){
            getline(image, dump);
        }
        //header info
        image >> rows >> ws >> colms >> ws;
        image >> maxVal >> ws;
        //read bytes

        image.close();
    }
}