#pragma once
#include "Vector3.h"
#include "Input.h"
class Camera
{
public:
	Camera();
	~Camera();

	void update(float dt);

	void moveForward(float dt);
	void moveUp(float dt);
	void moveRight(float dt);

	void movePYR(float p, float y, float r, float dt);

	//TEST
	float tempYaw;
	//***

	Vector3 getPosition();
	Vector3 getLookAt();
	Vector3 getUp();

	Vector3 getRight();

protected:
	Vector3 position, lookAt, up;
	Vector3 forward, right;
	float Yaw, Pitch, Roll;
	float moveSpeed;
	Input* input;

};