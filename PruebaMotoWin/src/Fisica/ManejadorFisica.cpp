/*
 * ManejadorFisica.cpp
 *
 *  Created on: 22/5/2016
 *      Author: Admin
 */

#include "ManejadorFisica.h"

#include "Gravedad.h"
#include <iostream>
#include "BoxObstaculo.h"
#include "Pista.h"

using namespace math;
using namespace fisica;



ManejadorFisica::ManejadorFisica() {
	// TODO Auto-generated constructor stub
	this->objetos = new std::vector<ObjetoFisico*>();
	this->fm = new FisicaMoto();

}

void ManejadorFisica::detectarColision(AABB q,std::vector<fisica::ObjetoFisico*>& lst){
	for(unsigned int i = 0; i  < this->objetos->size(); i++){

		ObjetoFisico* o = (*(this->objetos))[i];
		if (o->Intersects(q)){
			std::cout << "q=" << q<< "\n";
			std::cout << "Intersects q: " << o->BoundingAABB() << "\n";
			lst.push_back(o);
		}
	}

}

float3 calcularNormal(float3 p1,float3 p2,float3 p3,bool haciaArriba){
	float3 v1 = p2 - p1;
	float3 v2 = p3 - p1;
	float3 abc = v1.Cross(v2);
	float3 n = abc.Normalized();
	if (haciaArriba != (n.y >= 0)){
		return -n;
	}
	else{
		return n;
	}

}

float3 calcularNormalPista(Pista* p){
	float3* ptos = p->puntos;
	return calcularNormal(ptos[0],ptos[1],ptos[2],true);
}

float3 acelPendientePista(Pista* p,Gravedad& g){
	float3 n = calcularNormalPista(p);
	float3 acel = g.AcelPendiente(n);
	return acel;
}

void ManejadorFisica::detectarColisionMoto(){
	std::vector<fisica::ObjetoFisico*> tmp;
	detectarColision(fm->boundingBox,tmp);
	sobrePista =false;
	colisiono = false;
	for(unsigned int i = 0; i < tmp.size(); i++){

		ObjetoFisico* obj = tmp[i];
		if (obj->colisiona(*fm)){
			std::cout << "BBox obj: " << obj->BoundingAABB() << "\n";
			if (!obj->esObstaculo()){
				sobrePista = true;
				Pista* pista = dynamic_cast<Pista*>(obj);
				if (pista){
					float3 acelg = acelPendientePista(pista,this->g);
				}
			}
			else{
				colisiono = true;
			}
		}
		colisiono &= !sobrePista;
	}
}

void ManejadorFisica::test(){
	AABB aabbBox = AABB(float3(1,1,1),float3(2,3,4));
	ObjetoFisico* b[2];
	b[0] = new BoxObstaculo(aabbBox);
	b[1] = new BoxObstaculo(AABB(float3(0,0,0),float3(0.5,0.9,0.9)));
	this->objetos->push_back(b[0]);
	this->objetos->push_back(b[1]);


	std::vector<fisica::ObjetoFisico*> lst ;
	AABB aabb1(float3(9,8,5),float3(10,10,10));
	detectarColision(aabb1,lst);
	std::cout << "Size:  " << lst.size() << "\n";
	AABB aabb2(float3(1.5,2.5,3.5),float3(5,5,5));
	detectarColision(aabb2,lst);
	std::cout << "Size:  " << lst.size() << "\n";
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


void ManejadorFisica::registrarRampaConObs(float3* arriba, float3* abajo) {
	fisica::Rampa r =  Rampa(arriba,abajo);
	r.generarObjetos(this->objetos);
}

void ManejadorFisica::establecerGravedad(float acelHaciaAbajo){
	this->g.g = -acelHaciaAbajo;
}

void ManejadorFisica::registrarMoto(){

}

void ManejadorFisica::establecerPosicionMoto(float x, float y, float z){
	float3 p(x,y,z);
	fm->setPosicion(p);
}

bool ManejadorFisica::estaSobrePista(){
	return sobrePista;
}

void ManejadorFisica::simular(float dt){
	detectarColisionMoto();

}


