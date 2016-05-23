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
#include "Gravedad.h"
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

	virtual ~ManejadorFisica();

private:
	std::vector<fisica::ObjetoFisico*>* objetos;
	Gravedad g;
	bool choco;
	bool sobrePista;
};

#endif /* MANEJADORFISICA_H_ */
