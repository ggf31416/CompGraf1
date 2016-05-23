#include <iostream>
#include <fstream>
#include <string>
#include <cmath>


#include "camera.h"
#include "Model.h"

using namespace std;
using namespace model;


void drawOrtho(){


}
//
//int main() {
//	bool fin = false;
//	SDL_Event event;
//	SDL_Surface *screen;
//	GLdouble eyeX,eyeY,eyeZ,atX,atY,atZ,upX,upY,upZ;
//	GLchar* path ="/home/bruno/eclipseWorkspace/PruebaMoto/res/Models/moto.3ds";
//	Model* model = new Model();
//
//	//Declaraciones especificas
//	GLdouble factorMovCam = 0.05,anguloMovCam = 0.05,x,z,c,s;
//
//	//SDL Inicializacion
//	if(SDL_Init(SDL_INIT_VIDEO) == -1){
//		fprintf( stderr, "[Video Error]: %s\n", SDL_GetError());
//	};
//
//	screen = SDL_SetVideoMode(640,480,32,SDL_OPENGL | SDL_HWSURFACE | SDL_DOUBLEBUF);
//
//	if(screen == NULL){
//		fprintf(stderr,"[Video Error]: %s\n", SDL_GetError() );
//		SDL_Quit();
//		exit(1);
//	};
//	SDL_EnableKeyRepeat(50,50);
//
//	//OpenGL Inicializacion
//	//glClearColor(0.0,0.0,0.0,0);
//	glClearColor(1.0,1.0,1.0,0);
//	glClearDepth(0.0);
//	glMatrixMode(GL_PROJECTION);
//	gluPerspective(45,640/480,0.1,100);
//	glMatrixMode(GL_MODELVIEW);
////	eyeX = -12.66;
////	eyeY = -8.93;
////	eyeZ = -2.12;
////	atX = 0.93;
////	atY = 0.29;
////	atZ = 0.21;
////	upX = -0.20;
////	upY = -0.04;
////	upZ = 0.98;
//	eyeX = 0.0;
//	eyeY = 0.0;
//	eyeZ = -5.0;
//	atX = 0.0;
//	atY = 0.0;
//	atZ = 0.0;
//	upX = 0.0;
//	upY = 1.0;
//	upZ = 0.0;
//
//	//Modelo
//	if(model->loadasset(path)){
//		cout << "Couldn't load model: " << endl;
//	}
//
//	//Loop principal
//	while(!fin){
//		//Chequear eventos y tomar decisiones
//		while(SDL_PollEvent(&event)>0){
//			if(event.type==SDL_QUIT || (event.type==SDL_KEYDOWN && event.key.keysym.sym==SDLK_ESCAPE)){
//				fin = true;
//			}
//			else if(event.type==SDL_KEYDOWN && event.key.keysym.sym==SDLK_F11){
//				screen = SDL_SetVideoMode(0, 0, 0, screen->flags ^ SDL_FULLSCREEN);
//			}
//			else if(event.type==SDL_KEYDOWN && event.key.keysym.sym==SDLK_UP){
//				//Muevo camara hacia "adelante"
//				//vector de avance
//				x = (atX - eyeX)*factorMovCam;
//				z = (atZ - eyeZ)*factorMovCam;
//				//Traslado at y eye utilizando el vector (x,y)
//				eyeX+=x;
//				eyeZ+=z;
//				atX+=x;
//				atZ+=z;
//			}
//			else if(event.type==SDL_KEYDOWN && event.key.keysym.sym==SDLK_DOWN){
//				//Muevo camara hacia "atras"
//				x = (atX - eyeX)*factorMovCam*(-1.0);
//				z = (atZ - eyeZ)*factorMovCam*(-1.0);
//				//Traslado at y eye utilizando el vector (x,y)
//				eyeX+=x;
//				eyeZ+=z;
//				atX+=x;
//				atZ+=z;
//			}
//			else if(event.type==SDL_KEYDOWN && event.key.keysym.sym==SDLK_LEFT){
//				x = (atX - eyeX);
//				z = (atZ - eyeZ);
//				c = cos(-anguloMovCam);
//				s = sin(-anguloMovCam);
//				atX= eyeX + x*c - z*s;
//				atZ= eyeZ + x*s + z*c;
//			}
//			else if(event.type==SDL_KEYDOWN && event.key.keysym.sym==SDLK_RIGHT){
//				x = (atX - eyeX);
//				z = (atZ - eyeZ);
//				c = cos(anguloMovCam);
//				s = sin(anguloMovCam);
//				atX= eyeX + x*c - z*s;
//				atZ= eyeZ + x*s + z*c;
//			}
//		}
//		//Actualizar segun variables
//
//
//		//Actualizar el sistema según el tiempo que pasó
//
//		//Borrar buffers de color y profundidad
//		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//		glLoadIdentity();
//
//		//Posicionar camara
//		gluLookAt(eyeX,eyeY,eyeZ,atX,atY,atZ,upX,upY,upZ);
//
//		//Dibujar
//		model->draw();
//
//
//		SDL_GL_SwapBuffers();
//	};
//
//	return 0;
//}



