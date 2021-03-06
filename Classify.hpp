#ifndef CLASSIFY_HPP
#define CLASSIFY_HPP
#include <string>
#include <iostream>
#include <vector>

namespace CLNSIH001{
    class Picture{
        public:
            std::string name;
            int** intensity;
            int* histogram;
            int hgSize, rows, colms;
            int cluster; 
            long minD;
            Picture();
            //~Picture();
            void readImages(const std::string folder, const std::string image);
            void histo(const int width);
    };
    class Cluster{
        public:
            std::string name;
            std::vector<Picture> images;
            int* centroid, *prev;
            int length;     //centroid array length
            //~Cluster();
            void copyHisto(Picture image);
            void mean();
            bool equal();
    };
    class Classify{
        private:
            //variables
            std::string imageFolder;
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
            Classify(const std::string imageSet, const int k, const int bin, const bool color);
            //Copy Constructor
            Classify(const Classify & other);
            //Move Constructor
            Classify(Classify && other);
            //Copy Assignment Operator
            Classify & operator=(const Classify & other);
            //Move Assignment Operator
            Classify & operator=(Classify && other);

            friend std::ostream & operator<<(std::ostream& os, const Classify& Classify);
            void WriteTo(std::string outFile);
    };
}
#endif
