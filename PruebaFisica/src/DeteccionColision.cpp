/*
 * DeteccionColision.cpp
 *
 *  Created on: 11 de may. de 2016
 *      Author: Guillermo
 */

#include "DeteccionColision.h"
#include "OtherContrib/TriboxOverlap.h"
#include "OtherContrib/kdtree.h"
#include "MatGeoLib/Time/Clock.h"
#include "MatGeoLib/Geometry/KDTree.h"
#include <set>
#include "stdlib.h" // rand
#include <time.h>       /* time */
#include <vector>
#include <iostream>



DeteccionColision::DeteccionColision() {
	// TODO Auto-generated constructor stub

}

DeteccionColision::~DeteccionColision() {
	// TODO Auto-generated destructor stub
}

void DeteccionColision::test(){
	math::Clock clk;
	srand (time(NULL));
	std::vector<math::AABB> boxes;
	for (int i = 0; i < 100000; i++){
		float3 r_vec(rand(),rand(),rand());
		float3 r_long(rand() % 10,rand() % 10,rand() % 10);
		math::AABB aabb(r_vec, r_vec + r_long);
		boxes.push_back(aabb);
	}
	/*math::KdTree<math::AABB> mathGeoKd;
	for(int i = 0; i < boxes.size(); i++){
		mathGeoKd.AddObjects(boxes[i],1);
	}*/

	kdtree* cKD = kd_create(3);

	for(int i = 0; i < boxes.size(); i++){
		math::AABB a  = boxes[i];
		float3 coords[2] = {a.minPoint,a.maxPoint};
		for(int k = 0; k < 7; k++){
			kd_insert3f(cKD,coords[k / 4].x,coords[(k / 2) % 2].y,coords[k % 2].z,(void*)&a);
		}
	}

	std::vector<AABB> rangos;


	float rango = 20;

	for (int i = 0; i < 10000; i++){
		float3 centro(rand(),rand(),rand());

		math::AABB aabb;
		aabb.FromCenterAndSize(centro,float3(rango * 2,rango * 2,rango * 2));
		rangos.push_back(aabb);
	}


	int total_match = 0;
	math::tick_t t1 = clk.Tick();
	for(int i = 0; i < rangos.size(); i++){
		std::set<float> resultados;
		float3 centro = rangos[i].CenterPoint();
		kdres* res = kd_nearest_range3f(cKD,centro.x,centro.y,centro.z,rango);
		while(kd_res_next(res)){
			math::AABB  d = *(AABB*)kd_res_item_data(res);
			resultados.emplace(d.MinX() * d.MinY() * d.MinZ() );
		}
		kd_res_free(res);
		total_match += resultados.size();
	}
	std::cout <<"C Kd-tree: " << clk.MillisecondsSinceF(t1) << "  tm: " + total_match << "\n";

}






