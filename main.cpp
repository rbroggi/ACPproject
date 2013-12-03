#include <iostream>
#include <vector>
#include "RMS_testing.h"


using namespace std;


int main (){
    
    vector<int> v1(5,0); v1[0]=1; v1[3]=2;
    vector<int> v2(5,0); v2[1]=3; v2[2]=4; v2[4]=5;
    vector<int> v3(5,0); v3[0]=6; v3[2]=7; v3[3]=8; v3[4]=9;
    vector<int> v4(5,0); v4[2]=10; v4[3]=11;
    vector<int> v5(5,0); v5[4]=12;
    vector<int> v6(5,0); v6[1]=13; v6[2]=14;
    
    vector<vector<int> > v; v.push_back(v1); v.push_back(v2); v.push_back(v3); v.push_back(v4); v.push_back(v5); v.push_back(v6);
    
    sparse_m<int> mat(v);
    
    mat.print();

    cout<<endl;

    for (int i = 0; i < mat.row_num(); i++) {
        for (int j = 0; j < mat.coll_num(); j++) {
            cout<<mat(i,j)<<"\t";
        }
        cout<<endl;
    }
    
    
    
    }