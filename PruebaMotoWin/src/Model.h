#ifndef SRC_MODEL_H_
#define SRC_MODEL_H_

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <GL/glu.h>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


#define aisgl_min(x,y) (x<y?x:y)
#define aisgl_max(x,y) (y>x?y:x)

namespace model {

class Model {
public:
	GLdouble velX,velY,posX,posY;//Posiciones son los minimos del bounding box. (velocidad en segundos/unidad)
	Model();
	virtual ~Model();
	int loadasset (const GLchar* path);
	void display();
	void draw(GLdouble angulo,GLdouble angulo_centro);
	void get_bounding_box_for_node (const aiNode* nd,aiVector3D* min,aiVector3D* max,aiMatrix4x4* trafo);
	void get_bounding_box (aiVector3D* min, aiVector3D* max);

	//Manejo de Moto
	//Incrementa la velocidad aX en X y aY en Y por unidad de tiempo
	void acelerar(GLdouble aX,GLdouble aY,GLdouble dt);
	bool usarWireframe;

private:
	static const GLdouble relLargoEjeTrasero = 0.173553719; //(2.1/12.1)
	static const GLdouble relAltoEjeTrasero = 0.271428571; //(1.9/7)

	const aiScene* scene;
	GLuint scene_list;
	aiVector3D scene_min, scene_max, scene_center;
	/* current rotation angle */
	float angle;
	void reshape(int width, int height);
	void color4_to_float4(const aiColor4D *c, float f[4]);
	void set_float4(float f[4], float a, float b, float c, float d);
	void apply_material(const aiMaterial *mtl);
	void recursive_render (const aiScene *sc, const aiNode* nd);
	void do_motion();
};

} /* namespace model */

#endif /* SRC_MODEL_H_ */
