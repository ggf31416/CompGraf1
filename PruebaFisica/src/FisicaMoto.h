/*
 * FisicaMoto.h
 *
 *  Created on: 13 de may. de 2016
 *      Author: Guillermo
 */

#ifndef FISICAMOTO_H_
#define FISICAMOTO_H_

#include "MatGeoLib/Geometry/AABB.h"
#include "MatGeoLib/Geometry/OBB.h"
#include "MatGeoLib/Geometry/Triangle.h"

class FisicaRueda{
	public:
		FisicaRueda();
		virtual ~FisicaRueda();
		bool detectarColision(math::AABB obs);
		bool detectarColision(math::OBB obs);
		bool detectarColision(math::Triangle obs);

		bool usarSoloBox;
	private:
		math::OBB boxRueda;
		math::float3 centroRueda;
		float radioRueda;
};


class FisicaMoto {
public:
	FisicaMoto();
	virtual ~FisicaMoto();
private:
	math::OBB boxMayor;
	math::OBB boxSuperior;
	FisicaRueda* ruedas;



};



#endif /* FISICAMOTO_H_ */
