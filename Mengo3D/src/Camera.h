#ifndef _CAMERA_H
#define _CAMERA_H

#include <DirectXMath.h>
using namespace DirectX;

class Camera
{
public:
	Camera();
	Camera(const Camera& other){}
	~Camera();

	void Render();

	void SetPosition(float x, float y, float z);
	void SetRotation(float x, float y, float z);

	inline const XMFLOAT3& GetPosition()const
	{
		return XMFLOAT3(m_posX, m_posY, m_posZ); 
	}
	inline const XMFLOAT3& GetRotation()const
	{
		return XMFLOAT3(m_rotX, m_rotY, m_rotZ);
	}

	inline void GetViewMatrix(XMMATRIX& viewMatrix)const { viewMatrix = m_viewMatrix; }

private:
	float m_posX, m_posY, m_posZ;
	float m_rotX, m_rotY, m_rotZ;
	XMMATRIX m_viewMatrix;

};

#endif
