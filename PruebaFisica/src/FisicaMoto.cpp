/*
 * FisicaMoto.cpp
 *
 *  Created on: 13 de may. de 2016
 *      Author: Guillermo
 */

#include "FisicaMoto.h"




using namespace math;

// clase FisicaMoto

FisicaMoto::FisicaMoto() {
	// TODO Auto-generated constructor stub
	this->ruedas = new FisicaRueda[2];
}

FisicaMoto::~FisicaMoto() {
	// TODO Auto-generated destructor stub
}



FisicaRueda::FisicaRueda(){
	radioRueda =0;
	usarSoloBox = false;
}



FisicaRueda::~FisicaRueda(){

}


bool FisicaRueda::detectarColision(AABB obs){
	if (this->boxRueda.Intersects(obs)){
		return usarSoloBox || this->centroRueda.Distance(obs) < this->radioRueda;
	}
	return false;
}

bool FisicaRueda::detectarColision(OBB obs){
	if (this->boxRueda.Intersects(obs)){
		return usarSoloBox || this->centroRueda.Distance(obs) < this->radioRueda;
	}
	return false;
}

bool FisicaRueda::detectarColision(Triangle obs){
	if (this->boxRueda.Intersects(obs)){
		return usarSoloBox || this->centroRueda.Distance(obs) < this->radioRueda;
	}
	return false;
}





