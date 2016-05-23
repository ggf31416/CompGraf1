#ifndef TXYZ_H
#define TXYZ_H

#include "MatGeoLib/Math/float3.h"

class TXYZ
{
    public:
        TXYZ(double t,math::float3 xyz) {
            this->t = t;
            this->xyz = xyz;
        }
        double t;
        math::float3 xyz;
        virtual ~TXYZ() {}
    protected:
    private:

};

#endif // TXYZ_H
