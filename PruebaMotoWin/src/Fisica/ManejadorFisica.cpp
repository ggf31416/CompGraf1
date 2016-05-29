/*
 * ManejadorFisica.cpp
 *
 *  Created on: 22/5/2016
 *      Author: Admin
 */

#include "ManejadorFisica.h"

#include "Gravedad.h"
#include <iostream>
#include "BoxObstaculo.h"
#include "Pista.h"


using namespace math;
using namespace fisica;


void checkearCalcY();
float calcY(const math::Plane& plano, float x, float z);

ManejadorFisica::ManejadorFisica() {
	// TODO Auto-generated constructor stub
	this->objetos = new std::vector<ObjetoFisico*>();
	this->fm = new FisicaMoto();
	this->colisiono = false;
	this->sobrePista =false;
	this->pistaActual = 0;
	this->aceleracionPropia = 0;
	this->direccionMoto = float3::unitX;
	this->velMoto = float3::zero;
	//checkearCalcY();
}

void ManejadorFisica::detectarColision(AABB q,std::vector<fisica::ObjetoFisico*>& lst){
	for(unsigned int i = 0; i  < this->objetos->size(); i++){

		ObjetoFisico* o = (*(this->objetos))[i];
		if (o->Intersects(q)){
			//std::cout << "q=" << q<< "\n";
			//std::cout << "Intersects q: " << o->BoundingAABB() << "\n";
			lst.push_back(o);
		}
	}

}

float3 calcularNormal(float3 p1,float3 p2,float3 p3,bool haciaArriba){
	float3 v1 = p2 - p1;
	float3 v2 = p3 - p1;
	float3 abc = v1.Cross(v2);
	float3 n = abc.Normalized();
	if (haciaArriba != (n.y >= 0)){
		return -n;
	}
	else{
		return n;
	}

}

float3 calcularNormalPista(Pista* p){
	float3* ptos = p->puntos;
	return calcularNormal(ptos[0],ptos[1],ptos[2],true);
}

float3 acelPendientePista(Pista* p,Gravedad& g){
	float3 n = calcularNormalPista(p);
	float3 acel = g.AcelPendiente(n);
	return acel;
}

// calcula interseccion con rueda puntual por la altura del punto
// retorna si hubo colision, si la hubo nuevaPos es un punto sobre la pista, de lo contrario es la posicion vieja
bool calcularPosicionRuedaSimple(Pista* pista, const float3& centroRueda,float3& nuevaPos){
	float3 p(centroRueda.x,calcY(pista->plano,centroRueda.x,centroRueda.z),centroRueda.z);
	if (pista->getAABB().Contains(p)){
		float eps = 0.001;
		if (centroRueda.y <=  eps + p.y){
			nuevaPos = pista->plano.ClosestPoint(p);
			return true;
		}

	}

	nuevaPos = centroRueda;
	return false;

}



void ManejadorFisica::detectarColisionMoto(){
	std::vector<fisica::ObjetoFisico*> tmp;
	detectarColision(fm->boundingBox,tmp);
	sobrePista =false;
	colisiono = false;
	Pista* pista0 = 0;
	Pista* pista1 = 0;
	float3 punto0;
	float3 punto1;
	for(unsigned int i = 0; i < tmp.size(); i++){

		ObjetoFisico* obj = tmp[i];
		if (obj->colisiona(*fm)){
			//std::cout << "BBox obj: " << obj->BoundingAABB() << "\n";
			if (!obj->esObstaculo()){
				sobrePista = true;
				Pista* pista = dynamic_cast<Pista*>(obj);
				if (pista){
					acelGravedad = acelPendientePista(pista,this->g);
					this->pistaActual =pista;
					if (calcularPosicionRuedaSimple(pista,fm->ruedas[0].centroRueda,punto0)){
						pista0 = pista;
					}
					if (calcularPosicionRuedaSimple(pista,fm->ruedas[1].centroRueda,punto1)){
						pista1 = pista;
					}
				}
			}
			else{
				colisiono = true;
			}
		}
		colisiono &= !sobrePista;
	}
	/*if (pista0){
		fm->ruedas[0].setCentro(punto0);
	}
	if (pista1){
		fm->ruedas[1].setCentro(punto1);
	}*/
	if (pista0 || pista1){
		fm->posicionarPorRuedas(punto0,punto1);
	}
	if (!sobrePista){
		acelGravedad = g.AcelCaida();
	}
}

void ManejadorFisica::test(){
	AABB aabbBox = AABB(float3(1,1,1),float3(2,3,4));
	ObjetoFisico* b[2];
	b[0] = new BoxObstaculo(aabbBox);
	b[1] = new BoxObstaculo(AABB(float3(0,0,0),float3(0.5,0.9,0.9)));
	this->objetos->push_back(b[0]);
	this->objetos->push_back(b[1]);


	std::vector<fisica::ObjetoFisico*> lst ;
	AABB aabb1(float3(9,8,5),float3(10,10,10));
	detectarColision(aabb1,lst);
	std::cout << "Size:  " << lst.size() << "\n";
	AABB aabb2(float3(1.5,2.5,3.5),float3(5,5,5));
	detectarColision(aabb2,lst);
	std::cout << "Size:  " << lst.size() << "\n";
}



ManejadorFisica::~ManejadorFisica() {
	// TODO Auto-generated destructor stub
}

math::float3 toFloat3(float* arr, unsigned int idx){
	return float3(arr[idx],arr[idx+1],arr[idx+2]);
}

void ManejadorFisica::registrarRampaConObs(float* arr, unsigned char* arribaIdx, unsigned char* abajoIdx) {
	float3 arriba[4];
	float3 abajo[4];

	for(int i = 0; i < 4; i++) arriba[i] = toFloat3(arr,arribaIdx[i] * 3);
	for(int i = 0; i < 4; i++) abajo[i] = toFloat3(arr,abajoIdx[i] * 3);

	Rampa r =  Rampa(arriba,abajo);
	r.generarObjetos(this->objetos);
}


void ManejadorFisica::registrarRampaConObs(float3* arriba, float3* abajo) {
	Rampa r =  Rampa(arriba,abajo);
	r.generarObjetos(this->objetos);
	//return r;
}

void ManejadorFisica::establecerGravedad(float acelHaciaAbajo){
	this->g.g = -acelHaciaAbajo;
	this->acelGravedad = g.AcelCaida();
}



void ManejadorFisica::establecerPosicionMoto(float x, float y, float z){
	float3 p(x,y,z);
	fm->setPosicion(p);
}

bool ManejadorFisica::estaSobrePista(){
	return sobrePista;
}


float3 calcularVelocidadPostImpacto(float3 vel,float3 normalPlano){
	// calcular proyeccion de velocidad sobre normal
	float3 proyN = vel.Dot(normalPlano) * normalPlano;
	return vel - proyN;
}

float3 sinPerdidaDeVelocidad(float3 vel,float3 dirAvanceNueva){
	// calcular proyeccion de velocidad sobre normal
	float3 proyN = vel.Dot(dirAvanceNueva) * dirAvanceNueva;
	return proyN;
}


// retorna cambio de velocidad por gravedad
float3 ManejadorFisica::simularGravedad(float dt){

	if (this->fm->getY() <= alturaPiso){
		return float3::zero;
	}
	else{
		return acelGravedad * dt;
	}

}

float ManejadorFisica::getAnguloVertical(){
	// angulo entre direccionMoto y vertical
	return RadToDeg(Acos(direccionMoto.x));
}

void ManejadorFisica::simular(float dt){

	float3 dv_grav = simularGravedad(dt);
	//std::cout <<  "dt: " << dt << "    gv_grav : "<< dv_grav << "   vel " << velMoto << "  -> ";
	this->direccionMoto = fm->boxSuperior.axis[0];
	float3 dv_propio = direccionMoto  *  aceleracionPropia * dt;
	velMoto = velMoto + dv_propio + dv_grav;
	float3 deltaPos = velMoto * dt;
	fm->trasladar(deltaPos);

	detectarColisionMoto();

	// calculo direccion moto (rueda delantera - trasera)
	/*this->direccionMoto = fm->ruedas[1].centroRueda - fm->ruedas[0].centroRueda;
	this->direccionMoto.Normalize();*/


	if (this->sobrePista){
		this->velMoto = sinPerdidaDeVelocidad(velMoto,direccionMoto);
	}
	else if (this->colisiono){
		fm->setX(fm->getX() - 0.01);
	}
	else if (fm->getY() < this->alturaPiso){
		fm->setY(this->alturaPiso);
	}
	//if (velMoto->)

	//std::cout << "Pos Moto: "<< fm->getPos() << "SobrePista?: " << this->sobrePista << "\n";

}


bool ManejadorFisica::estaEnElAire(){
	return false;
}




// calcular altura de un punto dentro de un triangulo
// http://stackoverflow.com/questions/5507762/how-to-find-z-by-arbitrary-x-y-coordinates-within-triangle-if-you-have-triangle
float calcY(Triangle t, float x, float z) {
		vec p1 = t.a;
		vec p2 = t.b;
		vec p3 = t.c;
        float det = (p2.z - p3.z) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.z - p3.z);

        float l1 = ((p2.z - p3.z) * (x - p3.x) + (p3.x - p2.x) * (z - p3.z)) / det;
        float l2 = ((p3.z - p1.z) * (x - p3.x) + (p1.x - p3.x) * (z - p3.z)) / det;
        float l3 = 1.0f - l1 - l2;

        return l1 * p1.y + l2 * p2.y + l3 * p3.y;
}

// altura a partir de 3 puntos del plano
float calcY(float3 p1, float3 p2, float3 p3, float x, float z){
	float3 v1 = p2 - p1;
	float3 v2 = p3 - p1;
	float3 abc = v1.Cross(v2);
	float d = -abc.Dot(p1);// -(abc.x * p1.x + abc.y * p1.y + abc.z * p1.z);
	// Ax + By + Cz = -D
	// y = (-D - Ax - Cz) / B
	float y = (-d - abc.x * x - abc.z * z) / abc.y;
	return y;
}

// altura a partir de 3 puntos del plano
float calcY(const math::Plane& plano, float x, float z){
	float3 abc = plano.normal;
	float d = plano.d;
	// Ax + By + Cz = d
	// y = (d - Ax - Cz) / B
	float y = (d - abc.x * x - abc.z * z) / abc.y;
	return y;
}

void checkearCalcY(){
	float3 p1(1,1,1);
	float3 p2(3,7,1);
	float3 p3(3,7,-1);
	float x = 2;
	float z = 0;
	float altT, altP,alt3;
	altT= calcY(Triangle(p1,p2,p3),x,z);
	altP = calcY(Plane(p1,p2,p3),x,z);
	alt3 = calcY(p1,p2,p3,x,z);
	std::cout << "CalcY (" << x << "," << z << ") (y = 4):" << "\n";
	std::cout << "Altura x Triangulo: " << altT << "\n";
	std::cout << "Altura x Plano: " << altP << "\n";
	std::cout << "Altura x 3 puntos: " << alt3 << "\n";
	std::cout << "-----------------------------\n";

	altT= calcY(Triangle(p1,p2,p3),1,z);
	altP = calcY(Plane(p1,p2,p3),1,z);
	alt3 = calcY(p1,p2,p3,1,z);
	std::cout << "CalcY (" << 1 << "," << z << ") (y = 1):" << "\n";
	std::cout << "Altura x Triangulo: " << altT << "\n";
	std::cout << "Altura x Plano: " << altP << "\n";
	std::cout << "Altura x 3 puntos: " << alt3 << "\n";
	std::cout << "-----------------------------\n";

	altT= calcY(Triangle(p1,p2,p3),3,0);
	altP = calcY(Plane(p1,p2,p3),3,0);
	alt3 = calcY(p1,p2,p3,3,0);
	std::cout << "CalcY (" << 3 << "," << 0 << ") (y = 7):" << "\n";
	std::cout << "Altura x Triangulo: " << altT << "\n";
	std::cout << "Altura x Plano: " << altP << "\n";
	std::cout << "Altura x 3 puntos: " << alt3 << "\n";
	std::cout << "-----------------------------\n";

	altT= calcY(Triangle(p1,p2,p3),5,0);
	altP = calcY(Plane(p1,p2,p3),5,0);
	alt3 = calcY(p1,p2,p3,5,0);
	std::cout << "CalcY (" << 5 << "," << 0 << ") (y = 13):" << "\n";
	std::cout << "Altura x Triangulo: " << altT << "\n";
	std::cout << "Altura x Plano: " << altP << "\n";
	std::cout << "Altura x 3 puntos: " << alt3 << "\n";
	std::cout << "-----------------------------\n";


}

float ManejadorFisica::getAltura(){
	if (this->estaSobrePista()){
		float x = fm->boundingBox.minPoint.x;
		float z = fm->boundingBox.minPoint.z;
		math::Plane pl = this->pistaActual->plano;
		return calcY(pl,x,z);
	}
	else{
		return fm->boundingBox.minPoint.y;
	}
}


