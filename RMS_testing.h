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



using namespace std;




                /*    COSE DA DECIDERE
                 
                     Struttura dati della nostra URM (saparsa) - possibilità pensate:
                     
                         I) vector<double>: pro: semplice da scrivere i metodi;
                         contro: non rappresenta matrici sparse
                     
                     Strutture per matrici sparse:
                     oss.:  Ci sono un po' di strutture ottimali per rappresentare matrici sparse e l'ottimalità di una struttura rispetto al'altra
                     dipende delle operazioni che si vogliono compiere. Da quanto ho capito sono due cose che regolano la scelta di qualle
                     struttura usare:
                                 a) Se è importante eseguire operazioni matriciali e quindi poter accedere velocemente ai coefficienti;
                                 b) Se è importante eseguire modificazioni alle matrici (inserimenti di nuove posizioni, etc);
                     
                     Creddo che per noi è più importante b) perchè non dovremmo modificare le matrici ma sì, generalmente, valutare i suoi coefficienti
                     
                     
                         II) Usare Eigen: pro: facilità implementativa;
                         contro: - magari non ottimale perchè non dobbiamo modificare le nostre matrici;
                         - non funzionerebbe senza l'inclusione della Eigen
                         
                         III) Particolarmente quello che creddo sia il migliore: Yale format (vedi wikipedia)
                         Sarebbe una struttura con tre vettori - Ho iniziato a implementarla - poi cmq se non sei d'accordo cambiamo
                 
                 
                 */



// Oss.: Creo che se implementiamo la classe e le funzioni come templates non riusciremmo a fare le definizioni delle funzioni e metodi della classe in un'altro file. Dovremmo fare tutto qua se non mi sbaglio (ho avuto questo problema una volta quando ho fato la libreria degli alberi)

//Creddo che possiamo assumere che nell'ambito del nostro progetto (Recommended system) le matrici sparse con cui lavoreremo non hanno nè righe ne collone completamente vuote (corriponderebbero a users/item che non forniscono alcuna informazione e quindi impossibili da raccomandare)


template <class T>
class sparse_m {
    
private:
    
    /*
     Rappresentazione classica di matrice sparsa:
     
     A - vettore con i coeff. della matrice in ordine sinistra-destra - su-giù (prima riga, poi seconda, poi ...) (ha dimensione uguale a quella dei coefficienti non nulli)
     
     AI - vettore con indici della A che identificano la posizione in A dei primi coefficienti di ogni riga più un'ultimo campo uguale a A.size() + AI(0);
     
     AJ - vettore che contiene le collone dei coefficienti di A (ha la stessa dimensione di A)
     */
    
    vector<T> A;
    vector<unsigned int> AI;
    vector<unsigned int> AJ;
    unsigned int col_num;
    
public:
    
    //constructors:
    
    
    //Prende una matrice densa rappresentata da vector di vector - FUNZIONANTE
    sparse_m(const vector< vector<T> > & initialiser);
    
    //Prende una matrice rppresentata da un vettore con in più il numero di collone (quindi numero di righe= matrice.size/ncollone)
    sparse_m(const vector<T> & matrix, unsigned int num_coll);
    
    //Prende una matrice rappresentata da tre vettori con il primo essendo i coefficienti non nulli e il secondo e terzo i corrispettivi indici di riga e collona
    //Da osservare che assumiamo che non esistano né collone né righe completamente vuote
    sparse_m(const vector<T> & non_null, const vector<unsigned int> & row, const vector<unsigned int> & coll);
    
    
    
    
    
    //accedere agli elementi:
    //Per l-value - NON IMPLEMENTATA
    T & operator() (const unsigned int i, const unsigned int j);
    
    //Per r-value - NON FUNZIONANTE
    T operator() (const unsigned int i, const unsigned int j) const;
    
    
    //clonare l'emento - NON IMPLEMENTATA
    sparse_m & operator = (const sparse_m<T> & clone );
    
    
    //Stampa i tre vettori in riga A,AI,AJ - FUNZIONANTE
    void print(){
        
        for (auto it = A.begin(); it!=A.end(); it++)
            cout<< *it <<"\t";

        cout<<endl;
        
        for (auto it = AI.begin(); it!=AI.end(); it++)
            cout<< *it <<"\t";
        
        cout<<endl;
        
        for (auto it = AJ.begin(); it!=AJ.end(); it++)
            cout<< *it <<"\t";
        
        cout<<endl;
        
        
    }
    
    
};


//VERIFCARE IMPLEMENTAZIONE
template<class T>
sparse_m<T>::sparse_m(const vector<vector<T> > &initialiser){

    //numero di righe
    int n = initialiser.size();
    
    //numero di collone
    int m = initialiser[0].size();
    
    //primo parametro
    col_num = m;
    
    //Parametro transitorio
    
    vector<unsigned int> rows;
    
    for (int i = 0; i != n; i++) {
        for (int j = 0; j != m; j++) {
            if ((initialiser[i])[j] != 0) {
                A.push_back((initialiser[i])[j]);
                rows.push_back(i);
                AJ.push_back(j);
            }
        }
    }

    AI.push_back(0);
    
    for (int i = 1; i < rows.size() ; i++) {
        if (rows[i]!= rows[i-1]) {
            AI.push_back(i);
        }
    }

    AI.push_back(A.size());
}


template<class T>
T
sparse_m<T>::operator() (const unsigned int i, const unsigned int j) const {
    
    //Innanzitutto verifichiamo se l'elemento è zero poiché é la più grande possibilità:
    //In questo senso verifico se tra gli elementi della riga i (sono AI(i+1)- A(i)) qualcuno ha numer collona j
    unsigned int p;
    
    for(p = AI[i]; p < AI[i+1] && AJ[p] < j; p++)
        
    
    if (j== p)
        return A[p];
    
    else
        return 0;
    
    
}


/*INPUT: I)matrice URM ;
         II)tre vettori vuoti (URM_training, URM_test, URM_validaion);
         III)Percentuale di training data (trainig data/All data in URM); 
         IV)Percentuale di validation set (validation data/test data)                       
 */

template<class T>
void split_data_ramdom (const sparse_m<T> & URM, sparse_m<T> & URM_training, sparse_m<T> & URM_test, sparse_m<T> & URM_validation, const double ratio_training, const double ratio_validation);

/*OUTPUT: Tre matrici: a)URM_training;
                       b)URM_test;
                       c)URM_URM_validation;
*/




template<class T>
double RMS_recall ( const sparse_m<T> & URM_training, const sparse_m<T> & URM_test, const sparse_m<T> & URM_validaion, const vector<T> & predic);





#endif /* defined(____RMS_testing__) */
