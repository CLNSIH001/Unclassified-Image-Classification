#ifndef CLASSIFY_HPP
#define CLASSIFY_HPP
#include <string>
#include <iostream>
#include <vector>

namespace CLNSIH001{
    class Picture{
        public:
            std::string name;
            int rows, colms, maxVal;
            int** intensity;
            int* histogram;
            Picture(){
                name = "";
                rows, colms, maxVal = 0;
            }
            void readImages(const std::string folder, const std::string image);
            void histo(const int width);
    };
    class Classify{
        public:
            //variables
            std::string imageFolder, outFile;
            int numClusters, width;
            std::vector<Picture> pics;
            
            //Defualt Constructor
            Classify(const std::string imageSet);
            //Destructor
            ~Classify();
            //Copy Constructor
            Classify(const Classify & other);
            //Move Constructor
            Classify(Classify && other);
            //Copy Assignment Operator
            Classify & operator=(const Classify & other);
            //Move Assignment Operator
            Classify & operator=(Classify && other);

            //friend std::ostream & operator<<(std::ostream& os, const Classify& Classify);
            
            //methods
            std::string filesList(const std::string folderName);
            
    };
    /*std::ostream & operator<<(std::ostream& os, const Classify& Classify){
        //write code here
    }*/
}
#endif