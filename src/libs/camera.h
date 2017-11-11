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
	float pos[3];
	float front[3];
	float up[3];
	float right[3];
	float worldup[3];
};

void camera_init(struct camera *camera);
void camera_move(CAMERA_MOVEMENT move_type, float offset);
void camera_view_matrix(struct camera *camera);

#endif   /* ----- #ifndef camera_INC  ----- */
