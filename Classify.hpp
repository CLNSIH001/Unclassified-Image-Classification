#ifndef CLASSIFY_HPP
#define CLASSIFY_HPP
#include <string>
#include <iostream>
#include <vector>
#include <map>

namespace CLNSIH001{
    class Picture{
        public:
            std::string name;
            int** intensity;
            int* histogram;
            int hgSize, rows, colms;
            Picture(){
                name = "";
                hgSize, rows, colms = 0;
            }
            void readImages(const std::string folder, const std::string image);
            void histo(const int width);
            long distance(Picture other);
    };
    class Classify{
        public:
            //variables
            std::string imageFolder, outFile;
            int numClusters, width;
            std::vector<Picture> pics;
            std::map<std::string, Picture> clusters;
            
            //Defualt Constructor
            Classify(const std::string imageSet);
            //other constructors
            Classify(const std::string imageSet, const int binSize);
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
            std::string filesList(void) const;
            void KMC(void);

            
    };
    /*std::ostream & operator<<(std::ostream& os, const Classify& Classify){
        //write code here
    }*/
}
#endif