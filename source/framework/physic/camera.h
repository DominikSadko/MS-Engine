#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "framework/core/platform.h"

class Body;
class UIScene;

class CameraManager
{
	public:
		CameraManager();
		~CameraManager();

		Point update(UIScene* scene);
	    void set(Body* body);
	    Body* get() const;

	    void setOffset(const Point offset) { m_offset = offset; }
	    Point getOffset() const { return m_offset; }

	private:
	    Point m_position, m_offset;
	    Body* m_followBody;
};


#endif //   __CAMERA_H__   //
