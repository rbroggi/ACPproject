#include<iostream>
#include<fstream>
#include<iomanip>
#include "RMS_testing_2.h"


using namespace std;

double fRand(double fMin, double fMax)
{
    double f = (double)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
};

//Per provare genero due vettori
void rand_vec (vector<pair<int,double> > & v1, vector<pair<int,double> > & v2,int size_vec) {
    
    
    for (int i = 0; i < size_vec; i++) {
        
        double rnd = fRand(-1,5);
        if (rnd < 0) {
            rnd = -1;
        }
        
        v1.push_back(make_pair(i+1,rnd));
        
        rnd = fRand(-1,5);
        if (rnd < 0) {
            rnd = -1;
        }
        
        v2.push_back(make_pair(i+1,rnd));
    }


};



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
    
    cout<<"The DCG is: "<<RMS_DCG(v1,10)<<endl;
    
    vector<pair<int,double> > v3, v4;
    
    rand_vec(v3,v4,6);
    
    cout<<endl<<endl<<"Vectors: "<<endl;
    
    for (int i = 0; i < v3.size(); i++) {
        cout <<v3[i].first<<"\t";
    }
    
    cout<<endl;
    
    for (int i = 0; i < v3.size(); i++) {
        cout <<setprecision(4)<<v3[i].second<<"\t";
    }
    
    cout<<endl<<endl;
    
    for (int i = 0; i < v4.size(); i++) {
        cout <<v4[i].first<<"\t";
    }
    
    cout<<endl;
    
    for (int i = 0; i < v4.size(); i++) {
        cout <<setprecision(4)<<v4[i].second<<"\t";
    }
    
    cout<<endl<<endl;
    
    
    cout<<"Spearsman rho: "<<compute_s_rho(v3,v4)<<endl;
    
    cout<<"Kendau tau: "<<compute_k_tau(v3,v4)<<endl;
    
    
    //draw_ROC(v1,v2); ->NOT WORKING
    
    //cout<<"Fallout is: "<<RMS_fallout_robust(v2,10)<<endl;
    
    //cout<<"Precision is: "<<RMS_precision (v1, 10)<<endl;
    
    //cout<<"Robust Precision is: "<<RMS_precision_robust (v1, 10)<<endl;
    
    return 0;
}
