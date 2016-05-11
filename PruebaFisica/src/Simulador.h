#ifndef SIMULADOR_H
#define SIMULADOR_H

#include "Vector3D.h"
#include "TXYZ.h"
#include <vector>

class Simulador
{
    public:
        Simulador(Vector3D,double);
        std::vector<TXYZ>  calcular();
        virtual ~Simulador();
    protected:
    private:
        Vector3D vel_inicial;
        double delta_t;
        Vector3D g;
        TXYZ calcularPos(double t);
};
#endif // SIMULADOR_H
