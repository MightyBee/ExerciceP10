#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <initializer_list>
#include "Vecteur.h"
using namespace std;


/*##############################################################################
###                                                                          ###
###                      METHODES DE LA CLASSE Vecteur                       ###
###                                                                          ###
##############################################################################*/

//#############################  constructeurs  ##############################//
// construit un vecteur nul de dimension n et fait office de constructeur par defaut
Vecteur::Vecteur(const unsigned int& n, const double& val) : coord(n,val) {}

// construit un vecteur à partir d'une liste de double
Vecteur::Vecteur(const initializer_list<double>& liste) : coord(liste) {}


//##########################  opérateurs internes  ###########################//
// opérateurs de comparaison //
bool Vecteur::operator==(const Vecteur& v2) const{
	if(coord.size()!=v2.coord.size()){
		return false;
	} else {
		for(size_t i(0);i<coord.size();i++){
			if(coord[i]!=v2.coord[i]){return false;}
		}
		return true;
	}
}

bool Vecteur::operator!=(const Vecteur& v2) const{
	return not operator==(v2);
}


// retourne produit scalaire du vecteur courant avec un autre vecteur //
double Vecteur::operator*(const Vecteur& v2) const{
	if(coord.size()==v2.coord.size()){
		double retour(0.0);
		for(size_t i(0);i<coord.size();i++){
			retour+=coord[i]*v2.coord[i];
		}
		return retour;
	} else {
		Erreur err("dimension", "Vecteur::operator*(const Vecteur&)",
							 "Produit scalaire de deux vecteurs de dimension différente ("+to_string(coord.size())+" et "+to_string(v2.coord.size())+").");
		throw err;
	}
}

// addition d'un vecteur au vecteur courant //
Vecteur& Vecteur::operator+=(const Vecteur& v2){
	if(coord.size()==v2.coord.size()){
		for(size_t i(0);i<coord.size();i++){
			coord[i]+=v2.coord[i];
		}
		return *this;
	} else {
		Erreur err("dimension", "Vecteur::operator+=(const Vecteur&)",
							 "Addition/soustraction de deux vecteurs de dimension différente ("+to_string(coord.size())+" et "+to_string(v2.coord.size())+").");
		throw err;
	}
}

// soustraction d'un vecteur au vecteur courant //
Vecteur& Vecteur::operator-=(const Vecteur& v2){
	try{
		operator+=(-v2);
		return *this;
	}catch(Erreur err){
		err.set_fct("Vecteur::operator-=(const Vecteur&)");
		throw err;
	}
}

// multiplication du vecteur courant par un scalaire //
Vecteur& Vecteur::operator*=(const double& lambda){
	for(auto& el : coord){el*=lambda;}
	return *this;
}

// division du vecteur courant par un scalaire //
Vecteur& Vecteur::operator/=(const double& lambda){
	if(lambda!=0){
		operator*=(1.0/lambda);
		return *this;
	} else {
		Erreur err("division par 0", "Vecteur::operator/=(const double&)", "Division d'un vecteur par zéro.");
		throw err;
	}
}


//##############################  accesseurs  ################################//
const double& Vecteur::operator[](unsigned int i) const{
	if(i>=0 and i<coord.size()){
		return coord[i];
	}else{
		Erreur err("dimension", "Vecteur::operator[](unsigned int) const",
							 "L'indice de position fourni en argument ("+to_string(i)+") n'est pas valable (attendu : entier entre 0 et "+to_string(coord.size()-1)+", dim(Vecteur)="+to_string(coord.size())+").");
		throw err;
	}
}

// retourne la dimension du vecteur //
size_t Vecteur::taille() const{
	return coord.size();
}


//#############################  manipulateurs  ##############################//
// ajoute une dimension au vecteur courant, possibilité de specifier la valeur de la nouvelle composante (par défaut : 0) //
void Vecteur::augmente(double newCoord){
	if(possible()){coord.push_back(newCoord);} // possible() : polymorphique, empêche pour les Vecteur3D
}

// retourne s'il est possible d'ajouter une dimension au vecteur, toujours vrai pour un Vecteur (générique)
bool Vecteur::possible() const{
	return true;
}

// retourne une référence sur la n-ieme coordonnee du vecteur, permet de la modifier //
double& Vecteur::operator[](unsigned int i){
	if(i>=0 and i<coord.size()){
		return coord[i];
	}else{
		Erreur err("dimension", "Vecteur::operator[](unsigned int)",
							 "L'indice de position fourni en argument ("+to_string(i)+") n'est pas valable (attendu : entier entre 0 et "+to_string(coord.size()-1)+", dim(Vecteur)="+to_string(coord.size())+").");
		throw err;
	}
}


//###########################  autres opérations  ############################//
// affichage des coordonnees du vecteur //
ostream& Vecteur::affiche(ostream& sortie)const{
	//sortie << "( ";
	for(auto el : coord){
		sortie << el << " " ;
	}
	//sortie << ")";
	return sortie;
}

// retourne la norme du vecteur courant //
double Vecteur::norme() const{
	return sqrt(norme2());
}

// retourne la norme au carré du vecteur courant : c'est <v,v> //
double Vecteur::norme2() const{
	return (*this)*(*this); // on utilise la surcharge de l'operateur * (produit sclaire)
}



/*##############################################################################
###                                                                          ###
###                           OPÉRATEURS EXTERNES                            ###
###                                                                          ###
##############################################################################*/

// permet l'affichage standard : sortie << vecteur; //
ostream& operator<<(ostream& sortie, const Vecteur& v){
	return v.affiche(sortie);
}

// somme de deux vecteurs //
const Vecteur operator+(Vecteur v1, const Vecteur& v2){
	try{
		v1+=v2;
		return v1;
	}catch(Erreur err){
		err.set_fct("operator+(Vecteur, const Vecteur&)");
		throw err;
	}
}

// différence de deux vecteurs //
const Vecteur operator-(Vecteur v1, const Vecteur& v2){
	try{
		v1-=v2;
		return v1;
	}catch(Erreur err){
		err.set_fct("operator-(Vecteur, const Vecteur&)");
		throw err;
	}
}

// opposé d'un vecteur //
const Vecteur operator-(Vecteur v){
	v*=-1;
	return v;
}

// multiplication d'un vecteur par un scalaire, cas scal*vect //
const Vecteur operator*(const double& lambda, Vecteur v){
	v*=lambda;
	return v;
}

// multiplication d'un vecteur par un scalaire, cas vect*scal //
const Vecteur operator*(const Vecteur& v, const double& lambda){
	return lambda*v;
}

// division d'un vecteur par un scalaire //
const Vecteur operator/(Vecteur v, double lambda){
	try{
		v/=lambda;
		return v;
	}catch(Erreur err){
		err.set_fct("operator/(Vecteur, double)");
		throw err;
	}
}

// retourne le vecteur unitaire : v/||v||
const Vecteur operator~(Vecteur v){
	try{
		return v/v.norme();
	}catch(Erreur err){
		err.set_fct("operator~(Vecteur), i.e. vecteur unitaire");
		err.set_dscrpt("Le vecteur nul n'a pas de vecteur unitaire correspondant.");
		throw err;
	}
}

/*##############################################################################
###                                                                          ###
###                     METHODES DE LA CLASSE Vecteur3D                      ###
###                                                                          ###
##############################################################################*/

// constructeurs
Vecteur3D::Vecteur3D(double val) : Vecteur(3,val) {}

Vecteur3D::Vecteur3D(const Vecteur& v) : Vecteur(v) {
	if(v.taille()!=3){
		Erreur err("dimension", "Vecteur3D::Vecteur3D(const Vecteur&)",
							 "Construction d'un Vecteur3D à partir d'un Vecteur de dimension "+to_string(v.taille())+".");
		throw err;
	}
}

Vecteur3D::Vecteur3D(const double& x, const double& y, const double& z)
										: Vecteur({x,y,z}) {}


// retourne s'il est possible d'ajouter une dimension au vecteur
bool Vecteur3D::possible() const{
	if(coord.size()==2){
		return true;
	} else {
		return false;
	}
}

// retourne le produit vectoriel du vecteur courant avec un autre vecteur 3D //
Vecteur3D Vecteur3D::operator^(const Vecteur3D& v2) const{
	return Vecteur3D((coord[1]*v2.coord[2])-(coord[2]*v2.coord[1]),
									 (coord[2]*v2.coord[0])-(coord[0]*v2.coord[2]),
									 (coord[0]*v2.coord[1])-(coord[1]*v2.coord[0]));
}

Vecteur3D Vecteur3D::projXY() const{
	return Vecteur3D(x(),y(),0);
}

// retourne l'angle en valeur absolue entre deux vecteur //
double Vecteur3D::angle(Vecteur3D const& v2) const{
	double angle(0);
	Vecteur3D V1(~(*this));
	Vecteur3D V2(~v2);
	if(V1==-V2){angle=-M_PI;}
  else if(V1!=V2){
    angle=acos(V1*V2);
  }
	return angle;
}
