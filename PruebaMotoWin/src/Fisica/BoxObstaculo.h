/*
 * BoxObstaculo.h
 *
 *  Created on: 20 de may. de 2016
 *      Author: Guillermo
 */

#ifndef BOXOBSTACULO_H_
#define BOXOBSTACULO_H_

#include "ObjetoFisico.h"

namespace fisica {



class BoxObstaculo : public ObjetoFisico{
public:
	BoxObstaculo(math::AABB box) : ObjetoFisico(){
		this->aabb = box;
	}

	BoxObstaculo(): ObjetoFisico(){}
	virtual ~BoxObstaculo(){

	}
	bool esObstaculo(){
		return true;
	}
	 math::AABB getAABB() const{
		 return aabb;
	 }
	 math::AABB aabb;
};

} /* namespace test */

#endif /* BOXOBSTACULO_H_ */
