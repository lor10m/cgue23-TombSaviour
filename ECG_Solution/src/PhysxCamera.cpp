#include "PhysxCamera.h"

PhysxCamera::PhysxCamera(){}

PhysxCamera::PhysxCamera(double fov, double aspect_ratio, double near, double far) {
	m_fov = fov;
	m_aspectRatio = aspect_ratio;
	m_nearClip = near;
	m_farClip = far;
}

void PhysxCamera::setAspect(float aspectRatio) {
	m_aspectRatio = aspectRatio;
}

void PhysxCamera::setFOV(float fov) {
	m_fov = fov;
}

void PhysxCamera::setNearClip(float nearClip) {
	m_nearClip = nearClip;
}

void PhysxCamera::setFarClip(float farClip) {
	m_farClip = farClip;
}

void PhysxCamera::setTransform(PxTransform& transform) {
	m_transform = transform;
}

void PhysxCamera::lookAt(PxVec3& target, PxVec3& up) {
	PxVec3 eye = m_transform.p;
	glm::vec3 glm_eye(eye.x, eye.y, eye.z);

	glm::vec3 glm_target(target.x, target.y, target.z);

	PxVec3 upVector = up.getNormalized();
	glm::vec3 glm_up(upVector.x, upVector.y, upVector.z);

	glm::mat4 viewMatrix = glm::lookAt(glm_eye, glm_target, glm_up);

	PxMat44 newTransform(PxVec4(viewMatrix[0][0], viewMatrix[1][0], viewMatrix[2][0], viewMatrix[3][0]),
						 PxVec4(viewMatrix[0][1], viewMatrix[1][1], viewMatrix[2][1], viewMatrix[3][1]),
						 PxVec4(viewMatrix[0][2], viewMatrix[1][2], viewMatrix[2][2], viewMatrix[3][2]),
						 PxVec4(viewMatrix[0][3], viewMatrix[1][3], viewMatrix[2][3], viewMatrix[3][3]));

	m_transform = PxTransform(newTransform);
}

PxTransform& PhysxCamera::getTransform() {
	return m_transform;
}

PxVec3 PhysxCamera::getForwardVector() {
	return m_transform.q.rotate(PxVec3(0.0f, 0.0f, -1.0f));
}

PxVec3 PhysxCamera::getRightVector() {
	return m_transform.q.rotate(PxVec3(1.0f, 0.0f, 0.0f));
}

glm::mat4 PhysxCamera::getViewMatrix() {
	glm::vec3 eye(m_transform.p.x, m_transform.p.y, m_transform.p.z);
	glm::vec3 center(m_transform.p.x, m_transform.p.y, m_transform.p.z - 1.0f);
	glm::vec3 up(m_transform.q.rotate(PxVec3(0.0f, 1.0f, 0.0f)).x, m_transform.q.rotate(PxVec3(0.0f, 1.0f, 0.0f)).y, m_transform.q.rotate(PxVec3(0.0f, 1.0f, 0.0f)).z);
	return glm::lookAt(eye, center, up);
}