#include "Camera.h"



Camera::Camera()
{

	position = { 0,0,0 };
	lookAt = { 0,0,0 };
	forward = { 0,0,0 };
	up = { 0,1,0 };

	Yaw = 0;
	Pitch = 0;
	Roll = 0;

	moveSpeed = 1.0f;

}


Camera::~Camera()
{
}

void Camera::update(float dt) {
	float cosR, cosP, cosY;
	float sinR, sinP, sinY;

	cosY = cosf(Yaw*3.1415 / 180);
	cosP = cosf(Pitch*3.1415 / 180);
	cosR = cosf(Roll*3.1415 / 180);
	sinY = sinf(Yaw*3.1415 / 180);
	sinP = sinf(Pitch*3.1415 / 180);
	sinR = sinf(Roll*3.1415 / 180);

	forward.x = sinY*cosP;
	forward.y = sinP;
	forward.z = cosP*-cosY;

	up.x = -cosY * sinR - sinY * sinP * cosR;
	up.y = cosP * cosR;
	up.z = -sinY * sinR - sinP * cosR * -cosY;

	right = forward.cross(up);

	lookAt = position.operator+(forward);

}

void Camera::moveForward(float dt) {
	Vector3 movement = { forward.x*(dt*moveSpeed), forward.y*(dt*moveSpeed), forward.z*(dt*moveSpeed) };
	position.operator+=(movement);
}

void Camera::moveUp(float dt) {
	Vector3 movement = { up.x*(dt*moveSpeed), up.y*(dt*moveSpeed), up.z*(dt*moveSpeed) };
	position.operator+=(movement);
}

void Camera::moveRight(float dt) {
	Vector3 movement = { right.x*(dt*moveSpeed), right.y*(dt*moveSpeed), right.z*(dt*moveSpeed) };
	position.operator+=(movement);
}

void Camera::movePYR(float p, float y, float r, float dt) {
	Pitch += p*dt;
	Yaw += y*dt;
	Roll += r*dt;
}

Vector3 Camera::getPosition() {
	return position;
}
Vector3 Camera::getLookAt() {
	return lookAt;
}
Vector3 Camera::getUp() {
	return up;
}

Vector3 Camera::getRight() {
	return right;
}