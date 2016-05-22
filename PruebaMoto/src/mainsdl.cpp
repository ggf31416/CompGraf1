
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL/SDL.h>

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "camera.h"
#include "Model.h"

#define SCREEN_WIDTH  256
#define SCREEN_HEIGHT 256
#define SCREEN_BPP     16

#define TRUE  1
#define FALSE 0

using namespace std;

model::Model* model2;
SDL_Surface *surface;
GLuint ramp_list;
GLfloat inicioNivel[3] = {-10,0,0};
GLfloat finNivel[3] = {10,0,0};
GLdouble angulo = 0;//Angulo de inclinacion de la moto

void quit(int ret)
{
	SDL_Quit();
	exit(ret);
}


struct cam_t cam;

void draw_wall(GLfloat size, GLint x_lines, GLint z_lines)
{
        glBegin(GL_LINES);
                int i;
                for (i = 0; i < x_lines; i++) {
                        GLfloat xcoord = -size/2.0 + i/(GLfloat)(x_lines - 1)*size;
                        glVertex3f(xcoord, 0.0, size/2.0);
                        glVertex3f(xcoord, 0.0, size/-2.0);
                }
                int k;
                for (k = 0; k < x_lines; k++) {
                        GLfloat zcoord = -size/2.0 + k/(GLfloat)(z_lines - 1)*size;
                        glVertex3f(size/2.0, 0.0, zcoord);
                        glVertex3f(size/-2.0, 0.0, zcoord);
                }
        glEnd();
}




/* function to reset our viewport after a window resize */
int resize_window(int width, int height)
{
	if (height == 0) {
                height = 1;
        }
	GLfloat ratio = (GLfloat)width / (GLfloat)height;
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
        gluPerspective(45.0f, ratio, 0.1f, 100.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	return (TRUE);
}




/* function to handle key press events */
void handle_key_press(SDL_keysym * keysym)
{
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
	default:
		break;
	}

	return;
}




int init_gl()
{
        static GLfloat vertices[] = {
                -0.25, -3.0, -0.25,
                 0.25, -3.0, -0.25,
                 0.25,  3.0, -0.25,
                -0.25,  3.0, -0.25,
                -0.25, -3.0,  0.25,
                 0.25, -3.0,  0.25,
                 0.25,  1.0,  0.25,
                -0.25,  1.0,  0.25,
        };

        static GLubyte front_indices[] = {4, 5, 6, 7};
        static GLubyte right_indices[] = {1, 2, 6, 5};
        static GLubyte bottom_indices[] = {0, 1, 5, 4};
        static GLubyte back_indices[] = {0, 3, 2, 1};
        static GLubyte left_indices[] = {0, 4, 7, 3};
        static GLubyte top_indices[] = {2, 3, 7, 6};

        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, vertices);

        ramp_list = glGenLists(1);
        glNewList(ramp_list, GL_COMPILE);
                glColor3f(   0.0f,  1.0f,  0.0f ); /* Set The Color To Green           */
                glDrawElements(GL_QUADS, 4, GL_UNSIGNED_BYTE, front_indices);

                glColor3f(   1.0f,  0.5f,  0.0f ); /* Set The Color To Orange          */
                glDrawElements(GL_QUADS, 4, GL_UNSIGNED_BYTE, right_indices);

                glColor3f(   1.0f,  0.0f,  0.0f ); /* Set The Color To Red             */
                glDrawElements(GL_QUADS, 4, GL_UNSIGNED_BYTE, bottom_indices);

                glColor3f(   1.0f,  1.0f,  0.0f ); /* Set The Color To Yellow          */
                glDrawElements(GL_QUADS, 4, GL_UNSIGNED_BYTE, back_indices);

                glColor3f(   0.0f,  0.0f,  1.0f ); /* Set The Color To Blue            */
                glDrawElements(GL_QUADS, 4, GL_UNSIGNED_BYTE, left_indices);

                glColor3f(   1.0f,  0.0f,  1.0f ); /* Set The Color To Violet          */
                glDrawElements(GL_QUADS, 4, GL_UNSIGNED_BYTE, top_indices);
        glEndList();



	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);
        glShadeModel(GL_SMOOTH);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	return (TRUE);
}




int draw_gl_scene()
{
	/* These are to calculate our fps */
	static GLint T0 = 0;
	static GLint Frames = 0;
        static GLfloat backtrans = 0.0f;
        static GLfloat middletrans = 0.0f;
        static GLfloat fronttrans = 0.0f;
        static GLuint prev_sind = 0;




/*start drawing box*/
	//glClear(GL_COLOR_BUFFER_BIT);
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glLoadIdentity();
	render(cam);
	glPushMatrix();
	//render(cam);
	glTranslatef(0.0, -0.5, -6.0);
	glScalef(3.0, 1.0, 3.0);
	GLfloat size = 8.0;
	GLint x_lines = 30;
	GLint z_lines = 30;
	GLfloat halfsize = size/2.0;
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
        glCallList(ramp_list);
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
    aiVector3D *min = new aiVector3D(),*max = new aiVector3D();
    model2->get_bounding_box(min,max);
    //cout << '(' << min->x << ','<< min->y << ',' << min->z << ") ; (" << max->x << ','<< max->y << ',' << max->z << ')' << endl;
    //cout << max->x - min->x << ',' << max->y - min->y <<endl;
    //Supongo que la moto esta alineada respecto a Y y Z
    //(-0.325536,-0.00288519,-0.0700651) ; (0.306101,0.363991,0.0743123)


    glTranslated(inicioNivel[0]+(max->x-min->x)/2,0,0);

    model2->draw(angulo);

    glPopMatrix();

	glPushMatrix();
	GLdouble x = (2.1/12.1)*((max->x)-(min->x))-10;
	GLdouble y = (1.9/7.0)*((max->y)-(min->y));
	GLdouble z = max->z;

//	cout << max->x - min->x << ',' << max->y - min->y <<endl;
//	cout << x << ' ' << y << ' ' << z << endl;

	glPointSize(10);
	glBegin(GL_POINTS);
		glColor3d(0,1,0);
		glVertex3d(-10,0,0);
		glColor3d(0,0,1);
		glVertex3d(x,y,z);
	glEnd();

	glPopMatrix();

	delete min;
	delete max;


	/* Draw it to the screen */
	SDL_GL_SwapBuffers();

	/* Gather our frames per second */
	Frames++;
	{
		GLint t = SDL_GetTicks();
		if (t - T0 >= 5000) {
			GLfloat seconds = (t - T0) / 1000.0;
			GLfloat fps = Frames / seconds;
			printf("%d frames in %g seconds = %g FPS\n",
                                Frames, seconds, fps);
			T0 = t;
			Frames = 0;
		}
	}
	return (TRUE);
}

int main(int argc, char **argv)
{
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
	const GLchar* path ="../res/Models/moto3.3ds";
	model2 = new model::Model();
	//Variables del juego
	Uint32 tiempo,dt;
	//Aceleracion es la variacion en la velocidad por segundo.
	//Rozamiento es una "aceleracion negativa";
	//Angulo es la inclinacion de la moto respecto a la rueda trasera
	//Desacel es lo que desacelera la moto
	GLdouble acelX=0.3,acelY=0.00,desacelX=-0.06,desacelY=-0.00,rozamientoX=-0.05,rozamientoY=-0.00;
	GLdouble frenoX = -1.0;


	/* initialize SDL */
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
		fprintf(stderr, "Video initialization failed: %s\n",
			SDL_GetError());
		quit(1);
	}

	/* Fetch the video info */
	video_info = SDL_GetVideoInfo();

	if (!video_info) {
		fprintf(stderr, "Video query failed: %s\n",
			SDL_GetError());
		quit(1);
	}

	/* the flags to pass to SDL_SetVideoMode */
	video_flags = SDL_OPENGL;	/* Enable OpenGL in SDL */
	video_flags |= SDL_GL_DOUBLEBUFFER;	/* Enable double buffering */
	video_flags |= SDL_HWPALETTE;	/* Store the palette in hardware */
	video_flags |= SDL_RESIZABLE;	/* Enable window resizing */

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
		fprintf(stderr, "Video mode set failed: %s\n",
			SDL_GetError());
		quit(1);
	}

	/* initialize OpenGL */
	init_gl();
        init_cam(cam);
	trans(cam, create_v3f(0.0, 0.0, 3.0));
	move_forward(cam,  1.0 );

	/* resize the initial window */
	resize_window(SCREEN_WIDTH, SCREEN_HEIGHT);

	//Cargo modelo Assimp
	if(model2->loadasset(path)){
		cout << "Couldn't load model: " << endl;
	}

	/* wait for events */
	while (!done) {
		dt = tiempo;
		tiempo = SDL_GetTicks();
		dt = tiempo - dt;
		/* handle the events in the queue */
		Uint8* keystate = SDL_GetKeyState(NULL);
		//continuous-response keys
		if(keystate[SDLK_g]) {
				rot_y(cam, 5.0);
		} else if(keystate[SDLK_j]) {
				rot_y(cam, -5.0);
		} else if(keystate[SDLK_y]) {
				rot_x(cam, 5.0);
		} else if(keystate[SDLK_h]) {
				rot_x(cam, -5.0);
		} else if(keystate[SDLK_w]) {
				move_forward(cam,  -0.1 ) ;
		} else if(keystate[SDLK_a]) {
				strafe_right(cam, -0.1);
		} else if(keystate[SDLK_s]) {
				move_forward(cam,  0.1 ) ;
		} else if(keystate[SDLK_d]) {
				strafe_right(cam, 0.1);
		} else if(keystate[SDLK_c]) {
				move_up(cam, -0.3);
		} else if(keystate[SDLK_SPACE]) {
				move_up(cam, 0.3);
		} else if(keystate[SDLK_e]) {
				rot_z(cam, -5.0);
		} else if(keystate[SDLK_q]) {
				rot_z(cam, 5.0);
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
				surface = SDL_SetVideoMode(event.resize.w,
							   event.resize.h,
							   16, video_flags);
				if (!surface) {
					fprintf(stderr,
						"Could not get a surface after resize: %s\n",
						SDL_GetError());
					quit(1);
				}
				resize_window(event.resize.w,
					     event.resize.h);
				break;
			case SDL_KEYDOWN:
				handle_key_press(&event.key.keysym);
				break;
			case SDL_QUIT:
				done = TRUE;
				break;
			default:
				break;
			}
		}

		//Actualizo segun tiempo transcurrido.
		//Manejo de moto

		//Aceleracion y frenado
		GLdouble acelAuxX = 0;
		GLdouble acelAuxY = 0;
		 if(keystate[SDLK_RIGHT]and model2->velX >= 0) {
			 acelAuxX = acelX+rozamientoX;
		 }
		 else if(keystate[SDLK_RIGHT] and model2->velX < 0) {
		 	acelAuxX = acelX-rozamientoX;
		 }
		 else if(keystate[SDLK_LEFT] and model2->velX > 0) {
			acelAuxX = frenoX+rozamientoX;
		 }
		 else if(keystate[SDLK_LEFT] and model2->velX <= 0){
			 acelAuxX = desacelX-rozamientoX;
		 }
		 else if(model2->velX > 0){
		 	 acelAuxX = rozamientoX;
		 }
		 else if(model2->velX < 0){
		 	acelAuxX = -rozamientoX;
		 }

		 //Levantar rueda moto
		 if(keystate[SDLK_UP]) {
		 	angulo += (GLdouble)70*dt/1000;
		 	if(angulo > 70){
		 		angulo = 70;
		 	}
		 }
		 else{
			angulo -= (GLdouble)70*dt/1000;
			if(angulo < 0){
				angulo = 0;
			}
		 }

		 model2->acelerar(acelAuxX,acelAuxY,(GLdouble) dt/1000);

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
