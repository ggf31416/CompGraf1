/*
 * FisicaMoto.h
 *
 *  Created on: 13 de may. de 2016
 *      Author: Guillermo
 */

#ifndef FISICAMOTO_H_
#define FISICAMOTO_H_

#include "FisicaRueda.h"
#include "MatGeoLib/Math/Quat.h"
#include "MatGeoLib/Geometry/AABB.h"
#include "MatGeoLib/Geometry/OBB.h"



class FisicaMoto {
public:
	FisicaMoto();
	virtual ~FisicaMoto();
	void actualizarBBox();
	void setTamanio(float largo,float alto,float profundidad,float y_ruedas,float x_rueda0,float x_rueda1, float radioRueda);
	void setPosicion(math::float3 p);
	void trasladar(math::float3 vec);
	void establecerDireccion(math::float3 vec);
	void rotar(float grados,math::float3 centro);

	bool colisiona(const math::AABB &obs) ;
	bool colisiona(const math::OBB &obs) ;
	bool colisiona(const math::Triangle &obs) ;
	bool colisiona(const math::Plane &obs) {
		return this->boundingBox.Intersects(obs);
	}
	bool colisiona(const math::Polygon &obs) {
		return this->boundingBox.Intersects(obs);
	}

	math::AABB boundingBox;


	math::float3 getPos(){

		return this->boxSuperior.PointInside(0.5,0.5,0);
	}
	float getX(){ return getPos().x;}
	float getY(){ return getPos().y;}
	float getZ(){ return getPos().z;}

	void setY(float y){
		math::float3 dif(0,y - getY(),0);
		trasladar(dif);
	}

	void setX(float x){
		math::float3 dif(x - getX(),0,0);
		trasladar(dif);
	}

	math::float3 posicionarPorRuedas(math::float3 rueda0,math::float3 rueda1);
	FisicaRueda* ruedas;
	math::OBB boxSuperior;


	void dibujarBB();

private:


	math::float3 ejes[];




};



#endif /* FISICAMOTO_H_ */
