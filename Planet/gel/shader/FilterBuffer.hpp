#pragma once
#ifndef GEL_SHADER_FILTEBUFFER_HPP
#define GEL_SHADER_FILTEBUFFER_HPP
#include "PixelBuffer.hpp"
#include <functional>
#include "Buffer.hpp"
#include "FrameBuffer.hpp"
#include "NameRule.hpp"
#include "RenderBuffer.hpp"
#include "Shader.hpp"
#include "VertexArray.hpp"

namespace gel {
using FilterFunc = std::function<void(GLubyte*, int, int)>;
class FilterBuffer {
public:
	explicit FilterBuffer(const std::shared_ptr<Shader>& shader,
		const NameRule nameRule);
	void init(int width, int height, int ch);
	void destroy();
	void render(FilterFunc func);
private:
	//FIXME: copy from ScreenBuffer
	void initRect();
	GLuint texture;
	PixelBuffer pixelBuffer;
	std::shared_ptr<Shader> shader;
	NameRule nameRule;
	VertexArray vao;
	Buffer<GLfloat> vertex;
	Buffer<GLfloat> uv;
};
}
#endif