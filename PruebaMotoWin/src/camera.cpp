
#include <GL/glu.h>
#include "camera.h"
#include "math.h"
#include <iostream>


compg_vector3f create_v3f(GLfloat x, GLfloat y, GLfloat z)
{
	compg_vector3f tmp;
	tmp.x = x;
	tmp.y = y;
	tmp.z = z;
	return tmp;
}


GLfloat modulus_v3f(compg_vector3f * v)
{
        GLfloat sum = (v->x)*(v->x) + (v->y)*(v->y) + (v->z)*(v->z);
        GLfloat res = sqrt(sum);
	return res;
}


compg_vector3f normalize_v3f(compg_vector3f v)
{
	compg_vector3f res;
	GLfloat m = modulus_v3f(&v);
	if (m == 0.0f) {
                return create_v3f(0.0f, 0.0f, 0.0f);
        }
	res.x = v.x / m;
	res.y = v.y / m;
	res.z = v.z / m;
	return res;
}



compg_vector3f add_v3f(compg_vector3f v, compg_vector3f u)
{
	compg_vector3f res;
	res.x = v.x + u.x;
	res.y = v.y + u.y;
	res.z = v.z + u.z;
	return res;
}



compg_vector3f sub_v3f(compg_vector3f v, compg_vector3f u)
{
	compg_vector3f res;
	res.x = v.x - u.x;
	res.y = v.y - u.y;
	res.z = v.z - u.z;
	return res;
}


compg_vector3f mul_scalar(compg_vector3f v, GLfloat r)
{
	compg_vector3f res;
	res.x = v.x*r;
	res.y = v.y*r;
	res.z = v.z*r;
	return res;
}



compg_vector3f cross_product_v3f(compg_vector3f * u, compg_vector3f * v)
{
	compg_vector3f res;
	res.x = u->y*v->z - u->z*v->y;
	res.y = u->z*v->x - u->x*v->z;
	res.z = u->x*v->y - u->y*v->x;
	return res;
}



GLfloat dot_product_v3f(compg_vector3f v, compg_vector3f u)
{
	return v.x*u.x + v.y*u.y + v.z*u.z;
}




void init_cam(struct cam_t & cam)
{
	//cam.pos = create_v3f(-5.0, 0.3, 0.0);
	cam.pos = create_v3f(-1, 0.5, 1);
	cam.view_dir = create_v3f(-10, 0.3,-1.0);
	cam.right_vec = create_v3f(1.0, 0.0, 0.0);
	cam.up_vec = create_v3f(0.0, 1.0, 0.0);
        cam.rotated_x = 0.0;
        cam.rotated_y = 10.0;
        cam.rotated_z = 0.0;
        cam.radio = 1.5;
}



void trans(struct cam_t & cam, compg_vector3f dir)
{
	cam.pos = add_v3f(cam.pos, dir);
}





void rot_x(struct cam_t & cam, GLfloat angle)
{
	cam.rotated_x += angle;
        /*compg_vector3f vleft = mul_scalar(cam.view_dir, cos(angle*PIdiv180));
        compg_vector3f vright = mul_scalar(cam.up_vec, sin(angle*PIdiv180));
        compg_vector3f view = add_v3f(vleft, vright);
	cam.view_dir = normalize_v3f(view);*/
}

void rot_x(struct cam_t & cam, GLfloat angle, GLfloat min, GLfloat max)
{
	if (cam.rotated_x + angle < min) angle = cam.rotated_x - min;
	if (cam.rotated_x + angle > max) angle = max - cam.rotated_x;
	cam.rotated_x += angle;
    /*    compg_vector3f vleft = mul_scalar(cam.view_dir, cos(angle*PIdiv180));
        compg_vector3f vright = mul_scalar(cam.up_vec, sin(angle*PIdiv180));
        compg_vector3f view = add_v3f(vleft, vright);
	cam.view_dir = normalize_v3f(view);
	cam.up_vec = mul_scalar(cross_product_v3f(&cam.view_dir, &cam.right_vec), -1);*/
}

void rot_y(struct cam_t & cam, GLfloat angle, GLfloat min, GLfloat max)
{

	if (cam.rotated_y + angle < min) angle = cam.rotated_y - min;
	if (cam.rotated_y + angle > max) angle = max - cam.rotated_y;
	cam.rotated_y += angle;
    /*    compg_vector3f vleft = mul_scalar(cam.view_dir, cos(angle*PIdiv180));
        compg_vector3f vright = mul_scalar(cam.right_vec, sin(angle*PIdiv180));
        compg_vector3f view = sub_v3f(vleft, vright);
	cam.view_dir = normalize_v3f(view);
	cam.right_vec = cross_product_v3f(&cam.view_dir, &cam.up_vec);*/
}



void rot_y(struct cam_t & cam, GLfloat angle)
{
	cam.rotated_y += angle;
        /*compg_vector3f vleft = mul_scalar(cam.view_dir, cos(angle*PIdiv180));
        compg_vector3f vright = mul_scalar(cam.right_vec, sin(angle*PIdiv180));
        compg_vector3f view = sub_v3f(vleft, vright);
	cam.view_dir = normalize_v3f(view);
	cam.right_vec = cross_product_v3f(&cam.view_dir, &cam.up_vec);*/
}




void rot_z(struct cam_t & cam, GLfloat angle)
{
	cam.rotated_z += angle;
        compg_vector3f vleft = mul_scalar(cam.right_vec, cos(angle*PIdiv180));
        compg_vector3f vright = mul_scalar(cam.up_vec, sin(angle*PIdiv180));
        compg_vector3f right = add_v3f(vleft, vright);
	cam.right_vec = normalize_v3f(right);
	cam.up_vec = mul_scalar(cross_product_v3f(&cam.view_dir, &cam.right_vec), -1);
}




void render(struct cam_t cam)
{
	cam.pos.x = -cam.radio * cos(cam.rotated_y * PIdiv180) * cos(cam.rotated_x * PIdiv180);
	cam.pos.y = cam.radio * sin(cam.rotated_y * PIdiv180);
	cam.pos.z = cam.radio * cos(cam.rotated_y * PIdiv180) * sin(cam.rotated_x * PIdiv180);
	compg_vector3f ViewPoint = add_v3f(cam.pos, cam.view_dir);
	// ViewPoint = add_v3f(ViewPoint, cam.view_dir);
	gluLookAt(ViewPoint.x, ViewPoint.y, ViewPoint.z,
                        cam.view_dir.x, cam.view_dir.y, cam.view_dir.z,
                        cam.up_vec.x, cam.up_vec.y, cam.up_vec.z);
}




void move_forward(struct cam_t & cam, GLfloat dist)
{
        compg_vector3f v = mul_scalar(cam.view_dir, -dist);
        cam.pos = add_v3f(cam.pos, v);
}



void strafe_right(struct cam_t & cam, GLfloat dist)
{
        compg_vector3f r = mul_scalar(cam.right_vec, dist);
        cam.pos = add_v3f(cam.pos, r);
}



void move_up(struct cam_t & cam, GLfloat dist)
{
        compg_vector3f u = mul_scalar(cam.up_vec, dist);
        cam.pos = add_v3f(cam.pos, u);
}


void acercar_camara(struct cam_t& cam, GLfloat frac){
	frac = 1 - frac;
	if (cam.radio * frac < 0.25) cam.radio = 0.25;
	else if (cam.radio * frac > 10) cam.radio = 10;
	else cam.radio *= frac;
}

