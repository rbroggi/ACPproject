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




template <T>
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
    
public:
    
    //accedere agli elementi:
    //Non so s
    T operator[][](const unsigned int, const unsigned int);
    
    //clonare l'emento
    sparse_m & operator = (const sparse_m<T> & clone );
    
    
    
    
    
    
};




/*INPUT: I)matrice URM ;
         II)tre vettori vuoti (URM_training, URM_test, URM_validaion);
         III)Percentuale di training data (trainig data/All data in URM); 
         IV)Percentuale di validation set (validation data/test data)                       
 */

void split_data_ramdom (const sparse_m<T> & URM, sparse_m<T> & URM_training, sparse_m<T> & URM_test, sparse_m<T> & URM_validation, const double ratio_training, const double ratio_validation);

/*OUTPUT: Tre matrici: a)URM_training;
                       b)URM_test;
                       c)URM_URM_validation;
*/




template<T>
double RMS_recall ( const sparse_m<T> & URM_training, const sparse_m<T> & URM_test, const sparse_m<T> & URM_validaion, const vector<T> & predic);





#endif /* defined(____RMS_testing__) */
