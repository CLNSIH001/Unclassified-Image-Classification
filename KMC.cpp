#include "Classify.hpp"
#include <stdio.h>
#include <sstream>
#include <fstream>
#include <time.h>
#include <set>

namespace CLNSIH001{
    using namespace std;

    //Constructor
    Classify::Classify(const std::string imageSet, const int k, const int bin, const bool color):imageFolder(imageSet), numClusters(k), width(bin), colour(color){
        string list = filesList(), name;
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
        KMC();
    }

    //Copy Constructor
    Classify::Classify(const Classify & other):imageFolder(other.imageFolder), numClusters(other.numClusters), width(other.width), pics(other.pics), clusters(other.clusters), colour(other.colour){}
    //Move Constructor
    Classify::Classify(Classify && other):imageFolder(other.imageFolder), numClusters(other.numClusters), width(other.width), colour(other.colour){
        pics.swap(other.pics);
        clusters.swap(other.clusters);
        other.imageFolder = "";
        other.numClusters = 0;
        other.width = 0;
        other.colour = false;
    }
    //Copy Assignment Operator
    Classify & Classify::operator=(const Classify & other){
        imageFolder = other.imageFolder;
        numClusters = other.numClusters;
        width = other.width;
        imageFolder = other.imageFolder;
        numClusters = other.numClusters;
        width = other.width;
        pics = other.pics;
        clusters = other.clusters;
        colour = other.colour;
    }
    //Move Assignment Operator
    Classify & Classify::operator=(Classify && other){
        imageFolder = move(other.imageFolder);
        numClusters = move(other.numClusters);
        width = move(other.width);
        imageFolder = move(other.imageFolder);
        numClusters = move(other.numClusters);
        width = move(other.width);
        pics = move(other.pics);
        clusters = move(other.clusters);
        colour = move(other.colour);
    }

    Picture::Picture(){
        name = "";
        hgSize, rows, colms = 0;
        minD = __LONG_MAX__;
    }

    /*Cluster::~Cluster(){
        delete[] centroid, prev;
        centroid, prev = nullptr;
    }

    Picture::~Picture(){
        delete[] histogram;
        for (int a=0; a<rows; a++){
            delete intensity[a];
        }
        delete intensity;
    }*/
    
    //methods
    string Classify::filesList() const{
        char buff[256];
        string command = "cd '" + imageFolder + "'; ls";
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
        int maxVal = 0;
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

    void Picture:: histo(const int w){
        if (256%w == 0){
            histogram = new int[256/w];
            for (int i=0; i < 256/w; ++i) histogram[i]=0;
            hgSize = 256/w;
        }
        else{
            histogram = new int[(256/w)+1];
            for (int i=0; i < 256/w+1; ++i) histogram[i]=0;
            hgSize = 256/w+1;
        }
        for (int i = 0; i < rows; ++i)
        {
            for (int j = 0; j < colms; ++j)
            {
                int val = (intensity[i][j]) / w;
                histogram[val]++;
            }
        }
    }

    void Classify::KMC(){
        //initialize
        srand(time(NULL));
        set<int> generatedNums;
        for (int i=0; i<numClusters; ++i){
            Cluster cluster;
            string label = "cluster " + to_string(i) + ": ";
            cluster.name = label;
            int index = rand() % pics.size();
            while (generatedNums.count(index) > 0)
            {
                index = rand() % pics.size();
            }
            pics.at(index).cluster = i;
            cluster.images.push_back(pics.at(index));
            cluster.length = pics.at(index).hgSize;
            cluster.copyHisto(pics.at(index));
            generatedNums.insert(index);
            clusters.push_back(cluster);
        }
        //assign
        for (int j=0; j<pics.size(); ++j){
            if (generatedNums.count(j) > 0){
                continue;
            }
            for (int k = 0; k < clusters.size(); ++k){
                long dist = distance(pics.at(j).histogram, clusters.at(k).centroid, clusters.at(k).length);
                if (dist < pics.at(j).minD){
                    pics.at(j).minD = dist;
                    pics.at(j).cluster = k;
                    clusters.at(k).images.push_back(pics.at(j));
                }
            }
        }
        update();
        for (Cluster &c : clusters){
            while(!c.equal()){
                reassign();
                update();
            }
        }
    }

    void Classify::update(){
        //update each clusters centroid
        for (Cluster &c : clusters){
            for (int a = 0; a < c.length; ++a){
                c.prev[a] = c.centroid[a];
            }
            c.mean();
        }
    }

    void Classify::reassign(){
        //reassign
        for (Cluster &c : clusters) c.images.clear();
        for (int b=0; b<pics.size(); ++b){
            for (int c = 0; c < clusters.size(); ++c){
                long dist = distance(pics.at(b).histogram, clusters.at(c).centroid, clusters.at(c).length);
                if (dist < pics.at(b).minD){
                    pics.at(b).minD = dist;
                    pics.at(b).cluster = c;
                }
            }
            clusters.at(pics.at(b).cluster).images.push_back(pics.at(b));
        }
    }

    void Cluster::copyHisto(Picture image){
        centroid = new int[length];
        for (int i=0; i<length; ++i){
            centroid[i] = image.histogram[i];
        }
        prev = new int[length];
    }

    long Classify::distance(int* pic, int* cluster, int size){
        long d = 0;
        for (int i=0; i<size; ++i){
            d += (pic[i] - cluster[i])*(pic[i] - cluster[i]);
        }
        return d;
    }

    void Cluster::mean(){
        if (images.size() == 0){return;}
        for (int i=0; i<length; ++i){
            int sum = 0;
            for (int j=0; j<images.size(); ++j){
                sum += images.at(j).histogram[i];
            }
            centroid[i] = sum / images.size();
        }
    }

    bool Cluster::equal(){
        for (int x=0; x<length; ++x){
            if (centroid[x] - prev[x] != 0){return false;}
        }
        return true;
    }

    ostream & operator<<(std::ostream& os, const Classify& Classify){
        for (Cluster c : Classify.clusters){
            os << c.name;
            if (c.images.size() == 0){
                os << '\n' << std::endl;
                continue;}
            for (int i=0; i < c.images.size()-1; ++i){
                os << c.images[i].name << ", ";
            }
            os << c.images[c.images.size()-1].name;
            os << '\n' << std::endl;
        }
        return os;
    }

    void Classify::WriteTo(std::string outFile){
        string output = outFile + "#.txt";
        ofstream file(output.c_str(), ios::out);
        for (Cluster c : clusters){
            file << c.name;
            if (c.images.size() == 0){
                file << '\n' << '\n';
                continue;}
            for (int i=0; i < c.images.size()-1; ++i){
                file << c.images[i].name << ", ";
            }
            file << c.images[c.images.size()-1].name;
            file << '\n' << '\n';
        }
        file.close();
    }
}
