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
#include "MatGeoLib/Geometry/Plane.h"


class FisicaRueda{
	public:
		FisicaRueda();
		FisicaRueda(math::float3 centroRueda,float radioRueda,float anchoRueda);
		virtual ~FisicaRueda();
		bool colisiona(const math::AABB &obs) ;
		bool colisiona(const math::OBB &obs) ;
		bool colisiona(const math::Triangle &obs) ;
		void transladar(math::float3 vec);
		void setCentro(math::float3 vec){
			this->centroRueda = vec;
			actualizarBox();
		}

		void setY(float y){
			setCentro(float3(centroRueda.x,y,centroRueda.z));
		}

		bool colisiona(const math::Plane &obs) {
			return this->boxRueda.Intersects(obs) && this->centroRueda.Distance(obs) < this->radioRueda;
		}
		bool colisiona(const math::Polygon &obs) {
			return this->boxRueda.Intersects(obs);
		}

		math::float3 puntoMasCercano(const math::Plane &obs){
			return obs.ClosestPoint(this->centroRueda);
		}

		math::float3 puntoMasCercano(const math::AABB &obs){
			return obs.ClosestPoint(this->centroRueda);
		}

		math::float3 puntoMasCercano(const math::Triangle &obs){
			return obs.ClosestPoint(this->centroRueda);
		}

		math::float3 puntoMasCercano(const math::OBB &obs){
			return obs.ClosestPoint(this->centroRueda);
		}






		bool usarSoloBox;


		math::AABB boxRueda;
		math::float3 centroRueda;
		float radioRueda;
		float anchoRueda;


	private:
		void actualizarBox();

};

#endif /* FISICARUEDA_H_ */
