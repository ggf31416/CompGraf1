/*
 * FisicaRueda.cpp
 *
 *  Created on: 16 de may. de 2016
 *      Author: Guillermo
 */

#include "FisicaRueda.h"

using namespace math;
/*
 * Fisica Rueda
 */

FisicaRueda::FisicaRueda(){
	radioRueda =0;
	usarSoloBox = false;
	anchoRueda = 0;
	actualizarBox();
}

FisicaRueda::FisicaRueda(float3 centroRueda,float radioRueda,float anchoRueda){
	this->centroRueda = centroRueda;
	this->radioRueda = radioRueda;
	this->anchoRueda = anchoRueda;
	this->usarSoloBox = false;
	actualizarBox();
}



FisicaRueda::~FisicaRueda(){

}

void FisicaRueda::actualizarBox(){
	this->boxRueda = AABB::FromCenterAndSize(this->centroRueda,float3(2 * this->radioRueda,2 * this->radioRueda,this->anchoRueda));
}

bool FisicaRueda::colisiona(const math::AABB &obs) {
	if (this->boxRueda.Intersects(obs)){
		return usarSoloBox || this->centroRueda.Distance(obs) < this->radioRueda;
	}
	return false;
}

bool FisicaRueda::colisiona(const math::OBB &obs) {
	if (this->boxRueda.Intersects(obs)){
		return usarSoloBox || this->centroRueda.Distance(obs) < this->radioRueda;
	}
	return false;
}

bool FisicaRueda::colisiona(const math::Triangle &obs) {
	if (this->boxRueda.Intersects(obs)){
		return usarSoloBox || this->centroRueda.Distance(obs) < this->radioRueda;
	}
	return false;
}

void FisicaRueda::transladar(float3 vec){
	this->centroRueda = centroRueda + vec;
	actualizarBox();
}






