/*
 * FisicaMoto.cpp
 *
 *  Created on: 13 de may. de 2016
 *      Author: Guillermo
 */

#include "FisicaMoto.h"
#include <GL/gl.h>
#include "MatGeoLib/Geometry/LineSegment.h"
#include <iostream>



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
	this->boxSuperior = base; // conviete el AABB en OBB
	this->ruedas = new FisicaRueda[2];
	FisicaRueda r0(float3(x_rueda0 * largo, radioRueda,profundidad / 2),radioRueda,anchoRueda);
	FisicaRueda r1(float3(x_rueda1 * largo, radioRueda,profundidad / 2),radioRueda,anchoRueda);
	this->ruedas[0] = r0;
	this->ruedas[1] = r1;
	actualizarBBox();
}

void FisicaMoto::trasladar(float3 vec){
	this->boxSuperior.Translate(vec);
	this->ruedas[0].transladar(vec);
	this->ruedas[1].transladar(vec);
	actualizarBBox();
}

void FisicaMoto::setPosicion(math::float3 p){
	this->trasladar(p - this->boundingBox.minPoint);
}

void FisicaMoto::rotar(float grados,math::float3 centro){
	Quat q = Quat::RotateZ(grados);
	trasladar(-centro);
	this->boxSuperior =  q * this->boxSuperior  ;
	this->ruedas[0].centroRueda = q * this->ruedas[0].centroRueda;
	this->ruedas[1].centroRueda = q * this->ruedas[1].centroRueda;
	trasladar(centro);


}


void establecerEjes(OBB& box,float3* ejes){
	box.axis[0] = ejes[0];
	box.axis[1] = ejes[1];
	box.axis[2] = ejes[2];
}
// TODO
void FisicaMoto::establecerDireccion(float3  adelante){
	float3 adelante_n = adelante.Normalized();
	Quat q = Quat::LookAt(float3::unitX,adelante_n,float3::unitZ,float3::unitZ);
	ejes[0] = (q * float3::unitX).Normalized();
	ejes[1] = (q * float3::unitY).Normalized();
	ejes[2] = (q * float3::unitZ).Normalized();
	establecerEjes(this->boxSuperior,this->ejes);
	//establecerEjes(this->ruedas[0].boxRueda,this->ejes);
	//establecerEjes(this->ruedas[1].boxRueda,this->ejes);
}

// devuelve vector adelante nuevo
float3 FisicaMoto::posicionarPorRuedas(math::float3 rueda0,math::float3 rueda1){
	float3 centroRuedas = ruedas[0].centroRueda + (ruedas[1].centroRueda - ruedas[0].centroRueda) / 2;
	float altoCentroBox = (boxSuperior.pos - centroRuedas).Length();
	float largoOriginal = (ruedas[1].centroRueda - ruedas[0].centroRueda).Length() / 2;
	float3 dir = rueda1 - rueda0;
	establecerDireccion(dir);
	centroRuedas =   rueda0 + (rueda1 - rueda0) / 2;
	boxSuperior.pos = centroRuedas + boxSuperior.axis[1] * altoCentroBox;
	this->ruedas[0].setCentro( centroRuedas - boxSuperior.axis[0] * largoOriginal);
	this->ruedas[1].setCentro( centroRuedas + boxSuperior.axis[0] * largoOriginal);
	return dir;
}



bool FisicaMoto::colisiona(const math::AABB &obs) {
	return this->boxSuperior.Intersects(obs);
}
bool FisicaMoto::colisiona(const math::OBB &obs) {
	return this->boxSuperior.Intersects(obs);
}
bool FisicaMoto::colisiona(const math::Triangle &obs) {
	return this->boxSuperior.Intersects(obs);
}

void FisicaMoto::dibujarBB(){
	glColor3f(0.0, 0.0, 1.0);
	glBegin(GL_LINES);
	math::OBB box = boxSuperior;
	for(int i = 0; i < box.NumEdges(); i++){
		math::LineSegment seg = box.Edge(i);
		glVertex3f(seg.a.x, seg.a.y, seg.a.z);
		glVertex3f(seg.b.x, seg.b.y, seg.b.z);
	}
	glColor3f(1.0, 0.0, 0.0);
	for(int i = 0; i < 3; i++){
		float3 p = box.pos + box.axis[i] * box.r[i];
		glVertex3f(box.pos.x, box.pos.y, box.pos.z);
		glVertex3f(p.x,p.y,p.z);
	}

	glEnd();
	glBegin(GL_POINTS);
		for(int i = 0; i < 2; i++){
			float3 r = ruedas[i].centroRueda;
			glVertex3f(r.x,r.y,r.z);
		}

	glEnd();
}







