/*
 * =====================================================================================
 *
 *       Filename:  camera.c
 *
 *    Description:  opengl camera class implemented in c
 *
 *        Version:  1.0
 *        Created:  10/30/2017 10:42:02
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  dingmingxin (dmx), dingmingxin20@gmail.com
 *   Organization:  coding
 *
 * =====================================================================================
 */

#include <stdlib.h>
#include "camera.h"

#define CAMERA_DAFULT_YAW -90.0f
#define CAMERA_PITCH -90.0f

void
camera_init(struct camera *camera, kmVec3 pos, kmVec3 up, kmVec3 front, float yaw, float pitch) {
	camera->pos = pos;
	camera->up = up;
	camera->front = front;

	camera->yaw = yaw;
	camera->pitch = pitch;
}

static void 
camera_update_pos(struct camera *camera, kmVec3 *offset) {
	kmVec3 pos = camera->pos;
	kmVec3Add(&pos, &pos, offset);
}

void
camera_move(struct camera *camera, enum CAMERA_MOVEMENT movement, float offset) {

	kmVec3 tmp;
	kmVec3 front = camera->front;
	kmVec3 pos = camera->pos;
	kmVec3 up = camera->up;

	switch(movement) {
		case CM_BACKWARD: {
							 kmVec3Scale(&tmp, &front, -offset);
							 camera_update_pos(camera, &tmp);
						  }
			break;
		case CM_FORWARD: {
							 kmVec3Scale(&tmp, &front, offset);
							 camera_update_pos(camera, &tmp);
						 }
			break;
		case CM_LEFT: {
						  kmVec3Cross(&tmp, &front, &up);
						  kmVec3Normalize(&tmp, &tmp);
						  kmVec3Scale(&tmp, &tmp, -offset);
						  camera_update_pos(camera, &tmp);
					  }
			break;
		case CM_RIGHT: {
						  kmVec3Cross(&tmp, &front, &up);
						  kmVec3Normalize(&tmp, &tmp);
						  kmVec3Scale(&tmp, &tmp, offset);
						  camera_update_pos(camera, &tmp);
					   }
			break;
		default:
			break;
	}
}

void
camera_yaw(struct camera *camera, float new_value) {
	camera->yaw = new_value;
}

void 
camera_pitch(struct camera *camera, float new_value) {
	camera->pitch = new_value;
}

void
camera_update_angle(struct camera *camera) {
	kmVec3 front = camera->front;
	front.x = cos(kmDegreesToRadians(camera->yaw)) * cos(kmDegreesToRadians(camera->pitch));
	front.y = sin(kmDegreesToRadians(camera->pitch));
	front.z = sin(kmDegreesToRadians(camera->yaw)) * cos(kmDegreesToRadians(camera->pitch));
	kmVec3Normalize(&front, &front);
}

void
camera_view_matrix(struct camera *camera, kmMat4 *view) {
	kmMat4LookAt(view, &camera->pos, &camera->front, &camera->up);
}

