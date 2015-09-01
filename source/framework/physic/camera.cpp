#include "framework/physic/camera.h"

#include "framework/physic/uiscene.h"
#include "framework/physic/body.h"

CameraManager::CameraManager()
{
	m_position = {0, 0};
	m_offset = {0, 0};
	m_followBody = nullptr;
}

CameraManager::~CameraManager()
{

}

Point CameraManager::update(UIScene* scene)
{
	if(!m_followBody)
		return m_offset;

	Point pos = m_followBody->getPosition();
	m_position = { pos.x + m_offset.x, pos.y + m_offset.y };

	return m_position;
}

void CameraManager::set(Body* body)
{
	m_followBody = body;
}

Body* CameraManager::get() const
{
	return m_followBody;
}
