/*
 * Gravedad.h
 *
 *  Created on: 8 de may. de 2016
 *      Author: Guillermo
 */

#ifndef GRAVEDAD_H_
#define GRAVEDAD_H_

#include "Vector3D.h"

class Gravedad {
public:

	Gravedad();
	virtual ~Gravedad();

	Vector3D AcelPendiente(const Vector3D& normal_unit);
	float AcelPendienteModulo(const Vector3D& normal_unit);

	float distanciaPuntoPlano();
};

#endif /* GRAVEDAD_H_ */
