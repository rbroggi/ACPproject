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
#include <algorithm>  //use sort
#include <cmath>      //use log2 (just c++11)
#include <fstream>
#include <omp.h>

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

};

bool comparison_func_rate (const pair<int,double > & a, const pair<int,double > & b){
    return (a.second > b.second);
};

bool comparison_func_item (const pair<int,int > & a, const pair<int,int > & b){
    return (a.first < b.first);
};

bool comparison_func_item_2 (const pair<int,double > & a, const pair<int,double > & b){
    return (a.first < b.first);
};

int fatorial(int x){
    int total = 1;
    while (x > 1) {
        total*=x;
        x--;
    }
    return total;
};

int combination(int p, int k){

    if (p < k) {
        string s1("In a combination we must have p >= k !");
        error(s1);
    }

    int num = 1, den = 1;

    if (k >= p-k) {
        den = fatorial(p-k);
        while (p > k) {
            num*=p;
            p--;
        }
        return num/den;

    }

    else{
        den = fatorial(k);
        int t = p-k;
        while (p > t) {
            num*=p;
            p--;
        }
        return num/den;

    }

};


        /*
             Queste funzioni devono prendere in INPUT un vettore con i dati previsti da un algoritmo di
            raccomandazione per l'insieme di elementi del testset considerati rilevanti (es.: che hanno rate > 4)

             In altre parole: prima, nel testset si escludono i rating considerati rilevanti. Poi si fa partire l'algoritmo
            di raccomandazione generando i valori esclusi. Nel vettore "relevant" sono messi i dati (si veda la struct data)
            previsti dal'algoritmo per gli elementi rilevanti.

        */

//c++11
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

//Usa il massimo numero di processori disponipili nella macchina
double RMS_recall_omp (const vector<data> & relevant, const int N){

    if (relevant.size() == 0) {

        string s1("The relevant data must have contents! ");
        error(s1);

    }

    int hits = 0, i = 0;


#pragma omp paralel for default(shared) private(i) reduction(+:hits)
    for (i=0; i < relevant.size(); i++){
        if (relevant[i].my_pos() <= N)
            hits++;

    }



    double h = hits;

    return (h/relevant.size());
};
//controlla che tutti gli elementi siano valutati (per ogni elemento non valutato diminuisce il numero di termini rilevanti - denominatore)
//tendenzialmente non la usi
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

//Usa il massimo numero di processori disponipili nella macchina
double RMS_fallout_omp (const vector<data> & not_relevant, const int N){

    if (not_relevant.size() == 0) {

        string s1("The not_relevant data must have contents! ");
        error(s1);

    }


    int hits = 0, i = 0;

#pragma omp paralel for default(shared) private(i) reduction(+:hits)
    for (i=0; i < not_relevant.size(); i++){
        if (not_relevant[i].my_pos() <= N)
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

//Il vettore deve contenere dati continui (cioè bisogna che ci siano tutti gli user tra il primo e l'ultimo senza buchi di user)
//la complessità è theta(n) con n=relevant.size()
double RMS_precision_omp (const vector<data> & relevant, const int N){

    if (relevant.size() == 0) {

        string s1("The relevant data must have contents! ");
        error(s1);

    }


    int hits = 0, i = 0;

    int max_user_num = relevant[0].usr_num();
    int min_user_num = relevant[0].usr_num();

#pragma omp paralel for default(shared) private(i) reduction(+:hits) reduction(min:min_user_num) reduction(max:max_user_num)
    for (i = 0; i < relevant.size(); i++) {
        if (relevant[i].my_pos() <= N)
            hits++;

        if (relevant[i].usr_num() > max_user_num)
            max_user_num = relevant[i].usr_num();

        if (relevant[i].usr_num() < min_user_num)
            min_user_num = relevant[i].usr_num();


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

//calcola l'area sotto la curva ROC (l'algoritmo è tanto meglio con l'aumento di quest'area che è sempre compresa tra 0 e 1 dove 0.5 sarebbe l'area
//di un algoritmo che fa previsioni in un modo aleatorie (uguali proporzioni tra TP e FP) )
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

//Ha in output un file di testo con la seguente sintasse: plot([1 2 3 ....],[3 4 5 ...]) con il primo x-assis e secondo y-assis
void draw_ROC (const vector<data> & relevant,const vector<data> & not_relevant){

    vector<double> recall_n; recall_n.push_back(0);

    vector<double> fallout_n; fallout_n.push_back(0);

    for (int i = 1; recall_n.back() < 1 || fallout_n.back() < 1 ; i++) {
        recall_n.push_back(RMS_recall(relevant,i));
        fallout_n.push_back(RMS_fallout(not_relevant,i));
    }

    ofstream myfile;
    myfile.open ("ROC_plot.txt");

    myfile<<"plot([";
    for (auto it = fallout_n.begin(); it != fallout_n.end(); it++) {
        myfile<<*it<<" ";
    }

    myfile<<"],[";

    for (auto it = recall_n.begin(); it != recall_n.end(); it++) {
        myfile<<*it<<" ";
    }

    myfile<<"]);";

    myfile.close();

    cout<<"To plot the ROC use the file 'ROC_plot.txt'"<<endl;

};

//Ha in output un file di testo con la seguente sintasse: plot([1 2 3 ....],[3 4 5 ...]) con il primo x-assis e secondo y-assis
void draw_ROC_m (const vector<data> & relevant,const vector<data> & not_relevant){

    vector<double> recall_n; recall_n.push_back(0);

    vector<double> fallout_n; fallout_n.push_back(0);

    for (int i = 1; recall_n.back() < 1 || fallout_n.back() < 1 ; i++) {
        recall_n.push_back(RMS_recall(relevant,i));
        fallout_n.push_back(RMS_fallout(not_relevant,i));
    }

    ofstream myfile;
    myfile.open ("ROC_plot.m");

    myfile<<"x =[";

    for (auto it = fallout_n.begin(); it != fallout_n.end(); it++) {
        myfile<<*it<<" ";
    }

    myfile<<"];"<<endl;

    myfile<<"y =[";

    for (auto it = recall_n.begin(); it != recall_n.end(); it++) {
        myfile<<*it<<" ";
    }

    myfile<<"];"<<endl;

    myfile<<"plot(x,y);"<<endl;



    myfile.close();

    cout<<"To plot the ROC use the script 'ROC_plot.m'"<<endl;

};

//Funzionante
double RMS_ARHR (const vector<data> & relevant, const int N){

    if (relevant.size() == 0) {

        string s1("The relevant data must have contents! ");
        error(s1);

    }


    double numerator = 0;
    int hits = 0;

    for (auto it = relevant.begin(); it != relevant.end(); it++) {
        if (it->my_pos() <= N){
            numerator+=(1.0/(it->my_pos()));
            hits++;
        }
    }

    double h = hits;

    return (numerator/hits);

};

//usa tutti processori disponibile
double RMS_ARHR_omp (const vector<data> & relevant, const int N){

    if (relevant.size() == 0) {

        string s1("The relevant data must have contents! ");
        error(s1);

    }


    double numerator = 0;
    int hits = 0, i = 0;

#pragma omp paralel for default(shared) private(i) reduction(+:hits,numerador)
    for (i = 0; i < relevant.size(); i++) {
        if (relevant[i].my_pos() <= N){
            numerator+=(1.0/(relevant[i].my_pos()));
            hits++;
        }
    }

    double h = hits;

    return (numerator/hits);

};


//Funzionante
double RMS_ARP (const vector<data> & relevant, const int N){

    if (relevant.size() == 0) {

        string s1("The relevant data must have contents! ");
        error(s1);

    }


    double numerator = 0;
    int hits = 0;

    for (auto it = relevant.begin(); it != relevant.end(); it++) {
        if (it->my_pos() <= N){
            double temp= it->my_pos();
            numerator+=(temp/N);
            hits++;
        }
    }

    double h = hits;

    return (numerator/hits);

};

double RMS_ARP_omp (const vector<data> & relevant, const int N){

    if (relevant.size() == 0) {

        string s1("The relevant data must have contents! ");
        error(s1);

    }


    double numerator = 0;
    int hits = 0, i = 0;

#pragma omp paralel for default(shared) private(i) reduction(+:hits,numerador)
    for (i = 0; i < relevant.size(); i++) {
        if (relevant[i].my_pos() <= N){
            double temp= relevant[i].my_pos();
            numerator+=(temp/N);
            hits++;
        }
    }

    double h = hits;

    return (numerator/hits);

};

//Usa funzione log2() - solo c++11
double RMS_DCG (const vector<data> & relevant, const int N){

    if (relevant.size() == 0) {

        string s1("The relevant data must have contents! ");
        error(s1);

    }


    double numerator = 0;
    int hits = 0;

    for (auto it = relevant.begin(); it != relevant.end(); it++) {
        if (it->my_pos() <= N){
            double temp= max(1.0,log2(it->my_pos()));
            numerator+=(1.0/temp);
            hits++;
        }
    }

    double h = hits;

    return (numerator/hits);

};

//Usa funzione log2() - solo c++11
double RMS_DCG_omp (const vector<data> & relevant, const int N){

    if (relevant.size() == 0) {

        string s1("The relevant data must have contents! ");
        error(s1);

    }


    double numerator = 0;
    int hits = 0, i = 0;

#pragma omp paralel for default(shared) private(i) reduction(+:hits,numerador)
    for (i = 0; i < relevant.size(); i++) {
        if (relevant[i].my_pos() <= N){
            double temp= max(1.0,log2(relevant[i].my_pos()));
            numerator+=(1.0/temp);
            hits++;
        }
    }

    double h = hits;

    return (numerator/hits);

};



//In classifica ci sono i rank degli item per un determinato user: Importante notare che ogni indice rappresenta un item, quindi
//l'indice "[i]" rappresenta lo stesso item per entrambi vettori classifica_1 e classifica_2 vettori
//Questa funzione viene chiamata dalla compute_s_rho
double s_rho (const vector<double> & classifica_1, const vector<double> & classifica_2){

    if (classifica_1.size() != classifica_2.size()) {
        string s1("In Spearmens rho computation, both ranks must equal on number! ");
        error(s1);
    }

    double r_average_1 = 0;
    double r_average_2 = 0;

    for (int i = 0; i < classifica_1.size(); i++) {
        r_average_1 += classifica_1[i];
        r_average_2 += classifica_2[i];
    }

    r_average_1 = r_average_1/classifica_1.size();
    r_average_2 = r_average_2/classifica_1.size();

    double contributo = 0;

    double std_1 = 0;
    double std_2 = 0;

    for (int i = 0; i < classifica_1.size(); i++) {
        contributo += ((classifica_1[i] - r_average_1)*(classifica_2[i] - r_average_2));
        std_1 += ((classifica_1[i] - r_average_1)*(classifica_1[i] - r_average_1));
        std_2 += ((classifica_2[i] - r_average_2)*(classifica_2[i] - r_average_2));
    }

    std_1 = sqrt((1.0/(classifica_1.size()-1))*std_1);
    std_2 = sqrt((1.0/(classifica_1.size()-1))*std_2);


    return ((1.0/classifica_1.size())*contributo*(1.0/(std_1*std_2)));
};

//vettori che hanno come elementi la coppia (item,rank) che rappresentano le classifiche predette e reali. Questi vettori arrivano ordinati in rank
//C e D sono parametri pre-calcolati nella funzione compute_k_tau importantiper la valutazione di k_tau
double k_tau (const vector<pair<int,double> > & classifica_1, const vector<pair<int,double> > & classifica_2, int C, int D){

    if (classifica_1.size() != classifica_2.size()) {
        string s1("In Kendall tau computation, both ranks must equal on number! ");
        error(s1);
    }

    int I = 0;
    int I_l = 0;

    //Approffito del fatto che i vettori arrivano ordinat in rank e calcolo I e I_l


    for (int i = 1; i < classifica_1.size(); i++) {

        int cont = 0;

        if (classifica_1[i].second == classifica_1[i-1].second) {
            int j = i;
            cont = 1;
            while (classifica_1[j].second == classifica_1[i-1].second && j < classifica_1.size()) {
                j++;
                cont++;
            }

            I += combination(cont,2);

            i = j;
        }

    }

    for (int i = 1; i < classifica_1.size(); i++) {

        int cont = 0;

        if (classifica_2[i].second == classifica_2[i-1].second) {
            int j = i;
            cont = 1;
            while (classifica_2[j].second == classifica_2[i-1].second && j < classifica_2.size()) {
                j++;
                cont++;
            }

            I_l += combination(cont,2);

            i = j;
        }

    }


    double num = C - D;

    double den = sqrt( (C+D+I)*(C+D+I_l) );

    return num/den;


};


//funzione uttilizatta da entrambi compute_k_tau e compute_s_rho per ordinare le classifiche
//prende come INPUT le classifiche in vettori (item,rate), li ordina in rank e per item con voti uguali restituisce come rank la media della loro classifica
void orderProfile(vector<pair<int,double> > & p, vector<pair<int,double> > & r){

    //ordino i vettori in rate:
    sort(p.begin(),p.end(),comparison_func_rate);
    sort(r.begin(),r.end(),comparison_func_rate);

    //uso dei vettori temporanei
    vector<pair<int,double> > p_temp;
    vector<pair<int,double> > r_temp;

    //vettori con i rank:
    for (int i = 0; i < p.size(); i++)
        p_temp.push_back(make_pair(p[i].first,double(i+1.0)));

    for (int i = 0; i < r.size(); i++)
        r_temp.push_back(make_pair(r[i].first,double(i+1.0)));


    //Nel vettore info avrò elementi che delimitano gli indici di inizio e di fine di ogni sequenza di rate uguali
    vector<pair<int,int> > info, info2;

    for (int i = 1; i < p.size(); i++) {
        if (p[i].second == p[i-1].second) {
            int j = i;
            while (p[j].second == p[i-1].second && j < p.size()) {
                j++;
            }

            info.push_back(make_pair(i-1,j-1));

            i = j;

        }

    }

    for (int i = 1; i < r.size(); i++) {
        if (r[i].second == r[i-1].second) {
            int j = i;
            while (r[j].second == r[i-1].second && j < r.size()) {
                j++;
            }

            info2.push_back(make_pair(i-1,j-1));

            i = j;

        }

    }



    //Ora li assegno il rank degli elementi con rate uguali:

    for (auto it = info.begin(); it != info.end(); it++) {

        double sum = 0;

        for (int i = it->first; i <= it->second; i++) {
            sum += p_temp[i].second;
        }

        double rank = sum/(it->second - it->first + 1.0);

        for (int i = it->first; i <= it->second; i++) {
            p_temp[i].second = rank;
        }

    }

    for (auto it = info2.begin(); it != info2.end(); it++) {

        double sum = 0;

        for (int i = it->first; i <= it->second; i++) {
            sum += r_temp[i].second;
        }

        double rank = sum/(it->second - it->first + 1.0);

        for (int i = it->first; i <= it->second; i++) {
            r_temp[i].second = rank;
        }

    }


    //finalmente sostituiamo i valori:

    p = p_temp;
    r = r_temp;


};

//Il vettore popularity contiene per ogni item il numero di uttenti che lo ha valutato
//Importante nottare che l'indice popularity[0] è il numero di uttenti che ha valutato l'item 1 e così via
//FUNZIONE FUNZIONANTE - (piccola differenza rispetto a MATLAB perchè ho messo <=N e non solo < N)
//QUANDO SCELTA LA STRUTTURA DELLA URM BISOGNA SCRIVE UNA FUNZIONE CHE CALCOLA LA popularity!
double novelity (const vector<data> & relevant, const vector<int> & popularity, const int N){

    if (relevant.size() == 0) {

        string s1("The relevant data must have contents! ");
        error(s1);

    }


    int hits = 0;
    double num = 0;

    for (auto it = relevant.begin(); it != relevant.end(); it++) {
        if (it->my_pos() <= N){
            num += log2(1.0/(popularity[it->item_num()-1]));
            hits++;
        }
    }

    double den = hits;

    return num/den;

};


//Non necessaria. L'avevo usata solo per sapere se i dati erano continui
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

//In INPUT ricevono il vettore di elementi (item,rate) previsti e reali. La funzione accetta vettori con size diversi ma è più veloce se in INPUT arrivano
//solo gli item che hanno voto in entrambi predizione e realta
//FUNZIONANTE
double compute_s_rho (const vector<pair<int,double> > & prevision,const vector<pair<int,double> > & real){


    vector<pair<int,double> > p = prevision;
    vector<pair<int,double> > r = real;


    if (real.size() > prevision.size()) {
        string s1("The prediction vector has less elements the the real vector! ");
        error(s1);
    }

    //Prima di tutto mi assicuro che siano ordinati in item:
    sort(p.begin(),p.end(),comparison_func_item_2);
    sort(r.begin(),r.end(),comparison_func_item_2);

    //Devo escludere tutti gli elementi di prevision non presenti in real e anche quelli non valutati di entrambi i vettori
    vector<pair<int,double> > p_to_send;
    vector<pair<int,double> > r_to_send;

    int it_min = max(p[0].first,r[0].first);
    int it_max = min(p.back().first,r.back().first);

    int j1 = 0;
    int j2 = 0;


    //La convenzione è che elementi non valutati hanno rate 0. Per cambiare questa convenzione sostitire nei 'if'
    //tutte le condizioni che valutano p[j1].second e r[j2].second
    for (int i = it_min; i <= it_max; i++) {

        while (p[j1].first <= i && r[j2].first <= i) {

            //Includo nei nuovi vettori solo gli elementi valutati da entrambi real e prevision
            if (p[j1].first == i && r[j2].first == i && p[j1].second != 0 && r[j2].second != 0) {
                p_to_send.push_back(make_pair(i,p[j1].second));
                r_to_send.push_back(make_pair(i,r[j2].second));
                j1++;
                j2++;
            }

            else if (p[j1].first == i && r[j2].first == i && p[j1].second == 0 && r[j2].second == 0) {
                j1++;
                j2++;
            }

            else if (p[j1].first == i && r[j2].first == i && p[j1].second != 0 && r[j2].second == 0) {
                j1++;
                j2++;
            }

            else if (p[j1].first == i && r[j2].first == i && p[j1].second == 0 && r[j2].second != 0) {
                j1++;
                j2++;
            }

            else if (p[j1].first < i && r[j2].first < i){
                j1++;
                j2++;
            }

            else if (p[j1].first == i && r[j2].first < i){
                j2++;
            }

            else if (p[j1].first < i && r[j2].first == i){
                j1++;
            }

        }
    }

    //A questo punto i vettori p_to_send e r_to_send sono ordinati in item ma non gli è stato ancora assegnato un rank e a
    //questo proposito chiamo la funzione orderProfile() che assegna i rank e restituisce i vettori ordinati in item

    orderProfile(p_to_send,r_to_send);

    sort(p_to_send.begin(),p_to_send.end(),comparison_func_item_2);
    sort(r_to_send.begin(),r_to_send.end(),comparison_func_item_2);

    vector<double> p_to_send_2;
    vector<double> r_to_send_2;

    for (auto it = p_to_send.begin(); it != p_to_send.end(); it++) {
        p_to_send_2.push_back(it->second);
    }

    for (auto it = r_to_send.begin(); it != r_to_send.end(); it++) {
        r_to_send_2.push_back(it->second);
    }

    return s_rho (p_to_send_2,r_to_send_2);

};

double compute_k_tau (const vector<pair<int,double> > & prevision,const vector<pair<int,double> > & real){


    vector<pair<int,double> > p = prevision;
    vector<pair<int,double> > r = real;


    if (real.size() > prevision.size()) {
        string s1("The prediction vector has less elements the the real vector! ");
        error(s1);
    }

    //Prima di tutto mi assicuro che siano ordinati in item:
    sort(p.begin(),p.end(),comparison_func_item_2);
    sort(r.begin(),r.end(),comparison_func_item_2);

    //Devo escludere tutti gli elementi di prevision non presenti in real e anche quelli non valutati di entrambi i vettori
    vector<pair<int,double> > p_to_send;
    vector<pair<int,double> > r_to_send;

    int it_min = max(p[0].first,r[0].first);
    int it_max = min(p.back().first,r.back().first);

    int j1 = 0;
    int j2 = 0;


    //La convenzione è che elementi non valutati hanno rate 0. Per cambiare questa convenzione sostitire nei 'if'
    //tutte le condizioni che valutano p[j1].second e r[j2].second
    for (int i = it_min; i <= it_max; i++) {

        while (p[j1].first <= i && r[j2].first <= i) {

            //Includo nei nuovi vettori solo gli elementi valutati da entrambi real e prevision
            if (p[j1].first == i && r[j2].first == i && p[j1].second != 0 && r[j2].second != 0) {
                p_to_send.push_back(make_pair(i,p[j1].second));
                r_to_send.push_back(make_pair(i,r[j2].second));
                j1++;
                j2++;
            }

            else if (p[j1].first == i && r[j2].first == i && p[j1].second == 0 && r[j2].second == 0) {
                j1++;
                j2++;
            }

            else if (p[j1].first == i && r[j2].first == i && p[j1].second != 0 && r[j2].second == 0) {
                j1++;
                j2++;
            }

            else if (p[j1].first == i && r[j2].first == i && p[j1].second == 0 && r[j2].second != 0) {
                j1++;
                j2++;
            }

            else if (p[j1].first < i && r[j2].first < i){
                j1++;
                j2++;
            }

            else if (p[j1].first == i && r[j2].first < i){
                j2++;
            }

            else if (p[j1].first < i && r[j2].first == i){
                j1++;
            }

        }
    }

    int C = 0, D = 0;

    double diff_real;
    double diff_pred;


    for (int i = 0; i < (p_to_send.size() - 1 ); i++) {
        for (int j = i+1; j < p_to_send.size(); j++) {

            diff_pred = p_to_send[i].second - p_to_send[j].second;
            diff_real = r_to_send[i].second - r_to_send[j].second;

            if (diff_real*diff_pred > 0)
                C++;

            else if (diff_real*diff_pred < 0)
                D++;

        }


    }

    //A questo punto i vettori p_to_send e r_to_send sono ordinati in item ma non gli è stato ancora assegnato un rank e a
    //questo proposito chiamo la funzione orderProfile() che assegna i rank e restituisce i vettori ordinati in item
    orderProfile(p_to_send,r_to_send);

    //p_to_send and r_to_send ordered in rank


    return k_tau (p_to_send,r_to_send,C,D);

};






#endif /* defined(____RMS_testing__) */
