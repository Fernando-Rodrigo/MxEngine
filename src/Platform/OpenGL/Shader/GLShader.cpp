// Copyright(c) 2019 - 2020, #Momo
// All rights reserved.
// 
// Redistributionand use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met :
// 
// 1. Redistributions of source code must retain the above copyright notice, this
// list of conditionsand the following disclaimer.
// 
// 2. Redistributions in binary form must reproduce the above copyright notice,
// this list of conditionsand the following disclaimer in the documentation
// and /or other materials provided with the distribution.
// 
// 3. Neither the name of the copyright holder nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED.IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include "GLShader.h"
#include "Utilities/Logger/Logger.h"
#include "Core/Macro/Macro.h"
#include "Platform/OpenGL/GLUtilities/GLUtilities.h"
#include "Utilities/FileSystem/FileSystem.h"

namespace MxEngine
{
	GLShader::GLShader()
	{
		this->id = 0;
	}

	GLShader::GLShader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
	{
		Load(vertexShaderPath, fragmentShaderPath);
	}

	void GLShader::Bind() const
	{
		GLCALL(glUseProgram(id));
	}

	void GLShader::Unbind() const
	{
		GLCALL(glUseProgram(0));
	}

	GLShader::GLShader(GLShader&& shader) noexcept
	{
		#if defined(MXENGINE_DEBUG)
		this->vertexShaderPath = shader.vertexShaderPath;
		this->fragmentShaderPath = shader.fragmentShaderPath;
		#endif
		this->id = shader.id;
		shader.id = 0;
	}

	GLShader::~GLShader()
	{
		if (id != 0)
		{
			GLCALL(glDeleteProgram(id));
		}
	}

	void GLShader::Load(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
	{
		#if defined(MXENGINE_DEBUG)
		this->vertexShaderPath = vertexShaderPath;
		this->fragmentShaderPath = fragmentShaderPath;
		#endif
		std::string vs = File::ReadAllText(vertexShaderPath);
		std::string fs = File::ReadAllText(fragmentShaderPath);

		if (fs.empty())
		{
			Logger::Instance().Warning("MxEngine::Shader", "fragment shader is empty: " + fragmentShaderPath);
		}
		if (vs.empty())
		{
			Logger::Instance().Warning("MxEngine::Shader", "vertex shader is empty: " + vertexShaderPath);
		}

		Logger::Instance().Debug("Shader", "compiling vertex shader: " + vertexShaderPath);
		unsigned int vertexShader = CompileShader(ShaderType::VERTEX_SHADER, vs, vertexShaderPath);
		Logger::Instance().Debug("Shader", "compiling fragment shader: " + fragmentShaderPath);
		unsigned int fragmentShader = CompileShader(ShaderType::FRAGMENT_SHADER, fs, fragmentShaderPath);

		id = CreateProgram(vertexShader, fragmentShader);
		Logger::Instance().Debug("Shader", "shader program created with id = " + std::to_string(id));
	}

    void GLShader::LoadFromSource(const std::string& vertexSource, const std::string& fragmentSource)
    {
		Logger::Instance().Debug("Shader", "compiling vertex shader: [[raw source]]");
		unsigned int vertexShader = CompileShader(ShaderType::VERTEX_SHADER, vertexSource, "[[raw source]]");
		Logger::Instance().Debug("Shader", "compiling fragment shader: [[raw source]]");
		unsigned int fragmentShader = CompileShader(ShaderType::FRAGMENT_SHADER, fragmentSource, "[[raw source]]");

		id = CreateProgram(vertexShader, fragmentShader);
		Logger::Instance().Debug("Shader", "shader program created with id = " + std::to_string(id));
    }

	void GLShader::SetUniformFloat(const std::string& name, float f) const
	{
		int location = GetUniformLocation(name);
		if (location == -1) return;
		Bind();
		GLCALL(glUniform1f(location, f));
	}

	void GLShader::SetUniformVec3(const std::string& name, const Vector3& vec) const
	{
		int location = GetUniformLocation(name);
		if (location == -1) return;
		Bind();
		GLCALL(glUniform3f(location, vec.x, vec.y, vec.z));
	}

	void GLShader::SetUniformVec4(const std::string& name, const Vector4& vec) const
	{
		int location = GetUniformLocation(name);
		if (location == -1) return;
		Bind();
		GLCALL(glUniform4f(location, vec.x, vec.y, vec.z, vec.w));
	}

	void GLShader::SetUniformMat4(const std::string& name, const Matrix4x4& matrix) const
	{
		int location = GetUniformLocation(name);
		if (location == -1) return;
		Bind();
		GLCALL(glUniformMatrix4fv(location, 1, false, &matrix[0][0]));
	}

	void GLShader::SetUniformMat3(const std::string& name, const Matrix3x3& matrix) const
	{
		int location = GetUniformLocation(name);
		if (location == -1) return;
		Bind();
		GLCALL(glUniformMatrix3fv(location, 1, false, &matrix[0][0]));
	}

	void GLShader::SetUniformInt(const std::string& name, int i) const
	{
		int location = GetUniformLocation(name);
		if (location == -1) return;
		Bind();
		GLCALL(glUniform1i(location, i));
	}

	GLShader::ShaderId GLShader::CompileShader(ShaderType type, const std::string& source, const std::string& name) const
	{
		GLCALL(GLuint shaderId = glCreateShader((GLenum)type));
		const char* src = source.c_str();
		GLCALL(glShaderSource(shaderId, 1, &src, nullptr));
		GLCALL(glCompileShader(shaderId));

		GLint result;
		GLCALL(glGetShaderiv(shaderId, GL_COMPILE_STATUS, &result));
		if (result == GL_FALSE)
		{
			GLint length;
			GLCALL(glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &length));
			std::string msg;
			msg.resize(length);
			GLCALL(glGetShaderInfoLog(shaderId, length, &length, &msg[0]));
			msg.pop_back(); // extra \n character
			Logger::Instance().Error("Shader", (std::string)"failed to compile " +
				(type == ShaderType::VERTEX_SHADER ? "vertex" : "fragment") + " shader: " + name);
			Logger::Instance().Error("OpenGL", msg);
		}

		return shaderId;
	}

	IBindable::IdType GLShader::CreateProgram(GLShader::ShaderId vertexShader, GLShader::ShaderId fragmentShader) const
	{
		GLCALL(unsigned int program = glCreateProgram());

		GLCALL(glAttachShader(program, vertexShader));
		GLCALL(glAttachShader(program, fragmentShader));
		GLCALL(glLinkProgram(program));
		GLCALL(glValidateProgram(program));

		GLCALL(glDeleteShader(vertexShader));
		GLCALL(glDeleteShader(fragmentShader));

		return program;
	}

	int GLShader::GetUniformLocation(const std::string& uniformName) const
	{
		if (uniformCache.find(uniformName) != uniformCache.end())
			return uniformCache[uniformName];

		GLCALL(int location = glGetUniformLocation(id, uniformName.c_str()));
		if (location == -1)
			Logger::Instance().Warning("Shader", "uniform was not found: " + uniformName);
		uniformCache[uniformName] = location;
		return location;
	}
}