#include "Model.h"
#include <iostream>
#include "Fisica/MatGeoLib/Time/Clock.h"
#include <string>

namespace model {

Model::Model() {
	this->scene=NULL;
	this->scene_list=0;
	//scene_min, scene_max, scene_center;
	this->angle = 0.0;
	this->velX = 0.0;
	this->velY = 0.0;
	this->posX = 0;
	this->posY = 0;
	this->usarWireframe = false;

}

Model::~Model() {
	aiReleaseImport(scene);
}

/* ---------------------------------------------------------------------------- */
void Model::reshape(int width, int height){
	const double aspectRatio = (float) width / height, fieldOfView = 45.0;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fieldOfView, aspectRatio,1.0, 1000.0);  /* Znear and Zfar */
	glViewport(0, 0, width, height);
}

/* ---------------------------------------------------------------------------- */
void Model::get_bounding_box_for_node (const aiNode* nd,
	aiVector3D* min,
	aiVector3D* max,
	aiMatrix4x4* trafo
){
	aiMatrix4x4 prev;
	unsigned int n = 0, t;

	prev = *trafo;
	aiMultiplyMatrix4(trafo,&nd->mTransformation);

	for (; n < nd->mNumMeshes; ++n) {
		const aiMesh* mesh = scene->mMeshes[nd->mMeshes[n]];
		for (t = 0; t < mesh->mNumVertices; ++t) {

			aiVector3D tmp = mesh->mVertices[t];
			aiTransformVecByMatrix4(&tmp,trafo);

			min->x = aisgl_min(min->x,tmp.x);
			min->y = aisgl_min(min->y,tmp.y);
			min->z = aisgl_min(min->z,tmp.z);

			max->x = aisgl_max(max->x,tmp.x);
			max->y = aisgl_max(max->y,tmp.y);
			max->z = aisgl_max(max->z,tmp.z);
		}
	}

	for (n = 0; n < nd->mNumChildren; ++n) {
		get_bounding_box_for_node(nd->mChildren[n],min,max,trafo);
	}
	*trafo = prev;
}

/* ---------------------------------------------------------------------------- */
void Model::get_bounding_box (aiVector3D* min, aiVector3D* max)
{
	//math::Clock clock;
	//math::tick_t t1 = clock.Tick();
	aiMatrix4x4 trafo;
	aiIdentityMatrix4(&trafo);

	min->x = min->y = min->z =  1e10f;
	max->x = max->y = max->z = -1e10f;
	get_bounding_box_for_node(scene->mRootNode,min,max,&trafo);

	//std::cout << "ms get_bounding_box: " << clock.MillisecondsSinceF(t1) << " ms\n";
}

/* ---------------------------------------------------------------------------- */
void Model::color4_to_float4(const aiColor4D *c, float f[4])
{
	f[0] = c->r;
	f[1] = c->g;
	f[2] = c->b;
	f[3] = c->a;
}

/* ---------------------------------------------------------------------------- */
void Model::set_float4(float f[4], float a, float b, float c, float d)
{
	f[0] = a;
	f[1] = b;
	f[2] = c;
	f[3] = d;
}

/* ---------------------------------------------------------------------------- */
void Model::apply_material(const aiMaterial *mtl)
{
	float c[4];

	GLenum fill_mode;
	int ret1, ret2;
	aiColor4D diffuse;
	aiColor4D specular;
	aiColor4D ambient;
	aiColor4D emission;
	float shininess, strength;
	int two_sided;
	int wireframe;
	unsigned int max;

	int texIndex = 0;
	aiString texPath;	//contains filename of texture

	/*if(AI_SUCCESS == mtl->GetTexture(aiTextureType_DIFFUSE, texIndex, &texPath))
		{
			//bind texture
			unsigned int texId = texturesAndPaths[texPath.data];
			glBindTexture(GL_TEXTURE_2D, texId);
		}*/


	set_float4(c, 0.8f, 0.8f, 0.8f, 1.0f);
	if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_DIFFUSE, &diffuse))
		color4_to_float4(&diffuse, c);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, c);

	set_float4(c, 0.0f, 0.0f, 0.0f, 1.0f);
	if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_SPECULAR, &specular))
		color4_to_float4(&specular, c);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, c);

	set_float4(c, 0.2f, 0.2f, 0.2f, 1.0f);
	if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_AMBIENT, &ambient))
		color4_to_float4(&ambient, c);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, c);

	set_float4(c, 0.0f, 0.0f, 0.0f, 1.0f);
	if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_EMISSIVE, &emission))
		color4_to_float4(&emission, c);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, c);

	max = 1;
	ret1 = aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS, &shininess, &max);
	if(ret1 == AI_SUCCESS) {
    	max = 1;
    	ret2 = aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS_STRENGTH, &strength, &max);
		if(ret2 == AI_SUCCESS)
			glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess * strength);
        else
        	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
    }
	else {
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.0f);
		set_float4(c, 0.0f, 0.0f, 0.0f, 0.0f);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, c);
	}

	max = 1;
	if (usarWireframe)
		fill_mode = GL_LINE;
	else if(AI_SUCCESS == aiGetMaterialIntegerArray(mtl, AI_MATKEY_ENABLE_WIREFRAME, &wireframe, &max))
			fill_mode = wireframe ? GL_LINE : GL_FILL;
	else
		fill_mode = GL_FILL;

	glPolygonMode(GL_FRONT_AND_BACK, fill_mode);

	max = 1;
	if((AI_SUCCESS == aiGetMaterialIntegerArray(mtl, AI_MATKEY_TWOSIDED, &two_sided, &max)) && two_sided)
		glDisable(GL_CULL_FACE);
	else
		glEnable(GL_CULL_FACE);
}

/* ---------------------------------------------------------------------------- */
void Model::recursive_render (const aiScene *sc, const aiNode* nd)
{



	unsigned int i;
	unsigned int n = 0, t;
	aiMatrix4x4 m = nd->mTransformation;

	/* update transform */
	aiTransposeMatrix4(&m);
	glPushMatrix();
	glMultMatrixf((float*)&m);

	/* draw all meshes assigned to this node */
	for (; n < nd->mNumMeshes; ++n) {
		const aiMesh* mesh = scene->mMeshes[nd->mMeshes[n]];


		if(n < texturesAndPaths.size())
			glBindTexture(GL_TEXTURE_2D, texturesAndPaths[n].hTexture);

		apply_material(sc->mMaterials[mesh->mMaterialIndex]);

		if(mesh->mNormals == NULL) {
			glDisable(GL_LIGHTING);
		} else {
			glEnable(GL_LIGHTING);
		}

		for (t = 0; t < mesh->mNumFaces; ++t) {
			const aiFace* face = &mesh->mFaces[t];
			GLenum face_mode;

			switch(face->mNumIndices) {
				case 1: face_mode = GL_POINTS; break;
				case 2: face_mode = GL_LINES; break;
				case 3: face_mode = GL_TRIANGLES; break;
				default: face_mode = GL_POLYGON; break;
			}

			glBegin(face_mode);

			for(i = 0; i < face->mNumIndices; i++) {
				int index = face->mIndices[i];
				if(mesh->mColors[0] != NULL)
					glColor4fv((GLfloat*)&mesh->mColors[0][index]);
				if(mesh->mNormals != NULL)
					glNormal3fv(&mesh->mNormals[index].x);
				if(mesh->HasTextureCoords(0))
					glTexCoord2f(mesh->mTextureCoords[0][index].x, mesh->mTextureCoords[0][index].y);
				glVertex3fv(&mesh->mVertices[index].x);
			}

			glEnd();
		}
		//Para que no se vea rojo
		//glDisable(GL_LIGHTING);

	}

	/* draw all children */
	for (n = 0; n < nd->mNumChildren; ++n) {
		recursive_render(sc, nd->mChildren[n]);
	}

	glPopMatrix();


}

/* ---------------------------------------------------------------------------- */
//void Model::do_motion (void)
//{
//	static GLint prev_time = 0;
//	static GLint prev_fps_time = 0;
//	static int frames = 0;
//
//	int time = glutGet(GLUT_ELAPSED_TIME);
//	angle += (time-prev_time)*0.01;
//	prev_time = time;
//
//	frames += 1;
//	if ((time - prev_fps_time) > 1000) /* update every seconds */
//    {
//        int current_fps = frames * 1000 / (time - prev_fps_time);
//        printf("%d fps\n", current_fps);
//        frames = 0;
//        prev_fps_time = time;
//    }
//	glutPostRedisplay ();
//}

/* ---------------------------------------------------------------------------- */
void Model::display()
{
	float tmp;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.f,0.f,3.f,0.f,0.f,-5.f,0.f,1.f,0.f);

	/* rotate it around the y axis */
	glRotatef(angle,0.f,1.f,0.f);

	/* scale the whole asset to fit into our view frustum */
	tmp = scene_max.x-scene_min.x;
	tmp =  aisgl_max(scene_max.y - scene_min.y,tmp);
	tmp = aisgl_max(scene_max.z - scene_min.z,tmp);
	tmp = 1.f / tmp;
	glScalef(tmp, tmp, tmp);

        /* center the model */
	glTranslatef( -scene_center.x, -scene_center.y, -scene_center.z );

        /* if the display list has not been made yet, create a new one and
           fill it with scene contents */
	if(scene_list == 0) {
	    scene_list = glGenLists(1);
	    glNewList(scene_list, GL_COMPILE);
            /* now begin at the root node of the imported data and traverse
               the scenegraph by multiplying subsequent local transforms
               together on GL's matrix stack. */
	    recursive_render(scene, scene->mRootNode);
	    glEndList();
	}

	glCallList(scene_list);

	SDL_GL_SwapBuffers();

	//do_motion();
}


// basado en http://www.gamedev.net/topic/582240-assimp-drawing-textured-model/
void Model::recursiveTextureLoad(const struct aiScene *sc, const struct aiNode* nd)

{

	int i;
	unsigned int n = 0, t;
	 aiMatrix4x4 m = nd->mTransformation;



	// update transform

	aiTransposeMatrix4(&m);
	glPushMatrix();
	glMultMatrixf((float*)&m);



	// draw all meshes assigned to this node

	for (; n < nd->mNumMeshes; ++n)
	{

		const struct aiMesh* mesh = sc->mMeshes[nd->mMeshes[n]];
		unsigned int cont = aiGetMaterialTextureCount(sc->mMaterials[mesh->mMaterialIndex], aiTextureType_DIFFUSE);
		struct aiString* str = (aiString*)malloc(sizeof(struct aiString));

		if(cont > 0)
		{
			//aiGetMaterialString(sc->mMaterials[mesh->mMaterialIndex],AI_MATKEY_TEXTURE_DIFFUSE(0),str);
			aiGetMaterialTexture(sc->mMaterials[mesh->mMaterialIndex],aiTextureType_DIFFUSE,0,str,0,0,0,0,0,0);

			// See if another mesh is already using this texture, if so, just copy GLuint instead of remaking entire texture
			bool newTextureToBeLoaded = true;
			for(int x = 0; x < texturesAndPaths.size(); x++)
			{
				if(texturesAndPaths[x].pathName == *str)
				{
					TextureAndPath reusedTexture;
					reusedTexture.hTexture = texturesAndPaths[x].hTexture;
					reusedTexture.pathName = *str;
					texturesAndPaths.push_back(reusedTexture);
					newTextureToBeLoaded = false;

					std::cout << "Texture reused." << std::endl;

					break;

				}
			}



			if(newTextureToBeLoaded)
			{
				FREE_IMAGE_FORMAT formato = FreeImage_GetFileType(str->data,0);
				//Automatocally detects the format(from over 20 formats!)
				FIBITMAP* imagen = FreeImage_Load(formato, str->data);
				FIBITMAP* temp = imagen;
				imagen = FreeImage_ConvertTo32Bits(imagen);

				FreeImage_Unload(temp);
				int w = FreeImage_GetWidth(imagen);
				int h = FreeImage_GetHeight(imagen);


				//Some debugging code
				char* pixeles = (char*)FreeImage_GetBits(imagen);

				//FreeImage loads in BGR format, so you need to swap some bytes(Or use GL_BGR).
				//Now generate the OpenGL texture object

				TextureAndPath newTexture;
				newTexture.pathName = *str;
				glGenTextures(1, &newTexture.hTexture);



				glBindTexture(GL_TEXTURE_2D, newTexture.hTexture);
				glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA, w, h, 0, GL_BGRA_EXT,GL_UNSIGNED_BYTE,(GLvoid*)pixeles );
				//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
				glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

				glBindTexture(GL_TEXTURE_2D, newTexture.hTexture);


				GLenum huboError = glGetError();



				if(huboError)
				{
					std::cout<<"There was an error loading the texture"<<std::endl;
				}



				std::cout << "texture loaded." << std::endl;


				texturesAndPaths.push_back(newTexture);

			}
		}
	}


	// Get textures from all children

	for (n = 0; n < nd->mNumChildren; ++n)
		recursiveTextureLoad(sc, nd->mChildren[n]);

}


/* ---------------------------------------------------------------------------- */
int Model::loadasset (const char* path)
{

	math::Clock clock;
	math::tick_t t1 = clock.Tick();
	/* we are taking one of the postprocessing presets to avoid
	   spelling out 20+ single postprocessing flags here. */
	scene = aiImportFile(path,aiProcessPreset_TargetRealtime_MaxQuality);

	if (scene) {
		get_bounding_box(&scene_min,&scene_max);
		scene_center.x = (scene_min.x + scene_max.x) / 2.0f;
		scene_center.y = (scene_min.y + scene_max.y) / 2.0f;
		scene_center.z = (scene_min.z + scene_max.z) / 2.0f;
		std::cout << "ms loadasset: " << path <<  "  " << clock.MillisecondsSinceF(t1) << " ms\n";
		recursiveTextureLoad(scene, scene->mRootNode);

		return 1;
	}
	std::cout << "ms loadasset failed: " << clock.MillisecondsSinceF(t1) << " ms\n";
	return 0;
}

/* ---------------------------------------------------------------------------- */
void Model::draw(GLdouble angulo,GLdouble angulo_centro)
{
	glPushMatrix();
	aiVector3D *min = new aiVector3D(),*max = new aiVector3D();
	get_bounding_box(min,max);
	GLdouble largoMoto = (max->x-min->x);
	GLdouble altoMoto = (max->y-min->y);
	float escala = largoMoto > 1 ? 1/largoMoto : 1;

	//Muevo moto
	glTranslated(posX - largoMoto/2,posY ,0);
	glScalef(escala, escala, escala);
	//Roto moto
	glTranslated(-largoMoto/2+largoMoto*relLargoEjeTrasero,altoMoto*relAltoEjeTrasero,0);
	glRotated(angulo,0,0,1);
	glTranslated(largoMoto/2-largoMoto*relLargoEjeTrasero,-altoMoto*relAltoEjeTrasero,0);
	float centro_rot_x = largoMoto / 2;
	float centro_rot_y = altoMoto / 2;
	glColor3f(1,0,1);
	glTranslated(0,0,0);
	// centro de rotacion
	glBegin(GL_POINTS);
	glVertex3d(0, 0, 0);
	glEnd();
	glRotated(angulo_centro,0,0,1);
	glTranslated(0, 0,0);





	//Dibujo moto
	recursive_render(scene, scene->mRootNode);
	glPopMatrix();
}


//MANEJO MOTO

void Model::acelerar(GLdouble aX,GLdouble aY,GLdouble dt){
	velX+= aX*dt;
	velY+= aY*dt;
	posX+= velX*dt;
	posY+= velY*dt;
}


} /* namespace model */
