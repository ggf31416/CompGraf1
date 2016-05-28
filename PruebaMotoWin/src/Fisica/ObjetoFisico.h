/*
 * ObjetoFisico.h
 *
 *  Created on: 20 de may. de 2016
 *      Author: Guillermo
 */

#ifndef OBJETOFISICO_H_
#define OBJETOFISICO_H_

#include "MatGeoLib/Geometry/AABB.h"
#include "MatGeoLib/Math/float3.h"
#include "FisicaMoto.h"


namespace fisica {

/*
 * Clase abstracta para representar un elemento que interactua con la moto,
 * ya sea un obstaculo o parte de la pista
 */


class ObjetoFisico {
public:
	ObjetoFisico();
	virtual ~ObjetoFisico();
	virtual bool esObstaculo() = 0;
	virtual math::AABB getAABB() const = 0;
	virtual bool colisiona(FisicaMoto &fm){
		math::AABB aabb = getAABB();
		return fm.colisiona(aabb);
	}
	virtual bool colisiona(FisicaRueda &fr){
		math::AABB aabb = getAABB();
		return fr.colisiona(aabb);
	}

	math::AABB BoundingAABB() const{
		return getAABB();
	}

	 bool Intersects(const AABB &q) const{
		 return getAABB().Intersects(q);
	 }

	 // retornar normal en el punto especificado de la superficie
	 // puede ignorar el parametro y
	 math::float3 getNormalAt(math::float3 pos){
		 return float3::unitY;
	 }




};

} /* namespace test */

#endif /* OBJETOFISICO_H_ */
