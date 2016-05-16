/*
 * FisicaMoto.h
 *
 *  Created on: 13 de may. de 2016
 *      Author: Guillermo
 */

#ifndef FISICAMOTO_H_
#define FISICAMOTO_H_

#include "FisicaRueda.h"



class FisicaMoto {
public:
	FisicaMoto();
	virtual ~FisicaMoto();
	void actualizarBBox();
	void setTamanio(float largo,float alto,float profundidad,float y_ruedas,float x_rueda0,float x_rueda1, float radioRueda);
	void setPosicion(float3 p);
private:
	math::AABB boundingBox;
	math::OBB boxMayor;
	math::OBB boxSuperior;
	FisicaRueda* ruedas;



};



#endif /* FISICAMOTO_H_ */
