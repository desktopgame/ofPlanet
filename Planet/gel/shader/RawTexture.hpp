#pragma once
#ifndef GEL_SHADER_RAWTEXTURE_HPP
#define GEL_SHADER_RAWTEXTURE_HPP
#include "../gli.hpp"
#include "../util/Flag.hpp"
#include "Buffer.hpp"
#include "FrameBuffer.hpp"
#include "NameRule.hpp"
#include "RenderBuffer.hpp"
#include "Shader.hpp"
#include "VertexArray.hpp"
#include "Color4.hpp"
namespace gel {
class RawTexture {
public:
	explicit RawTexture(const std::shared_ptr<Shader>& shader,
		const NameRule nameRule);
	void init(int width, int height);
	void destroy();
	void render();
	GLuint getTextureID() const;
private:
	void initRect();
	GLuint textureID;
	int width;
	int height;

	std::shared_ptr<Shader> shader;
	Color4 clearColor;
	NameRule nameRule;
	VertexArray vao;
	Buffer<GLfloat> vertex;
	Buffer<GLfloat> uv;
};
}
#endif