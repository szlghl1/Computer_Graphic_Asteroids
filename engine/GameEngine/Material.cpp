//
//  Material.cpp
//  GameEngine
//
//  Created by David Lively on 2/3/16.
//  Copyright © 2016 David Lively. All rights reserved.
//

#include "Material.h"
#include "Enums.h"
#include "Files.h"

#include <string>
using namespace std;



void Material::SetUniforms(const GameTime& time)
{
	SetUniform("GameTimeTotalSeconds", time.TotalSeconds());
	SetUniform("TimeScale", 0.5f);
}


bool Material::Build(string vertexShaderSource, string fragmentShaderSource, string geometryShaderSource)
{
	check_gl_error();

	GLuint vertexShader = gl::CreateShader((GLenum)ShaderType::VertexShader);
	GLint vertSourceLength = (GLint)vertexShaderSource.length();
	GLchar* vertSourceStr = (GLchar*)vertexShaderSource.c_str();

	gl::ShaderSource(vertexShader, 1, &vertSourceStr, &vertSourceLength);

	gl::CompileShader(vertexShader);

	if (!CompileSuccessful(vertexShader))
	{
		auto vertexShaderlog = GetShaderLog(vertexShader);
		Log::Error << "Vertex Shader compilation error\n" << vertexShaderlog << endl;
		return false;
	}

	check_gl_error();

	GLuint fragmentShader = gl::CreateShader((GLenum)ShaderType::FragmentShader);
	GLint fragSourceLength = (GLint)fragmentShaderSource.length();
	GLchar* fragmentSourceStr = (GLchar*)fragmentShaderSource.c_str();

	gl::ShaderSource(fragmentShader, 1, &fragmentSourceStr, &fragSourceLength);

	gl::CompileShader(fragmentShader);
	check_gl_error();

	if (!CompileSuccessful(fragmentShader))
	{
		auto fragmentShaderLog = GetShaderLog(fragmentShader);
		Log::Error << "Fragment Shader compilation error\n" << fragmentShaderLog << endl;
		return false;
	}
	
	check_gl_error();
	GLuint geometryShader = gl::CreateShader((GLenum)ShaderType::GeometryShader);
	GLint geomSourceLength = (GLint)geometryShaderSource.length();
	GLchar* geomSourceStr = (GLchar*)geometryShaderSource.c_str();

	gl::ShaderSource(geometryShader, 1, &geomSourceStr, &geomSourceLength);

	gl::CompileShader(geometryShader);
	check_gl_error();

	if (!CompileSuccessful(geometryShader))
	{
		auto geometryShaderLog = GetShaderLog(geometryShader);
		Log::Error << "Geometry Shader compilation error\n" << geometryShaderLog << endl;
		return false;
	}
	
	/// link the shaders into a program
	/// at minimum, a program MUST have a vertex shader and a fragment shader
	m_program = gl::CreateProgram();

	gl::AttachShader(m_program, vertexShader);
	gl::AttachShader(m_program, fragmentShader);
	gl::AttachShader(m_program, geometryShader);

	check_gl_error();

	gl::LinkProgram(m_program);
	string log = GetProgramLog(m_program);
	if (log != "No errors.\n")
	{
		Log::Error << log << endl;
		DEBUG_BREAK;
	}

	check_gl_error();

	Log::Info << "Successfully built program.\n";

	GLint* a = new GLint;
	gl::GetProgramiv( m_program, gl::LINK_STATUS,a);
	if (*a)
	{
		Log::Info << "Successfully linked program." << std::endl;
	}
	else
	{
		Log::Error << "Failed linked program.\nPress a Key to exit." << std::endl;
		getchar();
		exit(0);
	}

	return true;

}

void Material::OnDispose()
{
	if (m_program > 0)
	{
		gl::UseProgram(0);
		gl::DeleteProgram(m_program);

		m_program = 0;
	}

}

string Material::GetShaderLog(GLuint shader)
{
	string log = "";
	GLint logLength = 0;

	gl::GetShaderiv(shader, gl::INFO_LOG_LENGTH, &logLength);

	if (logLength > 0)
	{
		char *logBuffer = new char[logLength];

		/// this is useless when using std::string
		GLint returnedLength;

		gl::GetShaderInfoLog(shader, logLength, &returnedLength, logBuffer);
		log = string(logBuffer);

		delete logBuffer;
	}

	return log;
}

/// <summary>
/// Gets the program information log.
/// </summary>
/// <param name="program">The program.</param>
/// <returns></returns>
string Material::GetProgramLog(GLuint program)
{
	string log = "";
	GLint logLength = 0;

	/// this is useless.
	GLint returnedLength;

	gl::GetProgramiv(program, gl::INFO_LOG_LENGTH, &logLength);

	if (logLength > 0)
	{
		char* logBuffer = new char[logLength + 1];

		gl::GetProgramInfoLog(program, logLength, &returnedLength, logBuffer);

		log = string(logBuffer);

		delete logBuffer;
	}

	return log;
}

/// check if the shader compiled successfully
bool Material::CompileSuccessful(GLint program)
{
	GLint status;

	gl::GetShaderiv(program, gl::COMPILE_STATUS, &status);

	return status != (GLint)false;
}


bool Material::Build(const std::string& path)
{
    auto vertFilename = path + ".vert.glsl";
    auto fragFilename = path + ".frag.glsl";
	auto geomFilename = path + ".geom.glsl";
    
    if(!Files::Exists(vertFilename))
    {
        Log::Error << "Could not find vertex shader \"" << vertFilename << "\"\n";
        return false;
    }
    
    if(!Files::Exists(fragFilename))
    {
        Log::Error << "Could not find fragment shader \"" << fragFilename << "\"\n";
        return false;
    }

	if (!Files::Exists(geomFilename))
	{
		Log::Error << "Could not find geometry shader \"" << geomFilename << "\"\n";
		return false;
	}
    
    bool success = true;
    
    do
    {
    
        Log::Info << "Loading vertex shader \"" << vertFilename << "\"\n";
        auto vertexShaderSource = Files::Read(vertFilename);
        
        Log::Info << "Loading fragment shader \"" << fragFilename << "\"\n";
        auto fragmentShaderSource = Files::Read(fragFilename);

		Log::Info << "Loading geometry shader \"" << geomFilename << "\"\n";
		auto geometryShaderSource = Files::Read(geomFilename);
        
        success = Build(vertexShaderSource, fragmentShaderSource, geometryShaderSource);

        if(!success)
        {
            cout << "Press enter to retry." << endl;
            getchar();
        }
        
    } while(!success);
    
    return true;
    
}
