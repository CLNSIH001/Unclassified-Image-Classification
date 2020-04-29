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
            Picture(){
                name = "";
                rows, colms, maxVal = 0;
            }
            void readImages(std::string folder, std::string image);
    };
    class Classify{
        public:
            //variables
            std::string imageFolder, outFile;
            int numClusters, width;
            std::vector<Picture> pics;
            
            //Defualt Constructor
            Classify(std::string imageSet);
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
            std::string filesList(std::string folderName);
            
    };
    /*std::ostream & operator<<(std::ostream& os, const Classify& Classify){
        //write code here
    }*/
}
#endif