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



class FisicaMoto {
public:
	FisicaMoto();
	virtual ~FisicaMoto();
	void actualizarBBox();
	void setTamanio(float largo,float alto,float profundidad,float y_ruedas,float x_rueda0,float x_rueda1, float radioRueda);
	void setPosicion(math::float3 p);
	void trasladar(math::float3 vec);
	void establecerDireccion(math::float3 vec);
	bool colisiona(const math::AABB &obs) ;
	bool colisiona(const math::OBB &obs) ;
	bool colisiona(const math::Triangle &obs) ;
private:
	math::AABB boundingBox;
	math::OBB boxMayor;
	math::OBB boxSuperior;
	math::float3 ejes[];
	FisicaRueda* ruedas;



};



#endif /* FISICAMOTO_H_ */
