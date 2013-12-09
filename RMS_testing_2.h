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
             Questa funzione deve prendere in INPUT un vettore con i dati previsti da un agoritmo di
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

double AUR(const vector<data> & relevant,const vector<data> & not_relevant);


#endif /* defined(____RMS_testing__) */
