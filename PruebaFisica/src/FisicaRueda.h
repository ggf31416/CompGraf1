/*
 * FisicaRueda.h
 *
 *  Created on: 16 de may. de 2016
 *      Author: Guillermo
 */

#ifndef FISICARUEDA_H_
#define FISICARUEDA_H_

#include "MatGeoLib/Geometry/GeometryAll.h"


class FisicaRueda{
	public:
		FisicaRueda();
		FisicaRueda(math::float3 centroRueda,float radioRueda,float anchoRueda);
		virtual ~FisicaRueda();
		bool colisiona(const math::AABB &obs) ;
		bool colisiona(const math::OBB &obs) ;
		bool colisiona(const math::Triangle &obs) ;
		void transladar(math::float3 vec);


		bool usarSoloBox;


		math::OBB boxRueda;
		math::float3 centroRueda;
		float radioRueda;
		float anchoRueda;


	private:


};

#endif /* FISICARUEDA_H_ */
