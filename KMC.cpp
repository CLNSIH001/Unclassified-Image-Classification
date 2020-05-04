#include "Classify.hpp"
#include <stdio.h>
#include <sstream>
#include <fstream>
#include <typeinfo>
#include <time.h>
#include <set>

namespace CLNSIH001{
    using namespace std;

    //Default Constructor - *CONSIDER POINTERS*
    Classify::Classify(const string imageSet, bool color):imageFolder(imageSet), outFile(""), numClusters(10), width(1), colour(color){
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
        /*for (Cluster c : clusters){
            cout << c.name;
            for (Picture p : c.images){
                cout << p.name << ", ";
            }
            cout << endl;
        }*/
    }
    Classify::Classify(const string imageSet, const int binSize, bool color):imageFolder(imageSet), outFile(""), numClusters(10), width(binSize), colour(color){
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
        for (Cluster &c : clusters){
            cout << c.equal() << endl;
        }
    }

    void Classify::update(){
        //update each clusters centroid
        for (Cluster &c : clusters){
            for (int a = 0; a < c.length; ++a){
                c.prev[a] = c.centroid[a];
            }
            c.mean();
            //c.images.clear();
        }
    }

    void Classify::reassign(){
        //reassign
        for (int b=0; b<pics.size(); ++b){
            for (int c = 0; c < clusters.size(); ++c){
                long dist = distance(pics.at(b).histogram, clusters.at(c).centroid, clusters.at(c).length);
                if (dist < pics.at(b).minD){
                    //remove pic from its current cluster
                    for (auto i = clusters.at(pics.at(b).cluster).images.begin(); i<clusters.at(pics.at(b).cluster).images.end(); ++i){
                        if (i->name == pics.at(b).name){
                            clusters.at(pics.at(b).cluster).images.erase(i);
                            break;
                        }
                    }
                    pics.at(b).minD = dist;
                    pics.at(b).cluster = c;
                    clusters.at(c).images.push_back(pics.at(b));
                }
            }
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
        for (int i=0; i<length; ++i){
            int sum = 0;
            for (int j=0; j<images.size(); ++j){
                sum += images.at(j).histogram[i];
            }
            //cout << this->name << " " << sum << endl;
            centroid[i] = sum / images.size();
        }
    }

    bool Cluster::equal(){
        for (int x=0; x<length; ++x){
            if (centroid[x] - prev[x] != 0){return false;}
        }
        return true;
    }
}