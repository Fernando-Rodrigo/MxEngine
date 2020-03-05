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

#include "GLRenderer.h"
#include "Utilities/Logger/Logger.h"
#include "Platform/OpenGL/GLUtilities/GLUtilities.h"
#include "Core/Interfaces/GraphicAPI/GraphicFactory.h"
#include "Platform/OpenGL/GraphicFactory/GLGraphicFactory.h"
#include "Utilities/Profiler/Profiler.h"

namespace MxEngine
{
	GLRenderer::GLRenderer()
	{
		this->clearMask |= GL_COLOR_BUFFER_BIT;
	}

	void GLRenderer::DrawTriangles(const VertexArray& vao, const IndexBuffer& ibo, const Shader& shader) const
	{
		vao.Bind();
		ibo.Bind();
		shader.Bind();
		GLCALL(glDrawElements(GL_TRIANGLES, (GLsizei)ibo.GetCount(), (GLenum)ibo.GetIndexTypeId(), nullptr));
	}

	void GLRenderer::DrawTriangles(const VertexArray& vao, size_t vertexCount, const Shader& shader) const
	{
		vao.Bind();
		shader.Bind();
		GLCALL(glDrawArrays(GL_TRIANGLES, 0, (GLsizei)vertexCount));
	}

	void GLRenderer::DrawLines(const VertexArray& vao, const IndexBuffer& ibo, const Shader& shader) const
	{
		vao.Bind();
		ibo.Bind();
		shader.Bind();
		GLCALL(glDrawElements(GL_LINES, (GLsizei)ibo.GetCount(), (GLenum)ibo.GetIndexTypeId(), nullptr));
	}

	void GLRenderer::DrawLinesInstanced(const VertexArray& vao, const IndexBuffer& ibo, const Shader& shader, size_t count) const
	{
		vao.Bind();
		ibo.Bind();
		shader.Bind();
		GLCALL(glDrawElementsInstanced(GL_LINES, (GLsizei)ibo.GetCount(), (GLenum)ibo.GetIndexTypeId(), nullptr, (GLsizei)count));
	}

	void GLRenderer::DrawLinesInstanced(const VertexArray& vao, size_t vertexCount, const Shader& shader, size_t count) const
	{
		vao.Bind();
		shader.Bind();
		GLCALL(glDrawArraysInstanced(GL_LINES, 0, (GLsizei)vertexCount, (GLsizei)count));
	}

	void GLRenderer::DrawTrianglesInstanced(const VertexArray& vao, const IndexBuffer& ibo, const Shader& shader, size_t count) const
	{
		vao.Bind();
		ibo.Bind();
		shader.Bind();
		GLCALL(glDrawElementsInstanced(GL_TRIANGLES, (GLsizei)ibo.GetCount(), (GLenum)ibo.GetIndexTypeId(), nullptr, (GLsizei)count));
	}

	void GLRenderer::DrawTrianglesInstanced(const VertexArray& vao, size_t vertexCount, const Shader& shader, size_t count) const
	{
		vao.Bind();
		shader.Bind();
		GLCALL(glDrawArraysInstanced(GL_TRIANGLES, 0, (GLsizei)vertexCount, (GLsizei)count));
	}

	void GLRenderer::DrawLines(const VertexArray& vao, size_t vertexCount, const Shader& shader) const
	{
		vao.Bind();
		shader.Bind();
		GLCALL(glDrawArrays(GL_LINES, 0, (GLsizei)vertexCount));
	}

	void GLRenderer::Clear() const
	{
		GLCALL(glClear(clearMask));
	}

	void GLRenderer::Flush() const
	{
		MAKE_SCOPE_PROFILER("Renderer::Flush");
		Graphics::Instance()->GetGraphicModule().OnRenderDraw();

		GLCALL(glFlush());
	}

	void GLRenderer::Finish() const
	{
		MAKE_SCOPE_PROFILER("Renderer::Finish");
		Graphics::Instance()->GetGraphicModule().OnRenderDraw();

		GLCALL(glFinish());
	}

	GLRenderer& GLRenderer::UseSampling(bool value)
	{
		if (value)
		{
			GLCALL(glEnable(GL_MULTISAMPLE));
		}
		else
		{
			GLCALL(glDisable(GL_MULTISAMPLE));
		}
		return *this;
	}

	GLRenderer& GLRenderer::UseDepthBuffer(bool value)
	{
		depthBufferEnabled = value;
		if (value)
		{
			GLCALL(glEnable(GL_DEPTH_TEST));
			GLCALL(glDepthFunc(GL_LESS));
			clearMask |= GL_DEPTH_BUFFER_BIT;
		}
		else
		{
			GLCALL(glDisable(GL_DEPTH_TEST));
			clearMask &= ~GL_DEPTH_BUFFER_BIT;
		}
		return *this;
	}

	GLRenderer& GLRenderer::UseCulling(bool value, bool counterClockWise, bool cullBack)
	{
		// culling 
		if (value)
		{
			GLCALL(glEnable(GL_CULL_FACE));
		}
		else
		{
			GLCALL(glDisable(GL_CULL_FACE));
		}

		// point order
		if (counterClockWise)
		{
			GLCALL(glFrontFace(GL_CCW));
		}
		else
		{
			GLCALL(glFrontFace(GL_CW));
		}

		// back / front culling
		if (cullBack)
		{
			GLCALL(glCullFace(GL_BACK));
		}
		else
		{
			GLCALL(glCullFace(GL_FRONT));
		}

		return *this;
	}

	GLRenderer& GLRenderer::UseClearColor(float r, float g, float b, float a)
	{
		GLCALL(glClearColor(r, g, b, a));
		return *this;
	}
	GLRenderer& GLRenderer::UseTextureMinFilter(MinFilter filter)
	{
		GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLint)filter));
		return *this;
	}

	GLRenderer& GLRenderer::UseTextureMagFilter(MagFilter filter)
	{
		GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLint)filter));
		return *this;
	}

	GLRenderer& GLRenderer::UseBlending(BlendFactor src, BlendFactor dist)
	{
		if (src == BlendFactor::NONE || dist == BlendFactor::NONE)
		{
			GLCALL(glDisable(GL_BLEND));
		}
		else
		{
			GLCALL(glEnable(GL_BLEND));
			GLCALL(glBlendFunc((GLenum)GL_SRC_ALPHA, (GLenum)GL_ONE_MINUS_SRC_ALPHA));
		}
		return *this;
	}

	GLRenderer& GLRenderer::UseTextureWrap(WrapType textureX, WrapType textureY)
	{
		GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (GLenum)textureX));
		GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (GLenum)textureY));
		return *this;
	}

	GLRenderer& GLRenderer::UseAnisotropicFiltering(float factor)
	{
		if (!glfwExtensionSupported("GL_EXT_texture_filter_anisotropic"))
		{
			Logger::Instance().Warning("OpenGL", "anisotropic filtering is not supported");
		}
		else
		{
			GLCALL(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, factor));
			Logger::Instance().Debug("OpenGL", "set anisotropic filtering factor to " + std::to_string((int)factor) + "x");
		}
		return *this;
	}

	float GLRenderer::GetLargestAnisotropicFactor() const
	{
		if (!glfwExtensionSupported("GL_EXT_texture_filter_anisotropic"))
		{
			Logger::Instance().Warning("OpenGL", "anisotropic filtering is not supported");
			return 0.0f;
		}
		float factor;
		GLCALL(glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &factor));
		return factor;
	}
}