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


struct Info_Rampa{
	float x_base;
	float y_base;
	float x_size;
	float z_size;
	float altura_x_min;
	float altura_x_max;

	void init(float x_base, float y_base, float x_size, float z_size,
		float altura_x_min, float altura_x_max){
		this->x_base = x_base;
		this->y_base = y_base;
		this->x_size = x_size;
		this->z_size = z_size;
		this->altura_x_min = altura_x_min;
		this->altura_x_max = altura_x_max;
	}
};


class Rampa{
public:
	Rampa(math::float3* superiores,math::float3 * inferiores );
	virtual ~Rampa();
	math::AABB getAABB();
	fisica::Pista getPista();
	void generarObjetos(std::vector<fisica::ObjetoFisico*> *lista);
	math::float3 superior[4];
	math::float3 inferior[4];
private:

	math::AABB boxInferior;
	math::AABB boxTrasero;
	fisica::Pista* pista;


};



#endif /* RAMPA_H_ */
