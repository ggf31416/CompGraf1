/*
 * Rampa.h
 *
 *  Created on: 20 de may. de 2016
 *      Author: Guillermo
 */

#ifndef RAMPA_H_
#define RAMPA_H_

#include "ObjetoFisico.h"
#include "Pista.h"
#include <vector>

namespace fisica {

class Rampa{
public:
	Rampa(math::float3* superiores,math::float3 * inferiores );
	virtual ~Rampa();
	math::AABB getAABB();
	Pista getPista();
	void generarObjetos(std::vector<ObjetoFisico*> *lista);
private:
	math::float3 superior[];
	math::float3 inferior[];
	math::AABB boxInferior;
	math::AABB boxTrasero;
	Pista* pista;
};

} /* namespace test */

#endif /* RAMPA_H_ */
