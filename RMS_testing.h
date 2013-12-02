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
                      Sarebbe una struttura con tre vettori  


                */




/*INPUT: I)matrice URM ;
         II)tre vettori vuoti (URM_training, URM_test, URM_validaion);
         III)Percentuale di training data (trainig data/All data in URM); 
         IV)Percentuale di validation set (validation data/test data)                       
 */

void split_data_ramdom (const vector<double> & URM, vector<double> & URM_training, vector<double> & URM_test, vector<double> & URM_validation, const double ratio_training, const double ratio_validation);

/*OUTPUT: Tre matrici: a)URM_training;
                       b)URM_test;
                       c)URM_URM_validation;
*/





double RMS_recall ( const vector<double> & URM_training, const vector<double> & URM_test, const vector<double> & URM_validaion, const vector<double> & predic);






#endif /* defined(____RMS_testing__) */
