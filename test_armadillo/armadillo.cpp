#include <armadillo>
#include <cstdio>

int main(int , char **) {
    
    //création d'une matrice de dimension 3 *3 aléaroire
    arma::mat A = arma::randu<arma::mat>(3, 3);

    A.print("matrice A aléatoire"); 


    // création matrice identité3 

    arma::mat B = arma::eye<arma::mat>(3 ,3) ;

    B.print("matrice identité 3");

    arma::mat Z = A + B ;
    printf("\nla somme est égale à  : \n");

    Z.print("matrice Z :");

    
}