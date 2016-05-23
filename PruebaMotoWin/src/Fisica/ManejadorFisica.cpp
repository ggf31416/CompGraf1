/*
 * ManejadorFisica.cpp
 *
 *  Created on: 22/5/2016
 *      Author: Admin
 */

#include "ManejadorFisica.h"
#include "Gravedad.h"

using namespace math;
using namespace fisica;

ManejadorFisica::ManejadorFisica() {
	// TODO Auto-generated constructor stub
	this->objetos = new std::vector<ObjetoFisico*>();

}

ManejadorFisica::~ManejadorFisica() {
	// TODO Auto-generated destructor stub
}

math::float3 toFloat3(float* arr, unsigned int idx){
	return float3(arr[idx],arr[idx+1],arr[idx+2]);
}

void ManejadorFisica::registrarRampaConObs(float* arr, unsigned char* arribaIdx, unsigned char* abajoIdx) {
	float3 arriba[4];
	float3 abajo[4];

	for(int i = 0; i < 4; i++) arriba[i] = toFloat3(arr,arribaIdx[i] * 3);
	for(int i = 0; i < 4; i++) abajo[i] = toFloat3(arr,abajoIdx[i] * 3);

	fisica::Rampa r =  Rampa(arriba,abajo);
	r.generarObjetos(this->objetos);
}

void ManejadorFisica::establecerGravedad(float acelHaciaAbajo){
	this->g.g = -acelHaciaAbajo;
}

void ManejadorFisica::registrarMoto(){

}

void ManejadorFisica::establecerPosicionMoto(float x, float y, float z){

}

bool ManejadorFisica::estaSobrePista(){

}

void ManejadorFisica::simular(float dt){

}
