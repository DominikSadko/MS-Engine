#ifndef __GRAVITON_H__
#define __GRAVITON_H__

#include "framework/core/platform.h"

class Body;
class UIScene;

class GravityManager
{
	public:
		GravityManager();
		~GravityManager();

		void clear();
	    void update(UIScene* scene);
	    void add(Body* body, int32_t distance, int32_t strength);
	    void remove(Body* body);

	private:
	    struct Graviton_t
	    {
	    	Body* body;
	    	int32_t distance;
	    	int32_t strength;
	    };

	    std::vector<Graviton_t> m_gravitons;
};


#endif //   __GRAVITON_H__   //
