#ifndef TXYZ_H
#define TXYZ_H

#include "Vector3D.h"

class TXYZ
{
    public:
        TXYZ(double t,Vector3D xyz) {
            this->t = t;
            this->xyz = xyz;
        }
        double t;
        Vector3D xyz;
        virtual ~TXYZ() {}
    protected:
    private:

};

#endif // TXYZ_H
