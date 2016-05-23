#ifndef VECTOR3D_H
#define VECTOR3D_H

#include <math.h>

class Vector3D
{
    public:

        double x;
        double y;
        double z;
        double m;

        static const Vector3D x_unit;
        static const Vector3D y_unit;
        static const Vector3D z_unit;
        static const Vector3D g;


        ~Vector3D()
        {
            //dtor
        }

        Vector3D(){
        	this->x = 0;
        	this->y = 0;
        	this->z = 0;
        	this->m = 0;
        }

        Vector3D(double _x,double _y, double _z){
            this->x = _x;
            this->y = _y;
            this->z = _z;
            this->m = -1;
        }

        Vector3D(double _x,double _y, double _z, double modulo){
            this->x = _x;
            this->y = _y;
            this->z = _z;
            this->m = modulo;
        }



        Vector3D suma(Vector3D o){
            return Vector3D(this->x+o.x,this->y+o.y,this->z+o.z);
        }

        Vector3D& operator +=(const Vector3D& o){
            this->x += o.x; this->y += o.y; this->z += o.z;
            return *this;
        }



        friend Vector3D operator + (Vector3D izq,const Vector3D& der){
            izq += der;  return izq.ch();
        }

        Vector3D& operator -=(const Vector3D& o){
             this->x -= o.x;
             this->y -= o.y;
             this->z -= o.z;
             return this->ch();
        }

        friend Vector3D operator - (Vector3D izq,const Vector3D& der){
            izq -= der; return izq.ch();
        }

        Vector3D& operator *=(double esc){
            this->x *=esc; this->y *= esc; this->z *= esc; this->ch();
            return this->ch();
        }

        friend Vector3D operator * (Vector3D izq, double esc){
            izq *= esc; return izq.ch();
        }

        friend Vector3D operator * (double esc,Vector3D izq){
            return izq * esc;
        }

        friend Vector3D operator * (const Vector3D& a, const Vector3D& b){
            return Vector3D(a.y * b.z - a.z * b.y, a.x * b.z - a.z * b.x, a.x * b.y - a.y * b.x);
        }

        friend Vector3D operator / (Vector3D a,double esc){
            return Vector3D(a.x / esc, a.y / esc, a.z / esc);
        }

        double interno(const Vector3D& b) const{
            return this->x * b.x + this->y * b.y + this->z * b.z;
        }

        double mod2(){
            return x * x + y * y + z * z;
        }

        double mod(){
            if (m < 0 ) m =  sqrt(this->mod2());
            return m;
        }

        double angulo(Vector3D o){
            double t = this->interno(o) / (this->mod() * o.mod());
            return acos(t);
        }

        friend Vector3D operator -(const Vector3D& a){
            return a * -1;
        }

        Vector3D N()  {
            return *this / this->mod();
        }

        Vector3D simetrico(Vector3D b){
            Vector3D N = b.N();
            return N * (2 * N.interno(*this)) - *this;
        }



    protected:
        Vector3D& ch(){
                   this->m = -1;
                   return *this;
               }
    private:
};

#endif // VECTOR3D_H
