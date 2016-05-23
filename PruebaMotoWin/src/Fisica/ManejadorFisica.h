/*
 * ManejadorFisica.h
 *
 *  Created on: 22/5/2016
 *      Author: Admin
 */

#ifndef MANEJADORFISICA_H_
#define MANEJADORFISICA_H_

#include "Rampa.h"
#include "ObjetoFisico.h"
#include "ObjetoContainer.h"
#include "Gravedad.h"
#include "MatGeoLib/Geometry/KDTree.h"
#include <vector>

class ManejadorFisica {
public:


	ManejadorFisica();

	void registrarRampaConObs(float* arr,  unsigned char* arribaIdx, unsigned char* abajoIdx) ;

	void establecerGravedad(float acelHaciaAbajo);

	void registrarMoto();

	void establecerPosicionMoto(float x, float y, float z);

	bool estaSobrePista();

	void simular(float dt);

	void test();



	virtual ~ManejadorFisica();



private:
	//math::KdTree<AABB> arbol;
	void detectarColisionMoto();
	void detectarColision(AABB q,std::vector<fisica::ObjetoFisico*>& lst);
	std::vector<fisica::ObjetoFisico*>* objetos;
	FisicaMoto* fm;
	Gravedad g;
	bool choco;
	bool sobrePista;

};

#endif /* MANEJADORFISICA_H_ */
