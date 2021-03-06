#include<iostream>
#include<fstream>
#include<iomanip>
#include<ctime>
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

    int N = 10;

    clock_t tot_t = clock();

    cout<<"CLOCKS_PER_SEC: "<<CLOCKS_PER_SEC<<endl;
    cout<<"For list of "<<N<<" elements: "<<endl<<endl ;


    ifstream myReadFile;



    //Prima parte: recall, precision, ARHR, ARP, DCG

    vector<data> v1;


    myReadFile.open("INPUT_recall.txt");

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

    cout<<"First part: "<<endl;

    cout<<"Sequential: "<<endl;

    cout<<"Recall is: "<<RMS_recall(v1,N)<<endl;

    cout<<"Precision is: "<<RMS_precision(v1,N)<<endl;

    cout<<"The ARHR is: "<<RMS_ARHR(v1,N)<<endl;

    cout<<"The ARP is: "<<RMS_ARP(v1,N)<<endl;

    cout<<"The DCG is: "<<RMS_DCG(v1,N)<<endl;

    cout<<"openMP directives: "<<endl;

    cout<<"Recall is: "<<RMS_recall_omp(v1,N)<<endl;

    cout<<"Precision is: "<<RMS_precision_omp(v1,N)<<endl;

    cout<<"The ARHR is: "<<RMS_ARHR_omp(v1,N)<<endl;

    cout<<"The ARP is: "<<RMS_ARP_omp(v1,N)<<endl;

    cout<<"The DCG is: "<<RMS_DCG_omp(v1,N)<<endl;

    cout<<"End first part."<<endl<<endl;

    //Per verificare quanto i metodi open mp sono più efficienti:
    vector<double> time_sec;
    vector<double> time_omp;

    cout<<endl<<"Those are the (cumulative) ratio that measures the difference of efficiency between OMP directives and sequential ones. Note that will be different between computers (more cores -> lower ratios): "<<endl;
    clock_t init_t = clock();
    for(int j = 1; j < 200; j++){
            double rec = RMS_recall(v1,j);
            double pr = RMS_precision(v1,j);
            double ARHR = RMS_ARHR(v1,j);
            double ARP = RMS_ARP(v1,j);
            double DCG = RMS_DCG(v1,j);

            time_sec.push_back(((double)(clock()-init_t))/CLOCKS_PER_SEC);
    }

    init_t = clock();
    for(int j = 1; j < 200; j++){
            double rec = RMS_recall_omp(v1,j);
            double pr = RMS_precision_omp(v1,j);
            double ARHR = RMS_ARHR_omp(v1,j);
            double ARP = RMS_ARP_omp(v1,j);
            double DCG = RMS_DCG_omp(v1,j);

            time_omp.push_back(((double)(clock()-init_t))/CLOCKS_PER_SEC);
    }

    for(int j = 1; j < time_sec.size();j++){
        if ( (time_sec[j] != 0) && (j%10 == 0 || j == (time_sec.size()-1)) )
            cout<<time_omp[j]/time_sec[j]<<"\t";
    }

    cout<<endl<<endl;
    //fine prima parte

    //Seconda parte: Fallout

    vector<data> v2;

    ifstream myReadFile2;

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

    myReadFile2.close();

    cout<<"Second part: "<<endl;

    cout<<"Fallout is: "<<RMS_fallout(v2,N)<<endl;

    cout<<"openMP fallout is: "<<RMS_fallout_omp(v2,N)<<endl;

    cout<<"End second part."<<endl<<endl;

    //fine seconda parte

    //Terza parte: ROC (dipende da parte 1 e 2)

    cout<<"Third part: "<<endl;

    cout<<"The area under the ROC curve is: "<<AUC_ROC(v1,v2)<<endl;
    draw_ROC(v1,v2);
    draw_ROC_m(v1,v2);

    cout<<"End third part."<<endl<<endl;
    //fine terza parte



    //Quarta parte: Novelity (dipende da parte 1)
    vector<int> popularity;

    ifstream myReadFile3;

    myReadFile3.open("popularity.txt");


    if (myReadFile3.is_open()) {
        while (!myReadFile3.eof()) {
            int it;

            myReadFile3 >>it;

            popularity.push_back(it);


        }
    }

    myReadFile3.close();

    cout<<"Fourth part: "<<endl;

    cout<<"Novelity is: "<<novelity(v1,popularity,N)<<endl;

    cout<<"End fourth part."<<endl<<endl;
    //fine quarta parte


    //Quinta parte: Spearman/Kendall

    vector<pair<int,double> > v3, v4;

    //Caso non abbiamo dati:
    //rand_vec(v3,v4,6);


    ifstream myReadFile4;

    myReadFile4.open("real.txt");


    if (myReadFile4.is_open()) {
        while (!myReadFile4.eof()) {
            int it;
            double rate;

            myReadFile4 >>it>>rate;


            v3.push_back(make_pair(it,rate));


        }
    }

    myReadFile4.close();

    ifstream myReadFile5;

    myReadFile5.open("predicted.txt");


    if (myReadFile5.is_open()) {
        while (!myReadFile5.eof()) {
            int it;
            double rate;

            myReadFile5 >>it>>rate;


            v4.push_back(make_pair(it,rate));


        }
    }

    myReadFile5.close();


    /*
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
    */

    cout<<"Fifth part: "<<endl;

    cout<<"Spearsman rho: "<<compute_s_rho(v4,v3)<<endl;

    cout<<"Kendau tau: "<<compute_k_tau(v4,v3)<<endl;


    cout<<"End fifth part."<<endl<<endl;


    //cout<<"Fallout is: "<<RMS_fallout_robust(v2,10)<<endl;

    //cout<<"Precision is: "<<RMS_precision (v1, 10)<<endl;

    //cout<<"Robust Precision is: "<<RMS_precision_robust (v1, 10)<<endl;

    cout<<endl<<endl<<"Total elapsed time: "<<((double)(clock()-tot_t))/CLOCKS_PER_SEC<<"s"<<endl;

    return 0;
}
