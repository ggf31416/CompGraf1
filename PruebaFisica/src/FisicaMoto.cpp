/*
 * FisicaMoto.cpp
 *
 *  Created on: 13 de may. de 2016
 *      Author: Guillermo
 */

#include "FisicaMoto.h"
#include "Vector3D.h"




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


// calcular altura de un punto dentro de un triangulo
// http://stackoverflow.com/questions/5507762/how-to-find-z-by-arbitrary-x-y-coordinates-within-triangle-if-you-have-triangle
float calcY(Triangle t, float x, float z) {
		vec p1 = t.a;
		vec p2 = t.b;
		vec p3 = t.c;
        float det = (p2.z - p3.z) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.z - p3.z);

        float l1 = ((p2.z - p3.z) * (x - p3.x) + (p3.x - p2.x) * (z - p3.z)) / det;
        float l2 = ((p3.z - p1.z) * (x - p3.x) + (p1.x - p3.x) * (z - p3.z)) / det;
        float l3 = 1.0f - l1 - l2;

        return l1 * p1.y + l2 * p2.y + l3 * p3.y;
}

// altura a partir de 3 puntos del plano
float calcY(float3 p1, float3 p2, float3 p3, float x, float z){
	float3 v1 = p2 - p1;
	float3 v2 = p3 - p1;
	float3 abc = v1.Dot(v2);
	float d = -abc.Dot(p1);
	// Ax + By + Cz = -D
	// y = (D - Ax - Cz) / B
	float y = (d - abc.x * x - abc.z * z);
	return z;
}






