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

//In classifica ci sono i rank degli item per un determinato user: Importante notare che ogni indice rappresenta un item, quindi
//l'indice "[i]" rappresenta lo stesso item per entrambi vettori classifica_1 e classifica_2 vettori
//Oss.: Creddo che sia più intelligente passare così gli argomenti perché non c'è bisogno che si passi la URM intera.
//PARALLELIZABILE
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
    
    std_1 = sqrt((1.0/classifica_1.size())*std_1);
    std_2 = sqrt((1.0/classifica_1.size())*std_2);
    
    
    return ((1.0/classifica_1.size())*contributo*(1.0/(std_1*std_2)));
};

//INPUT ha la stessa forma dei quello di s_rho. OUTPUT un coefficient k appartenente a [-1,1]
double k_tau (const vector<double> & classifica_1, const vector<double> & classifica_2){
    
    if (classifica_1.size() != classifica_2.size()) {
        string s1("In Kendall tau computation, both ranks must equal on number! ");
        error(s1);
    }
   
    int C = 0;
    int D = 0;
    int I = 0;
    int I_l = 0;
    
    for (int i = 0; i < classifica_1.size(); i++) {
        
        if (classifica_1[i] == classifica_2[i])
            C++;
        else
            D++;
        
    }
    
    vector<double> v1 = classifica_1;
    vector<double> v2 = classifica_2;
    
    sort(v1.begin(),v1.end());
    sort(v2.begin(),v2.end());
    
    for (int i = 1; i < v1.size(); i++) {
        
        if (v1[i] == v1[i-1])
            I++;
        
        if (v2[i] == v2[i-1])
            I_l++;
        
    }
    
    
    
    double num = C - D;
    
    double den = sqrt( (C+D+I)*(C+D+I_l) );
    
    return num/den;
    
    
};

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
            
            info.push_back(make_pair(i-1,j));
            
            i = j;
            
        }
    
    }
    
    for (int i = 1; i < r.size(); i++) {
        if (r[i].second == r[i-1].second) {
            int j = i;
            while (r[j].second == r[i-1].second && j < r.size()) {
                j++;
            }
            
            info2.push_back(make_pair(i-1,j));
            
            i = j;
            
        }
        
    }
    
    
    //Ora ri assegno il rank degli elementi con rate uguali:
    
    for (auto it = info.begin(); it != info.end(); it++) {
        
        double sum = 0;
        
        for (int i = it->first; i <= it->second; i++) {
            sum += p_temp[i].second;
        }
        
        double rate = sum/(it->second - it->first);
        
        for (int i = it->first; i <= it->second; i++) {
            p_temp[i].second = rate;
        }
        
    }
    
    for (auto it = info2.begin(); it != info2.end(); it++) {
        
        double sum = 0;
        
        for (int i = it->first; i <= it->second; i++) {
            sum += r_temp[i].second;
        }
        
        double rate = sum/(it->second - it->first);
        
        for (int i = it->first; i <= it->second; i++) {
            r_temp[i].second = rate;
        }
        
    }
    
    //finalmente sostituiamo i valori:
    
    p = p_temp;
    r = r_temp;
    
    sort(p.begin(),p.end(),comparison_func_item_2);
    sort(r.begin(),r.end(),comparison_func_item_2);
    
    
};

//Questa funzione gestisce i dati previsti e reali di un'uttente in modo da usare la funzione "s_rho" nel modo correto, Nel senso che mette a posto i vettori nel
//modo in cui bisogna chiamare la funzione s_rho
//In INPUT si attente un vettore previsioni e un vettore real con dentro i rispettivi ratting (accetano anche vettori con rate non assegnati (convenzione = -1))
//complessità nlogn (uso sort)
//double compute_s_rho (const vector<pair<int,double> > & prevision,const vector<pair<int,double> > & real){
//
//    //per poter non cambiare l'ordine dei vettori in entrata
//    vector<pair<int,double> > p = prevision;
//    vector<pair<int,double> > r = real;
//    
//    //ordina in base al rating (in ordine decrescente)
//    sort(p.begin(),p.end(),comparison_func_rate);
//    sort(r.begin(),r.end(),comparison_func_rate);
//    
//    /*   - per testare
//    //inizio test print (da cancellare fino alla prossima osservazione)
//    cout<<endl<<endl<<"Vectors ordered in rate: "<<endl;
//    
//    for (int i = 0; i < p.size(); i++) {
//        cout <<p[i].first<<"\t";
//    }
//    
//    cout<<endl;
//    
//    for (int i = 0; i < p.size(); i++) {
//        cout <<setprecision(4)<<p[i].second<<"\t";
//    }
//    
//    cout<<endl<<endl;
//    
//    for (int i = 0; i < r.size(); i++) {
//        cout <<r[i].first<<"\t";
//    }
//    
//    cout<<endl;
//    
//    for (int i = 0; i < r.size(); i++) {
//        cout <<setprecision(4)<<r[i].second<<"\t";
//    }
//    
//    cout<<endl<<endl;
//    //fine test
//    */
//     
//    //taglio gli elementi senza rate:
//    auto it = p.begin();
//    auto it2 = r.begin();
//    
//    while(it != p.end() && it->second != -1) {
//        it++;
//    }
//    
//    while(it2 != r.end() && it2->second != -1) {
//        it2++;
//    }
//    
//    if (it != p.end())
//        p.erase(it,p.end());
//
//    
//    if (it2 != r.end())
//        r.erase(it2,r.end());
//    
//    /*
//    //inizio test print (da cancellare fino alla prossima osservazione)
//    cout<<endl<<endl<<"Vectors ordered in rate, senza unrated: "<<endl;
//    
//    for (int i = 0; i < p.size(); i++) {
//        cout <<p[i].first<<"\t";
//    }
//    
//    cout<<endl;
//    
//    for (int i = 0; i < p.size(); i++) {
//        cout <<setprecision(4)<<p[i].second<<"\t";
//    }
//    
//    cout<<endl<<endl;
//    
//    for (int i = 0; i < r.size(); i++) {
//        cout <<r[i].first<<"\t";
//    }
//    
//    cout<<endl;
//    
//    for (int i = 0; i < r.size(); i++) {
//        cout <<setprecision(4)<<r[i].second<<"\t";
//    }
//    
//    cout<<endl<<endl;
//    //fine test
//    */
//
//    
//    //in questi metto per ogni item il suo corrisondente rank
//    vector<pair<int,int> > ranked_rated_1;
//    vector<pair<int,int> > ranked_rated_2;
//    
//    int rank = 1;
//    
//    for (int i = 0; i < p.size(); i++){
//        
//        //Garantire che è strettamente minore (perché per ratting uguali il rank dev'essere uguale)
//        if (i != 0) {
//            if (p[i].second < p[i-1].second) {
//                rank++;
//            }
//        }
//        ranked_rated_1.push_back(make_pair(p[i].first,rank));
//        
//    }
//   
//    rank = 1;
//    
//    for (int i = 0; i < r.size(); i++){
//        
//        if (i != 0) {
//            if (r[i].second < r[i-1].second) {
//                rank++;
//            }
//        }
//        
//        ranked_rated_2.push_back(make_pair(r[i].first,rank));
//    }
//    
//    /*
//    //inizio test print (da cancellare fino alla prossima osservazione)
//    cout<<endl<<endl<<"Vectors ordered in rate, senza unrated e con rank: "<<endl;
//    
//    for (int i = 0; i < ranked_rated_1.size(); i++) {
//        cout <<ranked_rated_1[i].first<<"\t";
//    }
//    
//    cout<<endl;
//    
//    for (int i = 0; i < ranked_rated_1.size(); i++) {
//        cout <<setprecision(4)<<ranked_rated_1[i].second<<"\t";
//    }
//    
//    cout<<endl<<endl;
//    
//    for (int i = 0; i < ranked_rated_2.size(); i++) {
//        cout <<ranked_rated_2[i].first<<"\t";
//    }
//    
//    cout<<endl;
//    
//    for (int i = 0; i < ranked_rated_2.size(); i++) {
//        cout <<setprecision(4)<<ranked_rated_2[i].second<<"\t";
//    }
//    
//    cout<<endl<<endl;
//    //fine test
//     */
//    
//    //ordinando i vettori in ordine di item
//    sort(ranked_rated_1.begin(),ranked_rated_1.end(),comparison_func_item);
//    sort(ranked_rated_2.begin(),ranked_rated_2.end(),comparison_func_item);
//    
//    //l'item minimo e massimo che hanno possibilità di essere stati valutati da entrambi classifiche
//    int it_min = max(ranked_rated_2[0].first,ranked_rated_1[0].first);
//    int it_max = min(ranked_rated_1.back().first,ranked_rated_2.back().first);
//    
//    //Questi vettori conterrano i rank degli item valutati in entrambi i vettori con ogni indice rappresentando un item (ci saranno solo item valutati realmente
//    //e previsti)
//    vector<int> p_to_send;
//    vector<int> r_to_send;
//    
//    int j1=0;
//    int j2=0;
//    
//    
//    /*
//    In questo ciclo l'idea è andare a valutare nei vettori ordinati per item, qualli sono gli elementi che sono valutati (presenti) in entrambi vettori
//    si noti che abbiamo già escluso tutti gli elementi non valutati di ogni vettore ora bisogna solo "filtrare" qualli sono gli item presenti in entrambi
//    Quindi l'idea è fare un ciclo sugli item (i) che possono essere presenti in entrambi vettori (sicuramente compresi tra it_min e it_max)
//    Quindi si procede valutando ogni elemento dei vettori:
//    Es.:
//    v1 = (1,5),(3,3),(4,2),(7,4),(10,6),(12,1)
//    v2 = (2,1),(3,2),(5,3),(7,4)
//    Mio ciclo "for" andrà da 2 a 7 (che sono gli elementi possibilmente present in entrambi vettori)
//    quindi con i contatori j1 e j2 partendo da 0 verifico ongi elemento dentro il ciclo while:
//        1ºloop while: (1 ==) ranked_rated_1[j1].first < 2 &&  (2 ==) ranked_rated_2[j2].first == 2 (entro nel loop) e ricado nel 4º caso
//        2ºloop while: (3 ==) ranked_rated_1[j1].first > 2 &&  (2 ==) ranked_rated_2[j2].first == 2 (non entro nel loop) -> i++ (i == 3)
//        1ºloop while: (3 ==) ranked_rated_1[j1].first == 3 &&  (2 ==) ranked_rated_2[j2].first < 3 (entro nel loop nel 3º caso)
//        2ºloop while: (3 ==) ranked_rated_1[j1].first == 3 &&  (3 ==) ranked_rated_2[j2].first == 3 (entro nel loop nel 1º caso e includo nei miei vettori da spedire
//         il rank del vettore predetto e del vettore reale)
//     
//     Continuando il raggionamento i miei vettori risultanti saranno:
//        p_to_send = 3,4;
//        r_to_send = 2,4;
//     */
//    
//    for (int i = it_min; i <= it_max; i++) {
//        
//        while (ranked_rated_1[j1].first <= i && ranked_rated_2[j2].first <= i) {
//            
//            
//            if (ranked_rated_1[j1].first == i && ranked_rated_2[j2].first == i) {
//                p_to_send.push_back(ranked_rated_1[j1].second);
//                r_to_send.push_back(ranked_rated_2[j2].second);
//                j1++;
//                j2++;
//            }
//            
//            else if (ranked_rated_1[j1].first < i && ranked_rated_2[j2].first < i){
//                j1++;
//                j2++;
//            }
//            
//            else if (ranked_rated_1[j1].first == i && ranked_rated_2[j2].first < i){
//                j2++;
//            }
//            
//            else if (ranked_rated_1[j1].first < i && ranked_rated_2[j2].first == i){
//                j1++;
//            }
//            
//        }
//    }
//    
//    /*
//    //inizio test print (da cancellare fino alla prossima osservazione)
//    cout<<endl<<endl<<"Vectors sent to func : "<<endl;
//    
//    for (int i = 0; i < p_to_send.size(); i++) {
//        cout <<p_to_send[i]<<"\t";
//    }
//    
//    cout<<endl;
//    
//    
//    
//    for (int i = 0; i < r_to_send.size(); i++) {
//        cout <<r_to_send[i]<<"\t";
//    }
//    
//    
//    cout<<endl<<endl;
//    //fine test
//    */
//    
//    //A questo punto chiamiamo la funzione s_rho:
//    
//    return s_rho (p_to_send,r_to_send);
//    
//};


//Funzione che gestisce i dati entranti per chiamare corretamente la funzione k_tau
//double compute_k_tau (const vector<pair<int,double> > & prevision,const vector<pair<int,double> > & real){
//    
//    //per poter non cambiare l'ordine dei vettori in entrata
//    vector<pair<int,double> > p = prevision;
//    vector<pair<int,double> > r = real;
//    
//    //ordina in base al rating (in ordine decrescente)
//    sort(p.begin(),p.end(),comparison_func_rate);
//    sort(r.begin(),r.end(),comparison_func_rate);
//    
//    /*   - per testare
//     //inizio test print (da cancellare fino alla prossima osservazione)
//     cout<<endl<<endl<<"Vectors ordered in rate: "<<endl;
//     
//     for (int i = 0; i < p.size(); i++) {
//     cout <<p[i].first<<"\t";
//     }
//     
//     cout<<endl;
//     
//     for (int i = 0; i < p.size(); i++) {
//     cout <<setprecision(4)<<p[i].second<<"\t";
//     }
//     
//     cout<<endl<<endl;
//     
//     for (int i = 0; i < r.size(); i++) {
//     cout <<r[i].first<<"\t";
//     }
//     
//     cout<<endl;
//     
//     for (int i = 0; i < r.size(); i++) {
//     cout <<setprecision(4)<<r[i].second<<"\t";
//     }
//     
//     cout<<endl<<endl;
//     //fine test
//     */
//    
//    //taglio gli elementi senza rate:
//    auto it = p.begin();
//    auto it2 = r.begin();
//    
//    while(it != p.end() && it->second != -1) {
//        it++;
//    }
//    
//    while(it2 != r.end() && it2->second != -1) {
//        it2++;
//    }
//    
//    if (it != p.end())
//        p.erase(it,p.end());
//    
//    
//    if (it2 != r.end())
//        r.erase(it2,r.end());
//    
//    /*
//     //inizio test print (da cancellare fino alla prossima osservazione)
//     cout<<endl<<endl<<"Vectors ordered in rate, senza unrated: "<<endl;
//     
//     for (int i = 0; i < p.size(); i++) {
//     cout <<p[i].first<<"\t";
//     }
//     
//     cout<<endl;
//     
//     for (int i = 0; i < p.size(); i++) {
//     cout <<setprecision(4)<<p[i].second<<"\t";
//     }
//     
//     cout<<endl<<endl;
//     
//     for (int i = 0; i < r.size(); i++) {
//     cout <<r[i].first<<"\t";
//     }
//     
//     cout<<endl;
//     
//     for (int i = 0; i < r.size(); i++) {
//     cout <<setprecision(4)<<r[i].second<<"\t";
//     }
//     
//     cout<<endl<<endl;
//     //fine test
//     */
//    
//    
//    //in questi metto per ogni item il suo corrisondente rank
//    vector<pair<int,int> > ranked_rated_1;
//    vector<pair<int,int> > ranked_rated_2;
//    
//    int rank = 1;
//    
//    for (int i = 0; i < p.size(); i++){
//        //Garantire che è strettamente minore (perché per ratting uguali il rank dev'essere uguale)
//        if (i != 0) {
//            if (p[i].second < p[i-1].second) {
//                rank++;
//            }
//        }
//        ranked_rated_1.push_back(make_pair(p[i].first,rank));
//        
//    }
//    
//    rank = 1;
//    
//    for (int i = 0; i < r.size(); i++){
//        
//        if (i != 0) {
//            if (r[i].second < r[i-1].second) {
//                rank++;
//            }
//        }
//        
//        ranked_rated_2.push_back(make_pair(r[i].first,rank));
//    }
//    
//    /*
//     //inizio test print (da cancellare fino alla prossima osservazione)
//     cout<<endl<<endl<<"Vectors ordered in rate, senza unrated e con rank: "<<endl;
//     
//     for (int i = 0; i < ranked_rated_1.size(); i++) {
//     cout <<ranked_rated_1[i].first<<"\t";
//     }
//     
//     cout<<endl;
//     
//     for (int i = 0; i < ranked_rated_1.size(); i++) {
//     cout <<setprecision(4)<<ranked_rated_1[i].second<<"\t";
//     }
//     
//     cout<<endl<<endl;
//     
//     for (int i = 0; i < ranked_rated_2.size(); i++) {
//     cout <<ranked_rated_2[i].first<<"\t";
//     }
//     
//     cout<<endl;
//     
//     for (int i = 0; i < ranked_rated_2.size(); i++) {
//     cout <<setprecision(4)<<ranked_rated_2[i].second<<"\t";
//     }
//     
//     cout<<endl<<endl;
//     //fine test
//     */
//    
//    //ordinando i vettori in ordine di item
//    sort(ranked_rated_1.begin(),ranked_rated_1.end(),comparison_func_item);
//    sort(ranked_rated_2.begin(),ranked_rated_2.end(),comparison_func_item);
//    
//    //l'item minimo e massimo che hanno possibilità di essere stati valutati da entrambi classifiche
//    int it_min = max(ranked_rated_2[0].first,ranked_rated_1[0].first);
//    int it_max = min(ranked_rated_1.back().first,ranked_rated_2.back().first);
//    
//    //Questi vettori conterrano i rank degli item valutati in entrambi i vettori con ogni indice rappresentando un item (ci saranno solo item valutati realmente
//    //e previsti)
//    vector<int> p_to_send;
//    vector<int> r_to_send;
//    
//    int j1=0;
//    int j2=0;
//    
//    
//    /*
//     In questo ciclo l'idea è andare a valutare nei vettori ordinati per item, qualli sono gli elementi che sono valutati (presenti) in entrambi vettori
//     si noti che abbiamo già escluso tutti gli elementi non valutati di ogni vettore ora bisogna solo "filtrare" qualli sono gli item presenti in entrambi
//     Quindi l'idea è fare un ciclo sugli item (i) che possono essere presenti in entrambi vettori (sicuramente compresi tra it_min e it_max)
//     Quindi si procede valutando ogni elemento dei vettori:
//     Es.:
//     v1 = (1,5),(3,3),(4,2),(7,4),(10,6),(12,1)
//     v2 = (2,1),(3,2),(5,3),(7,4)
//     Mio ciclo "for" andrà da 2 a 7 (che sono gli elementi possibilmente present in entrambi vettori)
//     quindi con i contatori j1 e j2 partendo da 0 verifico ongi elemento dentro il ciclo while:
//     1ºloop while: (1 ==) ranked_rated_1[j1].first < 2 &&  (2 ==) ranked_rated_2[j2].first == 2 (entro nel loop) e ricado nel 4º caso
//     2ºloop while: (3 ==) ranked_rated_1[j1].first > 2 &&  (2 ==) ranked_rated_2[j2].first == 2 (non entro nel loop) -> i++ (i == 3)
//     1ºloop while: (3 ==) ranked_rated_1[j1].first == 3 &&  (2 ==) ranked_rated_2[j2].first < 3 (entro nel loop nel 3º caso)
//     2ºloop while: (3 ==) ranked_rated_1[j1].first == 3 &&  (3 ==) ranked_rated_2[j2].first == 3 (entro nel loop nel 1º caso e includo nei miei vettori da spedire
//     il rank del vettore predetto e del vettore reale)
//     
//     Continuando il raggionamento i miei vettori risultanti saranno:
//     p_to_send = 3,4;
//     r_to_send = 2,4;
//     */
//    
//    for (int i = it_min; i <= it_max; i++) {
//        
//        while (ranked_rated_1[j1].first <= i && ranked_rated_2[j2].first <= i) {
//            
//            
//            if (ranked_rated_1[j1].first == i && ranked_rated_2[j2].first == i) {
//                p_to_send.push_back(ranked_rated_1[j1].second);
//                r_to_send.push_back(ranked_rated_2[j2].second);
//                j1++;
//                j2++;
//            }
//            
//            else if (ranked_rated_1[j1].first < i && ranked_rated_2[j2].first < i){
//                j1++;
//                j2++;
//            }
//            
//            else if (ranked_rated_1[j1].first == i && ranked_rated_2[j2].first < i){
//                j2++;
//            }
//            
//            else if (ranked_rated_1[j1].first < i && ranked_rated_2[j2].first == i){
//                j1++;
//            }
//            
//        }
//    }
//    
//    /*
//     //inizio test print (da cancellare fino alla prossima osservazione)
//     cout<<endl<<endl<<"Vectors sent to func : "<<endl;
//     
//     for (int i = 0; i < p_to_send.size(); i++) {
//     cout <<p_to_send[i]<<"\t";
//     }
//     
//     cout<<endl;
//     
//     
//     
//     for (int i = 0; i < r_to_send.size(); i++) {
//     cout <<r_to_send[i]<<"\t";
//     }
//     
//     
//     cout<<endl<<endl;
//     //fine test
//     */
//    
//    //A questo punto chiamiamo la funzione s_rho:
//    
//    return k_tau (p_to_send,r_to_send);
//    
//};


//Il vettore popularity contiene per ogni item il numero di uttenti che lo ha valutato
//Importante nottare che l'indice popularity[0] è il numero di uttenti che ha valutato l'item 1 e così via
//FUNZIONE FUNZIONANTE - (piccola differenza rispetto a MATLAB perchè ho messo <=N e non solo < N)  
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

//Più veloce se si passa solo gli elementi predetti che sono anche in real
//SEMBRA  FUNZIONANTE
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
    
    //A questo punto i vettori p_to_send e r_to_send sono ordinati in item ma non gli è stato ancora assegnato un rank e a
    //questo proposito chiamo la funzione orderProfile() che assegna i rank e restituisce i vettori ordinati in item
    
    orderProfile(p_to_send,r_to_send);
    
    
    
    vector<double> p_to_send_2;
    vector<double> r_to_send_2;
    
    for (auto it = p_to_send.begin(); it != p_to_send.end(); it++) {
        p_to_send_2.push_back(it->second);
    }
    
    for (auto it = r_to_send.begin(); it != r_to_send.end(); it++) {
        r_to_send_2.push_back(it->second);
    }
    
    return k_tau (p_to_send_2,r_to_send_2);
    
};



#endif /* defined(____RMS_testing__) */
