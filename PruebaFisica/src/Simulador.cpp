#include "Simulador.h"


Simulador::Simulador(Vector3D vel_inicial,double delta_t)
{
    this->vel_inicial = vel_inicial;
    this->delta_t = delta_t;
    g = Vector3D(0,0,-9.8);
}

std::vector<TXYZ>  Simulador::calcular(){
    Vector3D pos(0,0,0);
    //Vector3D vel_actual = this->vel_inicial;
    double t_final = 2 * vel_inicial.z / -g.z;
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
    Vector3D mov_grav = g * t * t / 2;
    Vector3D mov_vel = vel_inicial * t;
    Vector3D pos = mov_grav + mov_vel;
    return TXYZ(t,pos);
}

Simulador::~Simulador()
{
    //dtor
}
