/*
 * ManejadorFisica.h
 *
 *  Created on: 22/5/2016
 *      Author: Admin
 */

#ifndef MANEJADORFISICA_H_
#define MANEJADORFISICA_H_

#include "Rampa.h"
#include "Pista.h"
#include "ObjetoFisico.h"
#include "ObjetoContainer.h"
#include "Gravedad.h"
#include "MatGeoLib/Geometry/KDTree.h"
#include "FisicaMoto.h"
#include <vector>

class ManejadorFisica {
public:


	ManejadorFisica();

	void registrarRampaConObs(float* arr,  unsigned char* arribaIdx, unsigned char* abajoIdx) ;

	void registrarRampaConObs(math::float3* arriba, math::float3* abajo);

	void setTamanio(float largo,float alto,float profundidad,float y_ruedas,float x_rueda0,float x_rueda1, float radioRueda){
		fm->setTamanio(largo,alto,profundidad,y_ruedas,x_rueda0,x_rueda1,radioRueda);
	}


	void establecerGravedad(float acelHaciaAbajo);

	void registrarMoto();

	void establecerPosicionMoto(float x, float y, float z);

	bool estaSobrePista();



	void simular(float dt);

	void test();



	virtual ~ManejadorFisica();

	bool colisiono;
	bool sobrePista;
	float getAltura();

private:
	//math::KdTree<AABB> arbol;
	void detectarColisionMoto();
	void detectarColision(AABB q,std::vector<fisica::ObjetoFisico*>& lst);
	std::vector<fisica::ObjetoFisico*>* objetos;
	FisicaMoto* fm;
	Gravedad g;




	fisica::Pista* pistaActual;

};

#endif /* MANEJADORFISICA_H_ */
