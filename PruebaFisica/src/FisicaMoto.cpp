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
	delete[] this->ruedas;
}


void FisicaMoto::actualizarBBox(){
	this->boundingBox = AABB(this->boxSuperior);
	this->boundingBox.Enclose(this->ruedas[0].boxRueda);
	this->boundingBox.Enclose(this->ruedas[1].boxRueda);
}

// setea el tamaño y pone las ruedas en altura y = 0
/***
 * largo,alto,profundidad : box superior de la moto
 * y_ruedas: posicion vertical de las ruedas en relacion al borde inferior del box superior de la moto
 * x_rueda0: posicion rueda trasera
 * x_rueda1: posicion rueda delantera
 * Las ruedas quedan centradas e
 */
void FisicaMoto::setTamanio(float largo,float alto,float profundidad,float y_ruedas,float x_rueda0,float x_rueda1, float radioRueda){
	float anchoRueda = 0;
	float3 inicio = float3(0,radioRueda - y_ruedas,0);
	float3 boxSize = float3(largo,alto,profundidad);
	AABB base(inicio,inicio + boxSize);
	this->boxMayor = base; // conviete el AABB en OBB
	this->ruedas = new FisicaRueda[2];
	FisicaRueda r0(float3(x_rueda0, radioRueda,profundidad / 2),radioRueda,anchoRueda);
	FisicaRueda r1(float3(x_rueda1, radioRueda,profundidad / 2),radioRueda,anchoRueda);
	this->ruedas[0] = r0;
	this->ruedas[1] = r1;
	actualizarBBox();
}

void FisicaMoto::trasladar(float3 vec){
	this->boxMayor.Translate(vec);
	this->ruedas[0].transladar(vec);
	this->ruedas[1].transladar(vec);
}

// TODO
void FisicaMoto::establecerDireccion(float3 vec){
	float3 eje1 = vec.Normalized();
	//float3 eje2 = float3(-eje1.y)
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
	float3 abc = v1.Cross(v2);
	float d = -(abc.x * p1.x + abc.y * p1.y + abc.z * p1.z);
	// Ax + By + Cz = -D
	// y = (D - Ax - Cz) / B
	float y = (d - abc.x * x - abc.z * z);
	return y;
}







