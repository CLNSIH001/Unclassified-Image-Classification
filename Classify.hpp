#ifndef CLASSIFY_HPP
#define CLASSIFY_HPP
#include <string>
#include <iostream>
#include <vector>

namespace CLNSIH001{
    class Classify{
        public:
            //variables
            std::string imageFolder;
            std::string outFile;
            int numClusters;
            int width;
            std::vector<std::string> files;
            
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
            void readImages(std::string fileName);
    };
    /*std::ostream & operator<<(std::ostream& os, const Classify& Classify){
        //write code here
    }*/
}
#endif