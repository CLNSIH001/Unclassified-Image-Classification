#include "Classify.hpp"
#include <stdio.h>
#include <sstream>
#include <fstream>
#include <typeinfo>

namespace CLNSIH001{
    using namespace std;

    //Default Constructor - *CONSIDER POINTERS*
    Classify::Classify(const string imageSet):imageFolder(imageSet), outFile(""), numClusters(10), width(1){
        string list = filesList(imageSet), name;
        istringstream iss(list);
        while (!iss.eof()){
            iss >> name;
            Picture pic;
            pic.readImages(imageFolder, name);
            pics.push_back(pic);
        }
        pics.erase(pics.end());
        for (auto & p : pics){
            p.histo(width);
        }
        
        for (int i=0; i<256; ++i){
            cout << pics.at(0).histogram[i] <<" ";
        }
        cout << endl;
    }
    //Destructor
    Classify::~Classify(){}
    //Copy Constructor - *FIX*
    Classify::Classify(const Classify & other):imageFolder(other.imageFolder), outFile(other.outFile), numClusters(other.numClusters), width(other.width) {}
    //Move Constructor - *DONT FORGET DELETE*
    Classify::Classify(Classify && other)/*:init variables*/{}
    //Copy Assignment Operator - *INCOMPLETE*
    //Classify & Classify::operator=(const Classify & other){}
    //Move Assignment Operator
    /*Classify & Classify::operator=(Classify && other){
        imageFolder = move(other.imageFolder);
        outFile = move(other.outFile);
        numClusters = move(other.numClusters);
        width = move(other.width);
    }*/
    
    //methods
    string Classify::filesList(const std::string folderName){
        char buff[256];
        string command = "cd '" + folderName + "'; ls";
        string fList = "";

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

    void Picture::readImages(const string folder, const string fileName){
        name = fileName;
        string path = folder + "/" + fileName;
        ifstream image;
        image.open(path.c_str(), ios::in | ios::binary);
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
        intensity = new int*[rows];
        int tempIterator = 0;
        unsigned char* temp = new unsigned char[rows*colms*3];
        image.read((char*)(temp), rows*colms*3);
        image.close();
        for (int row = 0; row < rows; ++row)
        {
            intensity[row] = new int[colms];
            for (int colm=0; colm<colms; ++colm){
                int r = (int)(temp[tempIterator]);
                int g = (int)(temp[tempIterator+1]);
                int b = (int)(temp[tempIterator+2]);
                intensity[row][colm] = 0.21*(r) + 0.72*(g) + 0.07*(b);
                tempIterator+=3;
            }
        }
        delete[] temp;
        temp = nullptr;
    }

    void Picture:: histo(const int width){
        /*tomorrow do something with the width,
        the other constructors an the Driver arguments*/
        for (int i = 0; i < rows; ++i)
        {
            for (int j = 0; j < colms; ++j)
            {
                histogram[intensity[i][j]]++;
            }
        }
    }
}