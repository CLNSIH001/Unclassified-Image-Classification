#ifndef CLASSIFY_HPP
#define CLASSIFY_HPP
#include <string>
#include <iostream>
#include <vector>
//#include <map>

namespace CLNSIH001{
    class Picture{
        public:
            std::string name;
            int** intensity;
            int* histogram;
            int hgSize, rows, colms;
            int cluster; 
            long minD;
            Picture(){
                name = "";
                hgSize, rows, colms = 0;
                minD = __LONG_MAX__;
            }
            void readImages(const std::string folder, const std::string image);
            void histo(const int width);
    };
    class Cluster{
        public:
            std::string name;
            std::vector<Picture> images;
            int* centroid, *prev;
            int length;     //centroid array length
            void copyHisto(Picture image);
            void mean();
            bool equal();
    };
    class Classify{
        private:
            //variables
            std::string imageFolder, outFile;
            bool colour;
            int numClusters, width;
            std::vector<Picture> pics;
            std::vector<Cluster> clusters;
            
            //methods
            std::string filesList(void) const;
            void KMC(void);
            long distance(int* pic, int* cluster, int size);
            void update();
            void reassign();
        public:
            //Defualt Constructor
            Classify(const std::string imageSet, bool color);
            //other constructors
            Classify(const std::string imageSet, const int binSize, bool color);
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

            friend std::ostream & operator<<(std::ostream& os, const Classify& Classify);
    };
}
#endif