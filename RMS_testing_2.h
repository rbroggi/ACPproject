//
//  RMS_testing.h
//
//
//  Created by rodrigo broggi on 02/12/13.
//
//

#ifndef ____RMS_testing__
#define ____RMS_testing__

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>


using namespace std;

struct data {
    
public:
    data(int i, int u, double r, int p){ item=i; usr = u; rate=r; pos =p; };
    
    int usr_num() const {return usr;};
    int item_num() const {return item; };
    double my_rate() const {return rate;};
    int my_pos() const {return pos; }
    
    
    
private:
    int usr;
    int item;
    double rate;
    int pos;
    

};

void error (string & str){
    cerr<<str<<endl;
    exit(1);
    
}



        /*
             Queste funzioni devono prendere in INPUT un vettore con i dati previsti da un algoritmo di
            raccomandazione per l'insieme di elementi del testset considerati rilevanti (es.: che hanno rate > 4)
         
             In altre parole: prima, nel testset si escludono i rating considerati rilevanti. Poi si fa partire l'algoritmo
            di raccomandazione generando i valori esclusi. Nel vettore "relevant" sono messi i dati (si veda la struct data)
            previsti dal'algoritmo per gli elementi rilevanti.
         
        */

double RMS_recall (const vector<data> & relevant, const int N){
    
    if (relevant.size() == 0) {
        
        string s1("The relevant data must have contents! ");
        error(s1);
        
    }

    
    int hits = 0;
    
    for (auto it = relevant.begin(); it != relevant.end(); it++) {
        if (it->my_pos() <= N)
            hits++;
    }

    double h = hits;
    
    return (h/relevant.size());
    
};

double RMS_recall_robust (const vector<data> & relevant, const int N){
    
    
    if (relevant.size() == 0) {
        
        string s1("The relevant data must have contents! ");
        error(s1);
        
    }
    
    int to_decrease = 0;
    
    for (auto it = relevant.begin(); it != relevant.end(); it++) {
        if (it->my_rate() < 0 || it->my_rate() > 5) {
            to_decrease++;
        }
    }
    
    
    int hits = 0;
    
    for (auto it = relevant.begin(); it != relevant.end(); it++) {
        if (it->my_pos() <= N)
            hits++;
    }
    
    double h = hits;
    
    if (relevant.size()>to_decrease)
        return (h/(relevant.size()-to_decrease));
    
    else{
        string s1("Just unpredicted data on relevant! ");
        error(s1);
    
    }
    
    
};

double RMS_fallout (const vector<data> & not_relevant, const int N){
    
    if (not_relevant.size() == 0) {
        
        string s1("The not_relevant data must have contents! ");
        error(s1);
        
    }
    
    
    int hits = 0;
    
    for (auto it = not_relevant.begin(); it != not_relevant.end(); it++) {
        if (it->my_pos() <= N)
            hits++;
    }
    
    double h = hits;
    
    return (h/not_relevant.size());
    
};

double RMS_fallout_robust (const vector<data> & not_relevant, const int N){
    
    
    if (not_relevant.size() == 0) {
        
        string s1("The relevant data must have contents! ");
        error(s1);
        
    }
    
    int to_decrease = 0;
    
    for (auto it = not_relevant.begin(); it != not_relevant.end(); it++) {
        if (it->my_rate() < 0 || it->my_rate() > 5) {
            to_decrease++;
        }
    }
    
    
    int hits = 0;
    
    for (auto it = not_relevant.begin(); it != not_relevant.end(); it++) {
        if (it->my_pos() <= N)
            hits++;
    }
    
    double h = hits;
    
    if (not_relevant.size() > to_decrease)
        return (h/(not_relevant.size()-to_decrease));
    
    else{
        string s1("Just unpredicted data on relevant! ");
        error(s1);
    }
    
    
};

//Il vettore deve contenere dati continui (cioè bisogna che ci siano tutti gli user tra il primo e l'ultimo senza buchi di user)
//la complessità è theta(n) con n=relevant.size()
double RMS_precision (const vector<data> & relevant, const int N){

    if (relevant.size() == 0) {
        
        string s1("The relevant data must have contents! ");
        error(s1);
        
    }
				
    
    int hits = 0;
    
    int max_user_num = relevant[0].usr_num();
    int min_user_num = relevant[0].usr_num();
    
    for (auto it = relevant.begin(); it != relevant.end(); it++) {
        if (it->my_pos() <= N)
            hits++;
        
        if (it->usr_num() > max_user_num)
            max_user_num = it->usr_num();
        
        if (it->usr_num() < min_user_num)
            min_user_num = it->usr_num();
        
        
    }
    
    double h = hits;
    
    int user_num = max_user_num - min_user_num +1 ;
    
    return h/(N*user_num);

};

//complessità theta(nlogn)
double RMS_precision_robust (const vector<data> & relevant, const int N){
    
    if (relevant.size() == 0) {
        
        string s1("The relevant data must have contents! ");
        error(s1);
        
    }
    
    
    int hits = 0;
    
    vector<int> users;
    
    for (auto it = relevant.begin(); it != relevant.end(); it++) {
        if (it->my_pos() <= N)
            hits++;
        
        users.push_back(it->usr_num());
        
    }
    
    double h = hits;
    
    sort(users.begin(),users.end());
    
    int user_num = 1;
    
    
    for (auto it = (users.begin()+1); it != users.end(); it++) {
        auto it2 = it-1;
        if (*it > *it2) {
            user_num+=(*it-*it2);
        }
    }
    
    
    
    
    return h/(N*user_num);
    
};

void print (const vector<data> & relevant){

    vector<int> v;
    
    for (auto it = relevant.begin(); it!=relevant.end(); it++)
        v.push_back(it->usr_num());
    
    sort(v.begin(),v.end());
    
    cout<<endl;
    
    for (auto it = v.begin(); it!= v.end(); it++) {
        cout<<*it<<"\t";
    }

    cout<<endl;
    
};

double AUC_ROC (const vector<data> & relevant,const vector<data> & not_relevant){
    
    vector<double> recall_n; recall_n.push_back(0);
    
    vector<double> fallout_n; fallout_n.push_back(0);
    
    
    for (int i = 1; recall_n.back() < 1 || fallout_n.back() < 1 ; i++) {
        recall_n.push_back(RMS_recall(relevant,i));
        fallout_n.push_back(RMS_fallout(not_relevant,i));
    }
    
    double AUC = 0;
    
    if (recall_n.size() != fallout_n.size()) {
        string s1("for some reason the size of vectors of recall and fallout are different!");
        error(s1);
    }
    
    for (auto it = (recall_n.begin()+1), it2 = (fallout_n.begin()+1); it != recall_n.end(); it++, it2++) {
        AUC+=0.5*((*it2)-(*(it2-1)))*((*it)+(*(it-1)));
    }
    
    return AUC;
    
};







#endif /* defined(____RMS_testing__) */
