#ifndef __SHADER_H__
#define __SHADER_H__

#include "framework/core/platform.h"
#include "framework/image/image.h"
#include "framework/graphic/matrix.h"

class Shader : public Lua::Shared
{
	public:
		enum DrawMode
		{
			Triangle = 0,
			TriangleStrip = 1,
			TriangleFan = 2,
			Lines = 3,
			LineLoop = 4,
			LineStrip = 5,
			Points = 6
		};

	private:
		enum Location
		{
			Location_Position = 0,
			Location_TextureCoord = 1,
			Uniform_Projection = 2,
			Uniform_Translation = 3,
			Uniform_Color = 4,
			Uniform_Time = 5,
			Uniform_Resolution = 6,
			Uniform_Texture = 7,

			Uniform_Max = 10
		};

    public:
        Shader(const std::string& vertex, const std::string& fragment);
        virtual ~Shader();

        static void init();

        static void clear();

        void use();
        void draw(Shader::DrawMode mode, float vertexSize);
        void setImage(const Image* image, uint8_t id = 0);
        void setVertexPosition(float* data);
        void setCoordsPosition(float* data);
        void setProjection(int32_t width, int32_t height);
        void setTranslation(float x, float y);
        void setPosition(const Point& point) { m_position = point; };

        void setMatrix(float angle, bool vertical = false, bool horizontal = false);
        void scale(float x, float y);
        void setColor(const Color& color, uint8_t opacity = 0xFF);

        uint32_t getProgram() const { return m_program; }

    private:
        uint32_t compile(const char* source, uint32_t type);

        void setTransformMatrix(const Matrix3& transformMatrix) { m_matrixTranslation = transformMatrix; }

    private:
        uint32_t m_program;
        uint32_t m_locations[Shader::Uniform_Max + 1];

        Matrix3 m_matrixTranslation;
        Matrix3 m_matrixProjection;

        Point m_position;
};

#endif // __SHADER_H__ //
