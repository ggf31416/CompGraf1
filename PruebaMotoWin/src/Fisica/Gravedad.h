/*
 * Gravedad.h
 *
 *  Created on: 8 de may. de 2016
 *      Author: Guillermo
 */

#ifndef GRAVEDAD_H_
#define GRAVEDAD_H_

#include "MatGeoLib/Math/float3.h"

class Gravedad {
public:
	Gravedad(){
		g = -9.8;
	}
	Gravedad(float g);
	virtual ~Gravedad();

	math::float3 AcelPendiente(const math::float3 &normal_unit);
	float AcelPendienteModulo(const math::float3& normal_unit);

	// calcula posicion exacta a los dt segundos de iniciado el vuelo
	math::float3 CaidaLibre(math::float3 pos_ini,math::float3 vel_ini, float dt);



	float g;
};

#endif /* GRAVEDAD_H_ */
