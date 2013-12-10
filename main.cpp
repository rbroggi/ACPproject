#include<iostream>
#include<fstream>
#include "RMS_testing_2.h"

using namespace std;

int main() {
    
    ifstream myReadFile, myReadFile2;
    myReadFile.open("INPUT_recall.txt");

    vector<data> v1;
    vector<data> v2;
    
    
    if (myReadFile.is_open()) {
        while (!myReadFile.eof()) {
            int it, usr, pos;
            double rate;
            
            myReadFile >>it>>usr>>pos>>rate;
            
            data d(it,usr,rate,pos);
            
            v1.push_back(d);
            
            
        }
    }
    myReadFile.close();
    
    
    myReadFile2.open("INPUT_fallout.txt");
    
    
    if (myReadFile2.is_open()) {
        while (!myReadFile2.eof()) {
            int it, usr, pos;
            double rate;
            
            myReadFile2 >>it>>usr>>pos>>rate;
            
            data d(it,usr,rate,pos);
            
            v2.push_back(d);
            
            
        }
    }
    
    cout<<"The area under the ROC curve is: "<<AUC_ROC(v1,v2)<<endl;
    
    cout<<"The ARHR is: "<<RMS_ARHR(v1,10)<<endl;
    
    cout<<"The ARP is: "<<RMS_ARP(v1,10)<<endl;
    
    //cout<<"Fallout is: "<<RMS_fallout_robust(v2,10)<<endl;
    
    //cout<<"Precision is: "<<RMS_precision (v1, 10)<<endl;
    
    //cout<<"Robust Precision is: "<<RMS_precision_robust (v1, 10)<<endl;
    
    return 0;
}
