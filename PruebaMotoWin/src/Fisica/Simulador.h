#ifndef SIMULADOR_H
#define SIMULADOR_H

#include "MatGeoLib/Math/float3.h"
#include "TXYZ.h"
#include <vector>

class Simulador
{
    public:
        Simulador(math::float3 ini,double delta_t);
        std::vector<TXYZ>  calcular();
        virtual ~Simulador();
    protected:
    private:
        math::float3 vel_inicial;
        double delta_t;
        math::float3 g;
        TXYZ calcularPos(double t);
};
#endif // SIMULADOR_H
