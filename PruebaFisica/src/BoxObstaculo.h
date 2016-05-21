/*
 * BoxObstaculo.h
 *
 *  Created on: 20 de may. de 2016
 *      Author: Guillermo
 */

#ifndef BOXOBSTACULO_H_
#define BOXOBSTACULO_H_

#include "ObjetoFisico.h"

namespace test {



class BoxObstaculo : public ObjetoFisico{
public:
	BoxObstaculo(math::AABB box) : ObjetoFisico(){
		this->aabb = box;
	}
	virtual ~BoxObstaculo(){

	}
	bool esObstaculo(){
		return true;
	}
	 math::AABB getAABB(){
		 return aabb;
	 }
	 math::AABB aabb;
};

} /* namespace test */

#endif /* BOXOBSTACULO_H_ */
