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

Rampa::Rampa(float3* superiores,float3 * inferiores) {


	// TODO Auto-generated constructor stub
	this->superior[0] = superiores[0];
	float3 v_alto = superiores[0];
	float y_max = v_alto.y;


	for(int i = 1; i < 4; i++){
		this->superior[i] = superiores[i];
		if (superiores[i].y > y_max){
			y_max = superiores[i].y;
			v_alto = superiores[i];
		}
	}
	for(int i = 0; i < 4; i++){
		this->inferior[i] = inferiores[i];
	}



	float x_min = Min(inferior[0].x,inferior[1].x,inferior[2].x);
	float y_min = Min(inferior[0].y,inferior[1].y,inferior[2].y);
	float z_min = Min(inferior[0].z,inferior[1].z,inferior[2].z);

	float x_max = Max(inferior[0].x,inferior[1].x,inferior[2].x);
	float z_max = Max(inferior[0].z,inferior[1].z,inferior[2].z);


	float y_min_s = Min(superior[0].y,superior[2].y);

	float dif = y_max - y_min_s;

	float3 vec_min = float3(x_min,y_min,z_min);
	float3 vec_max = float3(x_max,y_max,z_max);
	float3 vec_med = float3(x_max,y_min_s,z_max);
	this->boxInferior = AABB(vec_min,vec_med);

	this->boxTrasero =  AABB(float3(v_alto.x,z_max,y_max),float3(v_alto.x,z_min,y_min_s));

	this->pista = new Pista( this->superior);
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
