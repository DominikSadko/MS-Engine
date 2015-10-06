#include "framework/graphic/shader.h"
#include "framework/graphic/shaderWindows.h"
#include "framework/graphic/graphic.h"
#include "framework/core/filemanager.h"
#include <stdex/format.h>

int lightLocation = 0, lightColor = 0;

Shader::Shader(const std::string& vertex, const std::string& fragment)
{
    GLuint vertexId = compile(g_fileManager.readFileString(vertex).c_str(), GL_VERTEX_SHADER);
    if(!vertexId)
    {
        LOGE("Could not compile shader %i", (int32_t)glGetError());
        return;
    }

    GLuint fragmentId = compile(g_fileManager.readFileString(fragment).c_str(), GL_FRAGMENT_SHADER);
    if(!fragmentId)
    {
        LOGE("Could not compile shader %i", (int32_t)glGetError());
        return;
    }

    m_program = glCreateProgram();
    if(!m_program)
    {
        LOGE("Could not create program.");
        return;
    }

    glAttachShader(m_program, vertexId);
    glAttachShader(m_program, fragmentId);
    glLinkProgram(m_program);

    // Check if linked
    GLint linked = 0;
    glGetProgramiv(m_program, GL_LINK_STATUS, &linked);
    if(!linked)
    {
        GLint size = 0;
        glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &size);
        if(size)
        {
            char* buffer = (char*)malloc(size);
            if(buffer)
            {
                glGetProgramInfoLog(m_program, size, NULL, buffer);
                LOGE("Could not link program %i, %s", (int32_t)glGetError(), buffer);
                delete buffer;
            }
        }

        LOGE("Could not link program %i, %i", (int32_t)glGetError(), (int32_t)linked);
        glDeleteProgram(m_program);
        m_program = 0;
        return;
    }

    m_locations[Shader::Location_Position]      = glGetAttribLocation(m_program, "a_position");
    m_locations[Shader::Location_TextureCoord]  = glGetAttribLocation(m_program, "a_texCoord");

    m_locations[Shader::Uniform_Projection]     = glGetUniformLocation(m_program, "u_projection");
    m_locations[Shader::Uniform_Translation]    = glGetUniformLocation(m_program, "u_translation");
    m_locations[Shader::Uniform_Color]          = glGetUniformLocation(m_program, "Color");
    m_locations[Shader::Uniform_Time]           = glGetUniformLocation(m_program, "Time");
    m_locations[Shader::Uniform_Resolution]     = glGetUniformLocation(m_program, "Resolution");

    if(lightColor <= 0) {
    	lightLocation     = glGetUniformLocation(m_program, "lightLocation");
    	lightColor     = glGetUniformLocation(m_program, "lightColor");
    }

    for(uint8_t tex = 0; tex <= Shader::Uniform_Max - Shader::Uniform_Texture; tex++)
        m_locations[Shader::Uniform_Texture + tex] = glGetUniformLocation(m_program, stdex::toString("tex", tex).c_str());

    glEnableVertexAttribArray(m_locations[Shader::Location_Position]);
    glEnableVertexAttribArray(m_locations[Shader::Location_TextureCoord]);

    m_matrixProjection.setIdentity();
    m_matrixTranslation.setIdentity();
    m_position = {0, 0};
}

Shader::~Shader()
{
    glDeleteProgram(m_program);
}

uint32_t Shader::compile(const char* source, uint32_t type)
{
    uint32_t shader = glCreateShader(type);
    if(!shader)
        return false;

    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    // Check if compiled
    int32_t compiled = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if(!compiled)
    {
        int32_t size = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &size);
        if(size)
        {
            char* buffer = (char*) malloc(size);
            if(buffer)
            {
                glGetShaderInfoLog(shader, size, NULL, buffer);
                LOGE("Could not compile shader %d:\n%s\n", type, buffer);
                delete buffer;
            }

            LOGE("Could not compile shader %i", glGetError());
            delete buffer;
            return false;
        }
    }

    return shader;
}

void Shader::clear()
{
    glUseProgram(0);
}

void Shader::use()
{
    glUseProgram(m_program);
}

void Shader::setImage(const Image* image, uint8_t id /*= 0*/)
{
    glUniform1i(m_locations[id], id);
    glActiveTexture(GL_TEXTURE0 + id);
    glBindTexture(GL_TEXTURE_2D, image->getTexture());
}

void Shader::setVertexPosition(float* data)
{
    glVertexAttribPointer(m_locations[Shader::Location_Position], 2, GL_FLOAT, GL_FALSE, 0, data);
}

void Shader::setCoordsPosition(float* data)
{
    glVertexAttribPointer(m_locations[Shader::Location_TextureCoord], 2, GL_FLOAT, GL_FALSE, 0, data);
}

void Shader::draw(Shader::DrawMode mode, float vertexSize)
{
    glUniformMatrix3fv(m_locations[Shader::Uniform_Projection], 1, false, m_matrixProjection.data());
    glUniformMatrix3fv(m_locations[Shader::Uniform_Translation], 1, false, m_matrixTranslation.data());
	glUniform1f(m_locations[Shader::Uniform_Time], getTicks());
	glUniform2f(m_locations[Shader::Uniform_Resolution], g_graphic->getDrawDimension().w, g_graphic->getDrawDimension().h);

	glUniform2f(lightLocation, 0.5, 0.5);
	glUniform3f(lightColor, 1.f, 0.f, 1.f);


    if(mode == DrawMode::Triangle)
    	glDrawArrays(GL_TRIANGLES, 0, 6 * vertexSize);
    else if(mode == DrawMode::TriangleStrip)
        glDrawArrays(GL_TRIANGLE_STRIP, 0, vertexSize);
    else if(mode == DrawMode::TriangleFan)
        glDrawArrays(GL_TRIANGLE_FAN, 0, vertexSize);
    else if(mode == DrawMode::Lines)
        glDrawArrays(GL_LINES, 0, vertexSize);
    else if(mode == DrawMode::LineLoop)
        glDrawArrays(GL_LINE_LOOP, 0, vertexSize);
    else if(mode == DrawMode::LineStrip)
        glDrawArrays(GL_LINE_STRIP, 0, vertexSize);
    else if(mode == DrawMode::Points)
        glDrawArrays(GL_POINTS, 0, vertexSize);

    m_matrixTranslation.setIdentity();
}

void Shader::setProjection(int32_t width, int32_t height)
{
    m_matrixProjection = { 2.0f / width,  0.0f,           0.0f,
                           0.0f,         -2.0f / height,  0.0f,
                          -1.0f,          1.0f,           1.0f };
}

void Shader::setTranslation(float x, float y)
{
    if(x < 0)
    {
        x += (float)(m_position.x);
        y += (float)(m_position.y);
    }
    else
    {
        x -= (float)(m_position.x);
        y -= (float)(m_position.y);
    }

    Matrix3x3 transformMatrix = { 1.0f,  0.0f,  x,
                                  0.0f,  1.0f,  y,
                                  0.0f,  0.0f,  1.0f };

    setTransformMatrix(m_matrixTranslation * transformMatrix.transposed());
}

void Shader::setMatrix(float angle, bool vertical/* = false*/, bool horizontal/* = false*/)
{
    if(horizontal || vertical)
    {
        Matrix3x3 transformMatrix =  { horizontal ? -1.f : 1.f,  0.f,   0.0f,
                                       0.0f,   vertical ? -1.f : 1.f,   0.0f,
                                       0.0f,   0.0f,   1.0f };

        setTransformMatrix(m_matrixTranslation * transformMatrix.transposed());
    }

    Matrix3x3 transformMatrix = { std::cos(angle),  -std::sin(angle),  (float)(m_position.x),
                                  std::sin(angle),   std::cos(angle),  (float)(m_position.y),
                                             0.0f,              0.0f,  1.0f };

    setTransformMatrix(m_matrixTranslation * transformMatrix.transposed());
}

void Shader::scale(float x, float y)
{
	Matrix3x3 transformMatrix = {
           x,  0.0f,  0.0f,
        0.0f,     y,  0.0f,
        0.0f,  0.0f,  1.0f
    };

    setTransformMatrix(m_matrixTranslation * transformMatrix.transposed());
}

void Shader::setColor(const Color& color, uint8_t opacity /*= 0xFF*/)
{
    glUniform4f(m_locations[Shader::Uniform_Color], color.r / 255.f, color.g / 255.f, color.b / 255.f,
                                                    (color.a / 255.f) * (opacity / 255.f));
}

void Shader::init()
{
#if defined(WIN32)
    glActiveTexture     = (PFNGLACTIVETEXTUREPROC)SDL_GL_GetProcAddress("glActiveTexture");

    glCreateShader      = (PFNGLCREATESHADERPROC)SDL_GL_GetProcAddress("glCreateShader");
    glCompileShader     = (PFNGLCOMPILESHADERPROC)SDL_GL_GetProcAddress("glCompileShader");
    glShaderSource      = (PFNGLSHADERSOURCEPROC)SDL_GL_GetProcAddress("glShaderSource");
    glAttachShader      = (PFNGLATTACHSHADERPROC)SDL_GL_GetProcAddress("glAttachShader");
    glDeleteShader      = (PFNGLDELETESHADERPROC)SDL_GL_GetProcAddress("glDeleteShader");

    glCreateProgram     = (PFNGLCREATEPROGRAMPROC)SDL_GL_GetProcAddress("glCreateProgram");
    glLinkProgram       = (PFNGLLINKPROGRAMPROC)SDL_GL_GetProcAddress("glLinkProgram");
    glUseProgram        = (PFNGLUSEPROGRAMPROC)SDL_GL_GetProcAddress("glUseProgram");
    glDeleteProgram     = (PFNGLDELETEPROGRAMPROC)SDL_GL_GetProcAddress("glDeleteProgram");

    glGetProgramiv      = (PFNGLGETPROGRAMIVPROC)SDL_GL_GetProcAddress("glGetProgramiv");
    glGetShaderiv       = (PFNGLGETSHADERIVPROC)SDL_GL_GetProcAddress("glGetShaderiv");
    glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)SDL_GL_GetProcAddress("glGetProgramInfoLog");
    glGetShaderInfoLog  = (PFNGLGETSHADERINFOLOGPROC)SDL_GL_GetProcAddress("glGetShaderInfoLog");

    glEnableVertexAttribArray  = (PFNGLENABLEVERTEXATTRIBARRAYPROC)SDL_GL_GetProcAddress("glEnableVertexAttribArray");
    glUniform1i                = (PFNGLUNIFORM1IPROC)SDL_GL_GetProcAddress("glUniform1i");
    glUniform1f                = (PFNGLUNIFORM1FPROC)SDL_GL_GetProcAddress("glUniform1f");
    glUniform2f                = (PFNGLUNIFORM2FPROC)SDL_GL_GetProcAddress("glUniform2f");
    glUniform3f                = (PFNGLUNIFORM3FPROC)SDL_GL_GetProcAddress("glUniform3f");
    glUniform4f                = (PFNGLUNIFORM4FPROC)SDL_GL_GetProcAddress("glUniform4f");
    glGetUniformLocation       = (PFNGLGETUNIFORMLOCATIONPROC)SDL_GL_GetProcAddress("glGetUniformLocation");
    glGetAttribLocation        = (PFNGLGETATTRIBLOCATIONPROC)SDL_GL_GetProcAddress("glGetAttribLocation");
    glVertexAttribPointer      = (PFNGLVERTEXATTRIBPOINTERPROC)SDL_GL_GetProcAddress("glVertexAttribPointer");
    glVertexAttrib1f           = (PFNGLVERTEXATTRIB1FPROC)SDL_GL_GetProcAddress("glVertexAttrib1f");
    glUniformMatrix3fv         = (PFNGLUNIFORMMATRIX3FVPROC)SDL_GL_GetProcAddress("glUniformMatrix3fv");
#endif
}
