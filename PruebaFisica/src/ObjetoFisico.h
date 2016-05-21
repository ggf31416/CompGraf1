/*
 * ObjetoFisico.h
 *
 *  Created on: 20 de may. de 2016
 *      Author: Guillermo
 */

#ifndef OBJETOFISICO_H_
#define OBJETOFISICO_H_

#include "MatGeoLib/Geometry/GeometryAll.h"
#include "FisicaMoto.h"


namespace test {

/*
 * Clase abstracta para representar un elemento que interactua con la moto,
 * ya sea un obstaculo o parte de la pista
 */


class ObjetoFisico {
public:
	ObjetoFisico();
	virtual ~ObjetoFisico();
	virtual bool esObstaculo() = 0;
	virtual math::AABB getAABB() = 0;
	virtual bool colisiona(FisicaMoto &fm){
		math::AABB aabb = getAABB();
		return fm.colisiona(aabb);
	}
	virtual bool colisiona(FisicaRueda &fr){
		math::AABB aabb = getAABB();
		return fr.colisiona(aabb);
	}

};

} /* namespace test */

#endif /* OBJETOFISICO_H_ */
