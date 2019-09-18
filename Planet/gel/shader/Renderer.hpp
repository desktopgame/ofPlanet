#pragma once
#ifndef GEL_SHADER_RENDERER_HPP
#define GEL_SHADER_RENDERER_HPP
#include <string>
#include <memory>
#include <variant>
#include <glm/glm.hpp>
#include <unordered_map>
#include "../util/enum.hpp"
#include "Buffer.hpp"
#include "VertexArray.hpp"

namespace gel {
class Shader;
enum class RendererParameterType : int {
	Attribute = 1 << 0,
	Uniform = 1 << 1,

	IntUnit = 1 << 2,
	IntVec2 = 1 << 3,
	IntVec3 = 1 << 4,
	IntVec4 = 1 << 5,

	FloatUnit = 1 << 6,
	FloatVec2 = 1 << 7,
	FloatVec3 = 1 << 8,
	FloatVec4 = 1 << 9,

	Matrix4x4 = 1 << 10,
};
DEF_ENUM_OPERATOR_HPP(RendererParameterType, int)

using RendererParameterValue = std::variant<
	int, glm::ivec2, glm::ivec3, glm::ivec4,
	float, glm::vec2,glm::vec3, glm::vec4, 
	glm::mat4,
	Buffer<int>,
	Buffer<float>
>;
class RendererParameter {
public:
	explicit RendererParameter(RendererParameterType type, RendererParameterValue value);
	RendererParameterType getType() const;
	
	void setValue(const RendererParameterValue value);
	RendererParameterValue getValue() const;
private:
	RendererParameterType type;
	RendererParameterValue value;
};
class Renderer {
public:
	explicit Renderer(const std::shared_ptr<Shader>& shader);
	void init();
	void destroy();

	void put(const std::string& name, const RendererParameter parameter);
	void remove(const std::string& name);
	RendererParameter get(const std::string& name) const;
	void apply();

	void drawArrays(GLenum mode, GLint first, GLsizei count);

	void setShader(const std::string& shaderName);
	void setShader(const std::shared_ptr<Shader>& shader);
	std::shared_ptr<Shader> getShader() const;
private:
	std::shared_ptr<Shader> shader;
	std::unordered_map<std::string, RendererParameter> params;
	VertexArray vao;
};
}
#endif