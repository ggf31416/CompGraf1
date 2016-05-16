/*
 * FisicaRueda.h
 *
 *  Created on: 16 de may. de 2016
 *      Author: Guillermo
 */

#ifndef FISICARUEDA_H_
#define FISICARUEDA_H_

#include "MatGeoLib/Geometry/AABB.h"
#include "MatGeoLib/Geometry/OBB.h"
#include "MatGeoLib/Geometry/Triangle.h"

class FisicaRueda{
	public:
		FisicaRueda();
		FisicaRueda(float3 centroRueda,float radioRueda)
		virtual ~FisicaRueda();
		bool detectarColision(math::AABB obs);
		bool detectarColision(math::OBB obs);
		bool detectarColision(math::Triangle obs);
		void transladar(float3 vec);


		bool usarSoloBox;


		math::OBB boxRueda;
		math::float3 centroRueda;
		float radioRueda;


	private:


};

#endif /* FISICARUEDA_H_ */
