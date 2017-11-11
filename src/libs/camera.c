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
camera_init(struct camera *camera, float *pos, float *up, float *front, float yaw, float pitch) {
	camera->pos[0] = pos[0];
	camera->pos[1] = pos[1];
	camera->pos[2] = pos[2];

	camera->up[0] = up[0];
	camera->up[1] = up[1];
	camera->up[2] = up[2];

	camera->front[0] = front[0];
	camera->front[1] = front[1];
	camera->front[2] = front[2];

	camera->yaw = yaw;
	camera->pitch = pitch;
}

void
camera_move(CAMERA_MOVEMENT move, float offset) {

}

void
camera_view_matrix(struct camera *camera) {
}
