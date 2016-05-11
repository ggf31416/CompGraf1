/*
 * Gravedad.cpp
 *
 *  Created on: 8 de may. de 2016
 *      Author: Guillermo
 */

#include "Gravedad.h"

Gravedad::Gravedad() {
	// TODO Auto-generated constructor stub

}

Gravedad::~Gravedad() {
	// TODO Auto-generated destructor stub
}

// n es vector normal unitario
// acelPendiente = g - n * dot_product(n,g);
Vector3D Gravedad::AcelPendiente(const Vector3D& n){
	return 9.8 * Vector3D(n.x * n.y, n.y * n.y - 1, n.z * n.y );
}

// sin (acos (x)) = sqrt (1 - x*x) segun Wolfram Alpha
// se puede probar que modulo(AcelPendiente) == AcelPendienteModulo
float Gravedad::AcelPendienteModulo(const Vector3D& n){
	return 9.8 * sqrt(1 - n.y * n.y);
}




