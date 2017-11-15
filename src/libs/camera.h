/*
 * =====================================================================================
 *
 *       Filename:  camera.h
 *
 *    Description:  opengl camera class implemented in c
 *
 *        Version:  1.0
 *        Created:  10/30/2017 10:41:40
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  dingmingxin (dmx), dingmingxin20@gmail.com
 *   Organization:  coding
 *
 * =====================================================================================
 */

#ifndef  camera_INC
#define  camera_INC

#include "kazmath/kazmath/vec2.h"
#include "kazmath/kazmath/vec3.h"
#include "kazmath/kazmath/vec4.h"
#include "kazmath/kazmath/mat4.h"
#include "kazmath/kazmath/GL/matrix.h"
#include "kazmath/kazmath/utility.h"

enum CAMERA_MOVEMENT {
	CM_FORWARD,
	CM_BACKWARD,
	CM_LEFT,
	CM_RIGHT
};

struct camera {
	//Movement
	float move_speed;
	float move_sensitivity;
	float zoom;

	//Eular Angles
	float pitch;
	float yaw;
	float roll;

	//Positions
	kmVec3 pos;
	kmVec3 front;
	kmVec3 up;
	kmVec3 right;
	kmVec3 worldup;
};

void camera_init(struct camera *camera, kmVec3 pos, kmVec3 up, kmVec3 front, float yaw, float pitch);
void camera_move(struct camera *camera, enum CAMERA_MOVEMENT move_type, float offset);
void camera_view_matrix(struct camera *camera, kmMat4 *view);
void camera_pitch(struct camera *camera, float new_value);
void camera_yaw(struct camera *camera, float new_value);
void camera_update_angle(struct camera *camera);

#endif   /* ----- #ifndef camera_INC  ----- */
