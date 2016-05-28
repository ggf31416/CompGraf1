/*
 * Gravedad.cpp
 *
 *  Created on: 8 de may. de 2016
 *      Author: Guillermo
 */

#include "Gravedad.h"

using namespace math;

Gravedad::Gravedad(float g) {
	// TODO Auto-generated constructor stub
	this->g = g;

}

Gravedad::~Gravedad() {
	// TODO Auto-generated destructor stub
}

// aceleracion por gravedad en pendiente
// n es vector normal unitario
// acelPendiente = g - n * dot_product(n,g);
math::float3 Gravedad::AcelPendiente(const math::float3& n){
	return -g * math::float3(n.x * n.y, n.y * n.y - 1, n.z * n.y );
}

// sin (acos (x)) = sqrt (1 - x*x) segun Wolfram Alpha
// se puede probar que modulo(AcelPendiente) == AcelPendienteModulo
float Gravedad::AcelPendienteModulo(const  math::float3& n){
	return Abs(g) * sqrt(1 - n.y * n.y);
}


math::float3 Gravedad::CaidaLibre(math::float3 pos_ini,math::float3 vel_ini, float dt){
    float3 mov_grav = float3(0,g * dt * dt / 2,0);
    float3 mov_vel = vel_ini * dt;
    float3 despl = mov_grav + mov_vel;
    return pos_ini + despl;
}

