#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL/SDL.h>
#include <string>

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "camera.h"
#include "Model.h"

#include "Fisica/ManejadorFisica.h"
#include "Fisica/Rampa.h"
#include "Fisica/MatGeoLib/Geometry/OBB.h"

#include "FreeImage/FreeImage.h"

#define SCREEN_WIDTH  1000
#define SCREEN_HEIGHT 640
#define SCREEN_BPP     16

#define TRUE  1
#define FALSE 0

#define V0 -0.5,-0.5,-0.5
#define V1 0.5,-0.5,-0.5
#define V2 0.5,1.5,-0.5
#define V3 -0.5,0.5,-0.5
#define V4 -0.5,-0.5,0.5
#define V5 0.5,-0.5,0.5
#define V6 0.5,1.5,0.5
#define V7 -0.5,0.5,0.5

#define DRAW_QUAD(P1,P2,P3,P4) glBegin(GL_QUADS);\
                                glVertex3f(P1);\
                                glVertex3f(P2);\
                                glVertex3f(P3);\
                                glVertex3f(P4);\
                        glEnd()

#define f3Tof(v) v.x,v.y,v.z

#define DRAW_QUAD2(P1,P2,P3,P4) glBegin(GL_QUADS);\
								glTexCoord2d(0,0);\
                                glVertex3f(f3Tof(P1));\
                                glTexCoord2d(0,1);\
                                glVertex3f(f3Tof(P2));\
                                glTexCoord2d(1,1);\
                                glVertex3f(f3Tof(P3));\
                                glTexCoord2d(1,0);\
                                glVertex3f(f3Tof(P4));\
                        glEnd()

#define FRONT_QUAD DRAW_QUAD(V4,V5,V6,V7)
#define RIGHT_QUAD DRAW_QUAD(V1,V2,V6,V5)
#define BOTTOM_QUAD DRAW_QUAD(V0,V1,V5,V4)
#define BACK_QUAD DRAW_QUAD(V0,V3,V2,V1)
#define LEFT_QUAD DRAW_QUAD(V0,V4,V7,V3)
#define TOP_QUAD DRAW_QUAD(V2,V3,V7,V6)

using namespace std;

model::Model* model2;
SDL_Surface *surface;
GLuint ramp_list;
GLfloat inicioNivel[3] = { -10, 0, 0 };
GLfloat finNivel[3] = { 10, 0, 0 };
GLdouble angulo = 0; //Angulo de inclinacion de la moto

GLuint texturaConcreto = 0;
bool wireframe = false;
bool sostengoClickDerecho = false;
bool enPausa = false;

ManejadorFisica* manejador;

std::vector<Info_Rampa> rampasRectas;

void quit(int ret) {
	SDL_Quit();
	exit(ret);
}

struct cam_t cam;

void cambiarWireframe() {
	wireframe = !wireframe;
	model2->usarWireframe = wireframe;
	glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);
}

bool cargarTextura(GLuint& textura, int& w, int& h,const char* archivo){
    /*char* archivo = new char[50];
    archivo = "res/Models/concreteTexture.jpg";*/

    FREE_IMAGE_FORMAT fif = FreeImage_GetFIFFromFilename(archivo);
    FIBITMAP* bitmap = FreeImage_Load(fif, archivo);
    if (bitmap){
		bitmap = FreeImage_ConvertTo24Bits(bitmap);

		w = FreeImage_GetWidth(bitmap);
		h = FreeImage_GetHeight(bitmap);

		void* datos = FreeImage_GetBits(bitmap);


		glGenTextures(1, &textura); // genera id de texturas

		glBindTexture(GL_TEXTURE_2D, textura);

		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);


		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // filtro para downscaling
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // filtro para upscaling
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

		// specify a two-dimensional texture image
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_BGR, GL_UNSIGNED_BYTE, datos);

		delete datos;
		return true;
    }
    else {

    	return false;
    	//exit (0);
    }


}

bool cargarTextura2(GLuint& textura, int& w, int& h,char* archivo){
	string nombre(archivo);
	string basePath= "res/Models/";

	string paths[] = {
			(basePath + nombre),
			("../../res/Models/" + nombre),
			("../res/Models/" + nombre),
			("./" + nombre),
	};
	bool cargo = false;
	for(int i = 0; i < 4 && !cargo; i++){
		cargo = cargarTextura(textura,w,h,paths[i].c_str());
	}
	if (!cargo){
    	std::cerr << "Error cargando textura " << archivo << std::endl;
	}
	return cargo;
}

void registrarRampa(float x_base, float y_base, float x_size, float z_size,
		float altura_x_min, float altura_x_max) {
	float z_base = -z_size / 2;
	math::float3 v0(x_base, y_base, z_base);
	math::float3 v1 = v0 + float3(x_size, 0, 0);
	math::float3 v2 = v1 + float3(0, altura_x_max, 0);
	math::float3 v3 = v0 + float3(0, altura_x_min, 0);
	math::float3 v4 = v0 + float3(0, 0, z_size);
	math::float3 v5 = v4 + float3(x_size, 0, 0);
	math::float3 v6 = v2 + float3(0, 0, z_size);
	math::float3 v7 = v0 + float3(0, altura_x_min, z_size);

	math::float3 abajo[4] = { v0, v1, v5, v4 };

	std::cout << "\n";
	math::float3 arriba[4] = { v2, v3, v7, v6 };

	std::cout << "\n";
	manejador->registrarRampaConObs(arriba, abajo);
	Info_Rampa info;
	info.init(x_base,y_base,x_size,z_size,altura_x_min,altura_x_max);
	rampasRectas.push_back(info);
}

void dibujaRampa(const Info_Rampa& i){//(float x_base, float y_base, float x_size, float z_size,float altura_x_min, float altura_x_max) {

	float z_base = -i.z_size / 2;
	math::float3 v0(i.x_base, i.y_base, z_base);
	math::float3 v1 = v0 + float3(i.x_size, 0, 0);
	math::float3 v2 = v1 + float3(0, i.altura_x_max, 0);
	math::float3 v3 = v0 + float3(0, i.altura_x_min, 0);
	math::float3 v4 = v0 + float3(0, 0, i.z_size);
	math::float3 v5 = v4 + float3(i.x_size, 0, 0);
	math::float3 v6 = v2 + float3(0, 0, i.z_size);
	math::float3 v7 = v0 + float3(0, i.altura_x_min, i.z_size);

	glColor3f(0.0f, 1.0f, 0.0f); /* Set The Color To Green           */
	DRAW_QUAD2(v4, v5, v6, v7); // front

	glColor3f(1.0f, 0.5f, 0.0f); /* Set The Color To Orange          */
	DRAW_QUAD2(v1, v2, v6, v5); // right

	glColor3f(1.0f, 0.0f, 0.0f); /* Set The Color To Red             */
	DRAW_QUAD2(v0, v1, v5, v4); // bottom

	glColor3f(1.0f, 1.0f, 0.0f); /* Set The Color To Yellow          */
	DRAW_QUAD2(v0, v3, v2, v1); // back

	glColor3f(0.0f, 0.0f, 1.0f); /* Set The Color To Blue            */
	DRAW_QUAD2(v0, v4, v7, v3); //left

	glColor3f(1.0f, 0.0f, 1.0f); /* Set The Color To Violet          */
	DRAW_QUAD2(v2, v3, v7, v6); // top

}

void draw_wall(GLfloat size, GLint x_lines, GLint z_lines) {
	glBegin(GL_LINES);
	int i;
	for (i = 0; i < x_lines; i++) {
		GLfloat xcoord = -size / 2.0 + i / (GLfloat) (x_lines - 1) * size;
		glVertex3f(xcoord, 0.0, size / 2.0);
		glVertex3f(xcoord, 0.0, size / -2.0);
	}
	int k;
	for (k = 0; k < x_lines; k++) {
		GLfloat zcoord = -size / 2.0 + k / (GLfloat) (z_lines - 1) * size;
		glVertex3f(size / 2.0, 0.0, zcoord);
		glVertex3f(size / -2.0, 0.0, zcoord);
	}
	glEnd();
}

/* function to reset our viewport after a window resize */
int resize_window(int width, int height) {
	if (height == 0) {
		height = 1;
	}
	GLfloat ratio = (GLfloat) width / (GLfloat) height;
	glViewport(0, 0, (GLsizei) width, (GLsizei) height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, ratio, 0.1f, 100.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	return (TRUE);
}

/* function to handle key press events */
void handle_key_press(SDL_keysym * keysym) {
	switch (keysym->sym) {
	case SDLK_ESCAPE:
		/* ESC key was pressed */
		quit(0);
		break;
	case SDLK_F1:
		/* F1 key was pressed
		 * this toggles fullscreen mode
		 */
		SDL_WM_ToggleFullScreen(surface);
		break;
	case SDLK_w:
		cambiarWireframe();
		break;
	case SDLK_p:
		enPausa = !enPausa;
		break;
	default:
		break;
	}

	return;
}

void registrarRampas() {
	registrarRampa(-0.5, -0.5, 1, 1, 1, 2);
	registrarRampa(2, -0.5, 1, 1, 1, 2);
	registrarRampa(4, -0.5, 1, 1, 1, 2);
	registrarRampa(-4, -0.5, 1, 1, 0, 4);
}

void dibujarRampas() {
	for(int i = 0; i < rampasRectas.size(); i++){
		dibujaRampa(rampasRectas[i]);
	}
}

void draw_ramp(void) {
	glColor3f(0.0f, 1.0f, 0.0f); /* Set The Color To Green           */
	FRONT_QUAD
	;
	glColor3f(1.0f, 0.5f, 0.0f); /* Set The Color To Orange          */
	RIGHT_QUAD
	;
	glColor3f(1.0f, 0.0f, 0.0f); /* Set The Color To Red             */
	BOTTOM_QUAD
	;
	glColor3f(1.0f, 1.0f, 0.0f); /* Set The Color To Yellow          */
	BACK_QUAD
	;
	glColor3f(0.0f, 0.0f, 1.0f); /* Set The Color To Blue            */
	LEFT_QUAD
	;
	glColor3f(1.0f, 0.0f, 1.0f); /* Set The Color To Violet          */
	TOP_QUAD
	;
}

void mostrarObstaculo() {
	static GLfloat vertices[] = { -0.25, -3.0, -0.25, // 0
			0.25, -3.0, -0.25, // 1
			0.25, 3.0, -0.25, // 2
			-0.25, 3.0, -0.25, // 3
			-0.25, -3.0, 0.25, // 4
			0.25, -3.0, 0.25, // 5
			0.25, 1.0, 0.25, // 6
			-0.25, 1.0, 0.25, // 7
			};

	static GLubyte front_indices[] = { 4, 5, 6, 7 };
	static GLubyte right_indices[] = { 1, 2, 6, 5 };
	static GLubyte bottom_indices[] = { 0, 1, 5, 4 };
	static GLubyte back_indices[] = { 0, 3, 2, 1 };
	static GLubyte left_indices[] = { 0, 4, 7, 3 };
	static GLubyte top_indices[] = { 2, 3, 7, 6 };

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, vertices);

	//ramp_list = glGenLists(1);
	//glNewList(ramp_list, GL_COMPILE);
	glColor3f(0.0f, 1.0f, 0.0f); /* Set The Color To Green           */
	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_BYTE, front_indices);

	glColor3f(1.0f, 0.5f, 0.0f); /* Set The Color To Orange          */
	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_BYTE, right_indices);

	glColor3f(1.0f, 0.0f, 0.0f); /* Set The Color To Red             */
	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_BYTE, bottom_indices);

	glColor3f(1.0f, 1.0f, 0.0f); /* Set The Color To Yellow          */
	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_BYTE, back_indices);

	glColor3f(0.0f, 0.0f, 1.0f); /* Set The Color To Blue            */
	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_BYTE, left_indices);

	glColor3f(1.0f, 0.0f, 1.0f); /* Set The Color To Violet          */
	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_BYTE, top_indices);
	//glEndList();
}

void setup_pointers(void) {
	static GLfloat vertices[] = {
	/*0*/-0.5, -0.5, -0.5,
	/*1*/0.5, -0.5, -0.5,
	/*2*/0.5, 1.5, -0.5,
	/*3*/-0.5, 0.5, -0.5,
	/*4*/-0.5, -0.5, 0.5,
	/*5*/0.5, -0.5, 0.5,
	/*6*/0.5, 1.5, 0.5,
	/*7*/-0.5, 0.5, 0.5,

	/*8*/-1.0, -1.0, -1.0,
	/*9*/1.0, -1.0, -1.0,
	/*10*/1.0, 1.0, -1.0,
	/*11*/-1.0, 1.0, -1.0, };

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, vertices);
}

void ramp_sides(void) {
	static GLubyte front_indices[] = { 4, 5, 6, 7 };
	static GLubyte right_indices[] = { 1, 2, 6, 5 };
	static GLubyte bottom_indices[] = { 0, 1, 5, 4 };
	static GLubyte back_indices[] = { 0, 3, 2, 1 };
	static GLubyte left_indices[] = { 0, 4, 7, 3 };
	static GLubyte top_indices[] = { 2, 3, 7, 6 };
}



int init_gl() {
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL); // activa usar color como color de material
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);
	// check OpenGL error
	GLenum err;

	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);


	char* archivo = new char[50];
	archivo = "grass1024.jpg";
	int w, h;
	if (!texturaConcreto){
		cargarTextura2(texturaConcreto,w,h,archivo);
	}


	GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	   glEnable(GL_LIGHTING);
	   glEnable(GL_LIGHT0);

	//setup_pointers();

	// check OpenGL error
	while ((err = glGetError()) != GL_NO_ERROR) {
		cerr << "OpenGL error: " << err << endl;

	}

	return (TRUE);
}




int draw_gl_scene() {
	/* These are to calculate our fps */
	static GLint T0 = 0;
	static GLint Frames = 0;
	static GLfloat backtrans = 0.0f;
	static GLfloat middletrans = 0.0f;
	static GLfloat fronttrans = 0.0f;
	static GLuint prev_sind = 0;

	/*start drawing box*/
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	render(cam);
	glPushMatrix();
	glTranslatef(0.0, -0.5, -6.0);
	glScalef(3.0, 1.0, 3.0);
	GLfloat size = 8.0;
	GLint x_lines = 30;
	GLint z_lines = 30;
	GLfloat halfsize = size / 2.0;
	glColor3f(1.0, 1.0, 1.0);
	glPushMatrix();
	glTranslatef(0.0, -halfsize, 0.0);
	draw_wall(size, x_lines, z_lines);
	glTranslatef(0.0, size, 0.0);
	draw_wall(size, x_lines, z_lines);
	glPopMatrix();
	glColor3f(0.0, 0.0, 1.0);
	glPushMatrix();
	glTranslatef(-halfsize, 0.0, 0.0);
	glRotatef(90.0, 0.0, 0.0, halfsize);
	draw_wall(size, x_lines, z_lines);
	glTranslatef(0.0, -size, 0.0);
	draw_wall(size, x_lines, z_lines);
	glPopMatrix();
	glColor3f(1.0, 0.0, 0.0);
	glPushMatrix();
	glTranslatef(0.0, 0.0, -halfsize);
	glRotatef(90.0, halfsize, 0.0, 0.0);
	draw_wall(size, x_lines, z_lines);
	glTranslatef(0.0, size, 0.0);
	draw_wall(size, x_lines, z_lines);
	glPopMatrix();
	glPopMatrix();
	/*finish drawing box*/
	glPushMatrix();
	glTranslatef(0.0, 0.0, 0.0);
	//glCallList(ramp_list);
	//mostrarObstaculo();
	manejador->dibujarBB();
	dibujarRampas();

	/*
	 glTranslatef(0.0, 0.0, 0.0);
	 glScalef(0.25, 0.25, 0.25);
	 glCallList(ramp_list);
	 glTranslatef(0.0, 0.3, 1.0);
	 glScalef(1.0, 1.25, 1.0);
	 glCallList(ramp_list);
	 glTranslatef(0.0, 0.25, 1.0);
	 glScalef(1.0, 1.25, 1.0);
	 glCallList(ramp_list);
	 glTranslatef(0.0, 1.0, 1.5);
	 glCallList(ramp_list);
	 glTranslatef(0.0, 5.3, 0.0);*/
	//glScalef( 1.0, -0.7, -1.0);
//        glCallList(ramp_list);
	glPopMatrix();

	glPushMatrix();
	aiVector3D *min = new aiVector3D(), *max = new aiVector3D();
	model2->get_bounding_box(min, max);
	//cout << '(' << min->x << ','<< min->y << ',' << min->z << ") ; (" << max->x << ','<< max->y << ',' << max->z << ')' << endl;
	//cout << max->x - min->x << ',' << max->y - min->y <<endl;
	//Supongo que la moto esta alineada respecto a Y y Z
	//(-0.325536,-0.00288519,-0.0700651) ; (0.306101,0.363991,0.0743123)

	glTranslated(inicioNivel[0] + (max->x - min->x) / 2, 0, 0);

	glDisable(GL_COLOR_MATERIAL);
	model2->draw(angulo , manejador->getAnguloVertical());
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);



	glPopMatrix();

	glPushMatrix();
	GLdouble x = (2.1 / 12.1) * ((max->x) - (min->x)) - 10;
	GLdouble y = (1.9 / 7.0) * ((max->y) - (min->y));
	GLdouble z = max->z;

//	cout << max->x - min->x << ',' << max->y - min->y <<endl;
//	cout << x << ' ' << y << ' ' << z << endl;

	glPointSize(10);
	glBegin(GL_POINTS);

	glColor3d(0, 1, 0);

	glVertex3d(-10, 0, 0);
	glColor3d(0, 0, 1);
	glVertex3d(x, y, z);
	glEnd();

	glPopMatrix();

	delete min;
	delete max;


    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texturaConcreto);

	glBegin(GL_QUADS);
	glTexCoord2d(0,0);
	glColor3d(0.5, 0.5, 0.5);
	glVertex3d(-20, 0, -20);
	glTexCoord2d(0,1);
	glVertex3d(-20,  0,20);
	glTexCoord2d(1,1);
	glVertex3d(20,  0,20);
	glTexCoord2d(1,0);
	glVertex3d(20,  0,-20);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	//glEnd();

	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR) {
		cerr << "OpenGL error: " << err << endl;

	}


	/* Draw it to the screen */
	SDL_GL_SwapBuffers();


	/* Gather our frames per second */
	Frames++;
	{
		GLint t = SDL_GetTicks();
		if (t - T0 >= 5000) {
			GLfloat seconds = (t - T0) / 1000.0;
			GLfloat fps = Frames / seconds;
			printf("%d frames in %g seconds = %g FPS\n", Frames, seconds, fps);
			T0 = t;
			Frames = 0;
		}
	}
	return (TRUE);
}

void configurarFisica() {
	manejador = new ManejadorFisica();
	manejador->establecerGravedad(0.5);
	registrarRampas();
}



int main(int argc, char *argv[]) {

	configurarFisica();
	/* Flags to pass to SDL_SetVideoMode */
	int video_flags;
	/* main loop variable */
	int done = FALSE;
	/* used to collect events */
	SDL_Event event;
	/* this holds some info about our display */
	const SDL_VideoInfo *video_info;
	/* whether or not the window is active */
	int is_active = TRUE;
	//model::Model* model;
	string nombre = "moto3.3ds";
	string basePath= "res/Models/";

	string path = (basePath + nombre).c_str();
	string  path_try2 = ("../../res/Models/" + nombre);
	string  path_try3 = ("../res/Models/" + nombre);
	string  path_try4 = ("./" + nombre);

	model2 = new model::Model();
	//Variables del juego
	Uint32 tiempo, dt;
	//Aceleracion es la variacion en la velocidad por segundo.
	//Rozamiento es una "aceleracion negativa";
	//Angulo es la inclinacion de la moto respecto a la rueda trasera
	//Desacel es lo que desacelera la moto
	GLdouble acelX = 0.6, acelY = 0.00, desacelX = -0.15, desacelY = -0.00,
			rozamientoX = -0.1, rozamientoY = -0.00;
	GLdouble frenoX = -1.0;

	int pantallaX = SCREEN_WIDTH;
	int pantallaY = SCREEN_HEIGHT;

	/* initialize SDL */
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
		fprintf(stderr, "Video initialization failed: %s\n", SDL_GetError());
		quit(1);
	}

	/* Fetch the video info */
	video_info = SDL_GetVideoInfo();

	if (!video_info) {
		fprintf(stderr, "Video query failed: %s\n", SDL_GetError());
		quit(1);
	}

	/* the flags to pass to SDL_SetVideoMode */
	video_flags = SDL_OPENGL; /* Enable OpenGL in SDL */
	video_flags |= SDL_GL_DOUBLEBUFFER; /* Enable double buffering */
	video_flags |= SDL_HWPALETTE; /* Store the palette in hardware */
	video_flags |= SDL_RESIZABLE; /* Enable window resizing */

	/* This checks to see if surfaces can be stored in memory */
	if (video_info->hw_available)
		video_flags |= SDL_HWSURFACE;
	else
		video_flags |= SDL_SWSURFACE;

	/* This checks if hardware blits can be done */
	if (video_info->blit_hw)
		video_flags |= SDL_HWACCEL;

	/* Sets up OpenGL double buffering */
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	/* get a SDL surface */
	surface = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP,
			video_flags);

	/* Verify there is a surface */
	if (!surface) {
		fprintf(stderr, "Video mode set failed: %s\n", SDL_GetError());
		quit(1);
	}

	/* initialize OpenGL */
	init_gl();
	init_cam(cam);
	//trans(cam, create_v3f(0.0, 0.0, 3.0));
	//move_forward(cam,  1.0 );

	/* resize the initial window */
	resize_window(pantallaX, pantallaY);

	//Cargo modelo Assimp (intento varios lados)
	// nota: inverti el valor de retorno de loadasset, 1 si cargo, 0 si fallo
	model2->usarWireframe = wireframe;
	int cargo = model2->loadasset(path.c_str()) || model2->loadasset(path_try2.c_str())
			|| model2->loadasset(path_try3.c_str()) || model2->loadasset(path_try4.c_str());
	if (!cargo) {
		cout << "Couldn't load model: " << endl;
		cout << "path1" << path << endl;
		cout << "path2" << path_try2 << endl;
		cout << "path3" << path_try3 << endl;
		cout << "path4" << path_try4 << endl;
	} else {
		aiVector3D *min = new aiVector3D(), *max = new aiVector3D();
		model2->get_bounding_box(min, max);
		float dif_x = max->x - min->x;
		std::cout << "Min BB: " << min->x << ", " << min->y << ", " << min->z
				<< "\n";
		std::cout << "Max BB: " << max->x << ", " << max->y << ", " << max->z
				<< "\n";
		manejador->setTamanio(dif_x, max->y - min->y, max->z - min->z, 0, 0, 1,
				0);
		manejador->establecerPosicionMoto(inicioNivel[0] - dif_x / 2, 0, 0);

	}

	SDL_WarpMouse(pantallaX / 2, pantallaY / 2);
	SDL_ShowCursor(1); // 0 para ocultarlo

	int mouse_x_old, mouse_y_old;

	/* wait for events */
	while (!done) {
		dt = tiempo;
		tiempo = SDL_GetTicks();
		dt = tiempo - dt;
		/* handle the events in the queue */
		Uint8* keystate = SDL_GetKeyState(NULL);
		//continuous-response keys
		if (keystate[SDLK_i]) {
			rot_y(cam, 3.0);
			std::cout << "cam.y_r=" << cam.rotated_y << "\n";
		} else if (keystate[SDLK_m]) {
			rot_y(cam, -3.0);
			std::cout << "cam.y_r=" << cam.rotated_y << "\n";
		} else if (keystate[SDLK_j]) {
			rot_x(cam, -3.0);
			std::cout << "cam.x_r=" << cam.rotated_x << "\n";
		} else if (keystate[SDLK_k]) {
			rot_x(cam, 3.0);
			std::cout << "cam.x_r=" << cam.rotated_x << "\n";
		} else if (keystate[SDLK_a]) {
			strafe_right(cam, -0.1);
		} else if (keystate[SDLK_s]) {
			move_forward(cam, 0.1);
		} else if (keystate[SDLK_d]) {
			strafe_right(cam, 0.1);
		} else if (keystate[SDLK_c]) {
			move_up(cam, -0.3);
		} else if (keystate[SDLK_SPACE]) {
			move_up(cam, 0.3);
		} else if (keystate[SDLK_e]) {
			rot_z(cam, -5.0);
		} else if (keystate[SDLK_z]) {
			//model2->posY += 0.1;
			manejador->aumentarY(0.1);
		} else if (keystate[SDLK_x]) {
			manejador->aumentarY(-0.1);
			//model2->posY -= 0.1;
		} else if (keystate[SDLK_q]) {
			done = TRUE;
			break;
		}

		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_ACTIVEEVENT:
				/* Something's happend with our focus
				 * If we lost focus or we are iconified, we
				 * shouldn't draw the screen
				 */
				if (event.active.gain == 0)
					is_active = FALSE;
				else
					is_active = TRUE;
				break;
			case SDL_VIDEORESIZE:

				surface = SDL_SetVideoMode(event.resize.w, event.resize.h, 16,
						video_flags);
				init_gl();
				if (!surface) {
					fprintf(stderr,
							"Could not get a surface after resize: %s\n",
							SDL_GetError());
					quit(1);
				}
				pantallaX = event.resize.w;
				pantallaY = event.resize.h;
				resize_window(event.resize.w, event.resize.h);
				break;
			case SDL_KEYDOWN:
				handle_key_press(&event.key.keysym);
				break;
			case SDL_MOUSEBUTTONDOWN:
				/*if (event.button.button == SDL_BUTTON_LEFT) {
					//move_forward(cam,-5 * 60.0f / 1000.0f);
					cam.radio -= 5 * 60.0f / 1000.0f;
				} else if (event.button.button == SDL_BUTTON_RIGHT) {
					cam.radio += 5 * 60.0f / 1000.0f;
					//move_forward(cam,5 * 60.0f / 1000.0f);
				}*/
				if (event.button.button == SDL_BUTTON_RIGHT){
					sostengoClickDerecho = true;
					SDL_ShowCursor(0);
				}
				else if (event.button.button == SDL_BUTTON_WHEELDOWN){
					acercar_camara(cam, - (5.0f / 1000.0f  * dt));
				}
				else if (event.button.button == SDL_BUTTON_WHEELUP){
					acercar_camara(cam,   (5.0f / 1000.0f  * dt));
				}
				break;
			case SDL_MOUSEBUTTONUP:
				if (event.button.button == SDL_BUTTON_RIGHT){
					sostengoClickDerecho = false;
					SDL_ShowCursor(1);
				}
				break;
			case SDL_MOUSEMOTION: {
				// inspirado en http://lazyfoo.net/SDL_tutorials/lesson09/index.php

				int mouse_x; // event.motion.x;
				int mouse_y; // = event.motion.y;
				SDL_GetMouseState(&mouse_x, &mouse_y);
				if (sostengoClickDerecho){
					int mouse_dx = mouse_x - pantallaX / 2;
					int mouse_dy = mouse_y - pantallaY / 2;
					if (!(mouse_dx == 0 && mouse_dy == 0)) {
						float dang_x = -mouse_dx * 3.0f * dt / 1000.0;
						float dang_y = -mouse_dy * 3.0f * dt / 1000.0;

						rot_x(cam, dang_x);

						rot_y(cam, dang_y, -15, 60);
						SDL_WarpMouse(pantallaX / 2, pantallaY / 2);
					}
				}
			}
				break;

			case SDL_QUIT:
				done = TRUE;
				break;

			default:
				break;

			}
		}

		if (!done){

			//Actualizo segun tiempo transcurrido.
			//Manejo de moto

			//Aceleracion y frenado
			GLdouble acelAuxX = 0;
			GLdouble acelAuxY = 0;
			if (keystate[SDLK_RIGHT] and model2->velX >= 0) {
				acelAuxX = acelX + rozamientoX;
			} else if (keystate[SDLK_RIGHT] and model2->velX < 0) {
				acelAuxX = acelX - rozamientoX;
			} else if (keystate[SDLK_LEFT] and model2->velX > 0) {
				acelAuxX = frenoX + rozamientoX;
			} else if (keystate[SDLK_LEFT] and model2->velX <= 0) {
				acelAuxX = desacelX - rozamientoX;
			} else if (model2->velX > 0) {
				acelAuxX = rozamientoX;
			} else if (model2->velX < 0) {
				acelAuxX = -rozamientoX;
			}

			//Levantar rueda moto
			if (keystate[SDLK_UP]) {
				angulo += (GLdouble) 70 * dt / 1000;
				if (angulo > 70) {
					angulo = 70;
				}
			} else {
				angulo -= (GLdouble) 70 * dt / 1000;
				if (angulo < 0) {
					angulo = 0;
				}
			}

			GLdouble oldPosX = model2->posX;
			GLdouble oldPosY = model2->posY;
			//model2->acelerar(acelAuxX,acelAuxY,(GLdouble) dt/1000);
			//manejador->establecerPosicionMoto(model2->posX-10,model2->posY,0);
			manejador->acelerarMotoAdelante(acelAuxX);
			//std::cout << "Pos Moto: " << model2->posX << ", " << model2->posY << "\n";

			/*cam.pos.x += model2->posX - oldPosX;
			 cam.pos.y += model2->posY - oldPosY;
			 cam.pos.z = 3;*/

			//render(cam);
			if (!enPausa){
				manejador->simular(dt / 1000.0f );

				if (manejador->colisiono && model2->velX > 0) {
		//		 		model2->velX = 0;
					manejador->setVelocidadX(0);
					//model2->posX -= 0.1;
					cout << "Colisiono!!!" << "\n";
				}

			model2->posX = manejador->posX() - inicioNivel[0];
			model2->posY = manejador->posY();
			model2->velX = manejador->velX();
			model2->velY = manejador->velY();
			}

			// actualizo posicion de la camara
			cam.view_dir.x = model2->posX + inicioNivel[0]; //+ 0.5;
			cam.view_dir.y = model2->posY +  0.25;
			cam.view_dir.z = 0;
		}
		//Dibujo la escena
		if (is_active) {
			draw_gl_scene();
		}
	}

	/* clean ourselves up and exit */
	quit(0);

	/* Should never get here */
	return (0);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
		LPSTR lpCmdLine, int nCmdShow) {
	char** x = new char*();
	return main(0, x);
}
