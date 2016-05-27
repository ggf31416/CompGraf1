/*
 * Rampa.cpp
 *
 *  Created on: 20 de may. de 2016
 *      Author: Guillermo
 */

#include "Rampa.h"
#include "BoxObstaculo.h"
#include <iostream>

namespace fisica {

using namespace math;

Rampa::Rampa(float3* superiores,float3 * inferiores) {
	std::cout << "superiores: ";
	for(int i = 0; i < 4; i++){
			std::cout << i << " " << float3(superiores[i]) << ",";
	}
	std::cout << "\ninferiores: ";
	for(int i = 0; i < 4; i++){
			std::cout << i << " " << float3(inferiores[i]) << ",";
	}

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
	std::cout << "\n";
	for(int i = 0; i < 4; i++){
		this->inferior[i] = inferiores[i];
	}
	std::cout << "\n";



	std::cout << "superior: ";
		for(int i = 0; i < 4; i++){
				std::cout << i << " " << float3(superior[i]) << ",";
		}
		std::cout << "\n";
		std::cout << "inferior: ";
		for(int i = 0; i < 4; i++){
				std::cout << i << " " << float3(inferior[i]) << ",";
		}
		std::cout << "\n";
	float x_min = Min(inferior[0].x,inferior[1].x,inferior[2].x);
	std::cout << "x_min= " << x_min << "\n";
	float y_min = Min(inferior[0].y,inferior[1].y,inferior[2].y);
	std::cout << "y_min= " << y_min << "\n";
	float z_min = Min(inferior[0].z,inferior[1].z,inferior[2].z);
	std::cout << "z_min= " << z_min << "\n";

	float x_max = Max(inferior[0].x,inferior[1].x,inferior[2].x);
	std::cout << "x_max= " << x_max << "\n";
	float z_max = Max(inferior[0].z,inferior[1].z,inferior[2].z);
	std::cout << "z_max= " << z_max << "\n";

	float y_min_s = Min(superior[0].y,superior[1].y,superior[2].y,superior[3].y);
	std::cout << "y_min_s= " << y_min_s << "\n";
	std::cout << "v_alto= " << v_alto << "\n";

	float dif = y_max - y_min_s;

	float3 vec_min = float3(x_min,y_min,z_min);
	float3 vec_max = float3(x_max,y_max,z_max);
	float3 vec_med = float3(x_max,y_min_s,z_max);
	this->boxInferior = AABB(vec_min,vec_med);

	this->boxTrasero =  AABB(float3(v_alto.x,y_min,z_min),float3(v_alto.x,y_max,z_max));

	this->pista = new Pista( this->superior);

	std::cout << "inferior: " << this->boxInferior.ToString() << "\n";
	std::cout << "trasero: " << this->boxTrasero.ToString() << "\n";
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
