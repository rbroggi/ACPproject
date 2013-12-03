#include <iostream>
#include <vector>
#include "RMS_testing.h"


using namespace std;


int main (){
    
    vector<int> v1; v1.push_back(1); v1.push_back(2); v1.push_back(0); v1.push_back(0);
    vector<int> v2; v2.push_back(0); v2.push_back(3); v2.push_back(9); v2.push_back(0);
    vector<int> v3; v3.push_back(0); v3.push_back(1); v3.push_back(4); v3.push_back(0);
    
    vector<vector<int> > v; v.push_back(v1); v.push_back(v2); v.push_back(v3);
    
    sparse_m<int> mat(v);
    
    mat.print();
    
    cout<< mat(1,4)<< endl;
    cout<< mat(2,0)<<endl;
    
}