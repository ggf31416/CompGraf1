#include <GL/gl.h>

#define PI 3.14159265359
#define PIdiv180 0.0174532925199


struct compg_vector3f
{
	GLfloat x;
	GLfloat y;
	GLfloat z;
};


compg_vector3f create_v3f(GLfloat x, GLfloat y, GLfloat z);


struct cam_t {
	compg_vector3f view_dir;
	compg_vector3f right_vec;	
	compg_vector3f up_vec;
	compg_vector3f pos;
	GLfloat rotated_x;	
	GLfloat rotated_y;	
	GLfloat rotated_z;	

};

void init_cam(struct cam_t & cam);
void render(struct cam_t cam);
void trans(struct cam_t & cam, compg_vector3f dir);
void rot_x(struct cam_t & cam, GLfloat angle);
void rot_x(struct cam_t & cam, GLfloat angle, GLfloat min, GLfloat max);
void rot_y(struct cam_t & cam, GLfloat angle);
void rot_y(struct cam_t & cam, GLfloat angle, GLfloat min, GLfloat max);
void rot_z(struct cam_t & cam, GLfloat angle);
void move_forward(struct cam_t & cam, GLfloat dist);
void move_up(struct cam_t & cam, GLfloat dist);
void strafe_right(struct cam_t & cam, GLfloat dist);




