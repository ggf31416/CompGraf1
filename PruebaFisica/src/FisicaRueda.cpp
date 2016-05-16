/*
 * FisicaRueda.cpp
 *
 *  Created on: 16 de may. de 2016
 *      Author: Guillermo
 */

#include "FisicaRueda.h"


/*
 * Fisica Rueda
 */

FisicaRueda::FisicaRueda(){
	radioRueda =0;
	usarSoloBox = false;
}

FisicaRueda::FisicaRueda(float3 centroRueda,float radioRueda,float anchoRueda){
	this->centroRueda =centroRueda;
	this->radioRueda = radioRueda;
	this->anchoRueda = anchoRueda;
	this->usarSoloBox = false;
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

void FisicaRueda::transladar(float3 vec){
	this->centroRueda = centroRueda + vec;
}






