#ifndef __FIXTURE_H__
#define __FIXTURE_H__

#include "framework/core/platform.h"

#include "framework/lua/lua.h"
#include <Box2D/Box2D.h>

struct Pattern
{
	public:
		Pattern()
		{
			offset = 0;
			animations = 0;
			time = 0;
			repeatable = false; //todo
		}

		uint8_t offset;
		uint8_t animations;
		uint16_t time;
		bool repeatable;
};

class Body;
class Fixture : public Lua::Shared
{
	public:
		Fixture(b2Fixture* fixture, const stdex::Vertices& vertices);
		~Fixture();

		static Fixture* Vertice(Body* body, const stdex::Vertices& vertices);
		static Fixture* Circle(Body* body, int32_t radius);
		static Fixture* Edge(Body* body, const Point& from, const Point& to);
		static Fixture* Square(Body* body, const Rect& rect);

		b2Fixture* getB2Fixture() const { return m_fixture; };
		Fixture* getNext() const;
		Body* getBody() const;

		uint8_t getType() const;
		bool isSensor() const;
		int32_t getDensity() const;
		int32_t getFriction() const;
		int32_t getRestitution() const;

		void setFilterData(uint16_t category, uint16_t mask);
		void setSensor(bool sensor);
		void setDensity(int32_t density);
		void setFriction(int32_t friction);
		void setRestitution(int32_t restitution);

		void setPolygonVertices(const stdex::Vertices& vertices);
		void setCircleSize(int32_t radius);
		void setEdgeSize(const Point& from, const Point& to);
		void setSquareSize(const Rect& rect);

		void addImageCoords(const stdex::Vertices& vertices);
		void setImageCoords(const stdex::Vertices& vertices);
		void updateImageCoords(const Image* image);
		void eraseImageCoords();

		void addPattern(uint8_t offset, uint8_t animations, uint16_t time);
		void setPattern(uint8_t pattern);

	private:
		b2Fixture* m_fixture;
		stdex::Vertices m_vertices;
		std::vector<stdex::Vertices> m_coords;
		std::vector<Pattern> m_patterns;
		uint8_t m_pattern;

	public:
		void setPhantom(bool phantom) { m_phantom = phantom; }
		bool isPhantom() { return m_phantom; }

	private:
		bool m_phantom;

		friend class Body;
};

#endif // __FIXTURE_H__ //
