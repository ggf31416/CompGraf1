/*
 * Rampa.cpp
 *
 *  Created on: 20 de may. de 2016
 *      Author: Guillermo
 */

#include "Rampa.h"
#include "BoxObstaculo.h"

namespace fisica {

using namespace math;

Rampa::Rampa(float3* superiores,float alturaMenor, float alturaMayor  ) {
	// TODO Auto-generated constructor stub
	for(int i = 0; i < 4; i++){
		this->superior[i] = superiores[i];
	}
	for(int i = 0; i < 2; i++){
		this->inferior[i] = float3(superior[i].x, superior[i].y - alturaMayor,superior[i].z);
	}
	for(int i = 2; i < 4; i++){
		this->inferior[i] = float3(superior[i].x, superior[i].y - alturaMayor,superior[i].z);
	}
	float dif = alturaMayor - alturaMenor;

	float3 puntosAbajo[4] = {inferior[0],inferior[1],inferior[2],superior[0]};
	this->boxInferior =  AABB::MinimalEnclosingAABB(puntosAbajo,4);

	float3 puntosAtras[2] = {superior[2],float3(superior[3].x,superior[3].y - dif, superior[3].z)};
	this->boxTrasero =  AABB::MinimalEnclosingAABB(puntosAtras,2);

	float3* s = this->superior;
	this->pista = new Pista(s);
}

void Rampa::generarObjetos(std::vector<ObjetoFisico*> *lista){
	BoxObstaculo* inf = new BoxObstaculo(this->boxInferior);
	BoxObstaculo* atras = new BoxObstaculo(this->boxTrasero);

	lista->push_back(this->pista);
	lista->push_back(inf);
	lista->push_back(atras);

}



Rampa::~Rampa() {
	// TODO Auto-generated destructor stub
}

} /* namespace test */
