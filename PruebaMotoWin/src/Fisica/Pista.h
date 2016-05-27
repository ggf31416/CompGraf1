/*
 * Pista.h
 *
 *  Created on: 20 de may. de 2016
 *      Author: Guillermo
 */

#ifndef PISTA_H_
#define PISTA_H_

#include "MatGeoLib/Geometry/Plane.h"
#include "ObjetoFisico.h"
#include "MatGeoLib/Geometry/AABB.h"

namespace fisica {


class Pista : public ObjetoFisico {
public:
	math::Plane plano;

	Pista(math::float3 *quad) : ObjetoFisico(){
		for(int i = 0; i < 4 ; i++){
			this->puntos[i] = quad[i];
		}
		aabb = math::AABB::MinimalEnclosingAABB(quad,4);
		plano = math::Plane(quad[0],quad[1],quad[2]);
	}
	virtual ~Pista(){

	}

	virtual bool colisiona(FisicaMoto &fm){
		math::AABB aabb = getAABB();
		return fm.colisiona(aabb) && fm.colisiona(plano);
	}
	virtual bool colisiona(FisicaRueda &fr){
		math::AABB aabb = getAABB();
		return fr.colisiona(aabb) && fr.colisiona(plano);
	}

	virtual bool esObstaculo(){
		return false;
	}
	math::AABB getAABB() const {
		return aabb;
	}
	math::float3 puntos[4];

private:

	math::AABB aabb;
};
} /* namespace test */

#endif /* PISTA_H_ */
