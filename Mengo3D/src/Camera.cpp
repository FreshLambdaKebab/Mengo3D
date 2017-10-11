#include "Camera.h"

Camera::Camera() :
	m_posX(0.0f),
	m_posY(0.0f),
	m_posZ(0.0f),
	m_rotX(0.0f),
	m_rotY(0.0f),
	m_rotZ(0.0f)
{
}

Camera::~Camera()
{
}

void Camera::Render()
{
	XMFLOAT3 up, position, lookAt;
	XMVECTOR upVector, positionVector, lookAtVector;
	float yaw, pitch, roll;
	XMMATRIX rotationMatrix;

	//setup the vector that points upwards
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	//load it into a xmvector structure
	upVector = XMLoadFloat3(&up);

	//setup the position of the camera in the world
	position.x = m_posX;
	position.y = m_posY;
	position.z = m_posZ;

	//load into xmvector structure
	positionVector = XMLoadFloat3(&position);

	//setup where the camera is looking at by default
	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;

	lookAtVector = XMLoadFloat3(&lookAt);

	//set the yaw pitch and roll rotations in radians
	pitch = m_rotX * 0.0174532925f;
	yaw = m_rotY * 0.0174532925f;
	roll = m_rotZ * 0.0174532925f;

	//create the rotation matrix from the yaw pitch and roll values
	rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	//transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin
	lookAtVector = XMVector3TransformCoord(lookAtVector, rotationMatrix);
	upVector = XMVector3TransformCoord(upVector, rotationMatrix);

	//translate the rotated camera pos to the location of the viewer
	lookAtVector = XMVectorAdd(positionVector, lookAtVector);

	//create the view matrix from the three updated vectors
	m_viewMatrix = XMMatrixLookAtLH(positionVector, lookAtVector, upVector);

}

void Camera::SetPosition(float x, float y, float z)
{
	m_posX = x;
	m_posY = y;
	m_posZ = z;
}

void Camera::SetRotation(float x, float y, float z)
{
	m_rotX = x;
	m_rotY = y;
	m_rotZ = z;
}
