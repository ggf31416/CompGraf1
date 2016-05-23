/*
 * ObjetoContainer.h
 *
 *  Created on: 23/5/2016
 *      Author: Admin
 */

#ifndef OBJETOCONTAINER_H_
#define OBJETOCONTAINER_H_

#include "ObjetoFisico.h"

class ObjetoContainer {
public:
	ObjetoContainer(fisica::ObjetoFisico* obj){
		this->obj = obj;
	}
	ObjetoContainer();
	virtual ~ObjetoContainer();

	math::AABB BoundingAABB() const{
		return obj->getAABB();
	}

	fisica::ObjetoFisico* obj;
};

#endif /* OBJETOCONTAINER_H_ */

