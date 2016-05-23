#include "Simulador.h"

using namespace math;

Simulador:: Simulador(math::float3 ini,double delta_t)
{
    this->vel_inicial = ini;
    this->delta_t = delta_t;
    g = float3(0,-9.8,0);
}

std::vector<TXYZ>  Simulador::calcular(){
	float3 pos(0,0,0);
    //Vector3D vel_actual = this->vel_inicial;
    double t_final = 2 * vel_inicial.y / -g.y;
    double t = 0;
    std::vector<TXYZ> res;
    do{
        res.push_back(calcularPos(t));
        t += delta_t;
    } while(t < t_final);
    res.push_back(calcularPos(t_final));
    return res;
}

TXYZ Simulador::calcularPos(double t){
    float3 mov_grav = g * t * t / 2;
    float3 mov_vel = vel_inicial * t;
    float3 pos = mov_grav + mov_vel;
    return TXYZ(t,pos);
}

Simulador::~Simulador()
{
    //dtor
}
