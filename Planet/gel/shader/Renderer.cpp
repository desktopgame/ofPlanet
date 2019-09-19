#include "Renderer.hpp"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "ShaderRegistry.hpp"
namespace gel {

DEF_ENUM_OPERATOR_CPP(RendererParameterType, int)

// RendererParameter
RendererParameter::RendererParameter(RendererParameterType type,
                                     RendererParameterValue value)
    : type(type), value(value) {}

RendererParameterType RendererParameter::getType() const { return type; }

void RendererParameter::setValue(const RendererParameterValue value) {
        this->value = value;
}

RendererParameterValue RendererParameter::getValue() const { return value; }
// Renderer
Renderer::Renderer(const std::shared_ptr<Shader>& shader)
    : shader(shader), 
	  params(),
	  vao() {}
void Renderer::init() { vao.init(); }
void Renderer::destroy() { vao.destroy(); }
void Renderer::put(const std::string& name, const RendererParameter parameter) {
        params.insert_or_assign(name, parameter);
}
void Renderer::remove(const std::string& name) { params.erase(name); }
RendererParameter Renderer::get(const std::string& name) const {
        return params.at(name);
}
void Renderer::apply() {
        shader->use();
		vao.bind();
        auto iter = params.begin();
        while (iter != params.end()) {
                auto pair = *iter;
                auto name = pair.first;
				applyImpl(name);
                ++iter;
        }
		vao.unbind();
        shader->unuse();
}

void Renderer::apply(const std::string & name)
{

	shader->use();
	vao.bind();
	applyImpl(name);
	vao.unbind();
	shader->unuse();
}

void Renderer::drawArrays(GLenum mode, GLint first, GLsizei count) {
		shader->use();
        vao.bind();
        glDrawArrays(mode, first, count);
        vao.unbind();
		shader->unuse();
}

void Renderer::drawElements(Buffer<GLushort>& index, GLenum mode, GLsizei count, GLenum type, const GLvoid * indices)
{
	shader->use();
	vao.bind();
	index.bind();
	glDrawElements(mode, count, type, indices);
	index.unbind();
	vao.unbind();
	shader->unuse();
}

void Renderer::drawElementsInstanced(Buffer<GLushort>& index, GLenum mode, GLsizei count, GLenum type, const void * indices, GLsizei instancecount)
{
	shader->use();
	vao.bind();
	index.bind();
	glDrawElementsInstanced(mode, count, type, indices, instancecount);
	index.unbind();
	vao.unbind();
	shader->unuse();
}

void Renderer::setShader(const std::string& shaderName) {
        setShader(ShaderRegistry::getInstance().get(shaderName));
}
void Renderer::setShader(const std::shared_ptr<Shader>& shader) {
        this->shader = shader;
}
std::shared_ptr<Shader> Renderer::getShader() const { return shader; }
VertexArray& Renderer::getVertexArray()
{
	return vao;
}

void Renderer::applyImpl(const std::string & name)
{
	auto param = params.at(name);
	auto type = param.getType();
	if (!!(type & RendererParameterType::Attribute)) {
		// Int
		if (!!(type & RendererParameterType::IntUnit)) {
			Buffer<int> buf =
				std::get<Buffer<int> >(param.getValue());
			buf.bind();
			shader->setVertexAttribPointer(
				name, 1, GL_INT, GL_FALSE, 0, NULL);
			shader->enableVertexAttribArray(name);
			//buf.unbind();
		}
		else if (!!(type & RendererParameterType::IntVec2)) {
			Buffer<int> buf =
				std::get<Buffer<int> >(param.getValue());
			buf.bind();
			shader->setVertexAttribPointer(
				name, 2, GL_INT, GL_FALSE, 0, NULL);
			shader->enableVertexAttribArray(name);
			//buf.unbind();
		}
		else if (!!(type & RendererParameterType::IntVec3)) {
			Buffer<int> buf =
				std::get<Buffer<int> >(param.getValue());
			buf.bind();
			shader->setVertexAttribPointer(
				name, 3, GL_INT, GL_FALSE, 0, NULL);
			shader->enableVertexAttribArray(name);
			//buf.unbind();
		}
		else if (!!(type & RendererParameterType::IntVec4)) {
			Buffer<int> buf =
				std::get<Buffer<int> >(param.getValue());
			buf.bind();
			shader->setVertexAttribPointer(
				name, 4, GL_INT, GL_FALSE, 0, NULL);
			shader->enableVertexAttribArray(name);
			//buf.unbind();
			// Float
		}
		else if (!!(type &
			RendererParameterType::FloatUnit)) {
			Buffer<float> buf =
				std::get<Buffer<float> >(param.getValue());
			buf.bind();
			shader->setVertexAttribPointer(
				name, 1, GL_FLOAT, GL_FALSE, 0, NULL);
			shader->enableVertexAttribArray(name);
			//buf.unbind();
		}
		else if (!!(type &
			RendererParameterType::FloatVec2)) {
			Buffer<float> buf =
				std::get<Buffer<float> >(param.getValue());
			buf.bind();
			shader->setVertexAttribPointer(
				name, 2, GL_FLOAT, GL_FALSE, 0, NULL);
			shader->enableVertexAttribArray(name);
			//buf.unbind();
		}
		else if (!!(type &
			RendererParameterType::FloatVec3)) {
			Buffer<float> buf =
				std::get<Buffer<float> >(param.getValue());
			buf.bind();
			shader->setVertexAttribPointer(
				name, 3, GL_FLOAT, GL_FALSE, 0, NULL);
			shader->enableVertexAttribArray(name);
			//buf.unbind();
		}
		else if (!!(type &
			RendererParameterType::FloatVec4)) {
			Buffer<float> buf =
				std::get<Buffer<float> >(param.getValue());
			buf.bind();
			shader->setVertexAttribPointer(
				name, 4, GL_FLOAT, GL_FALSE, 0, NULL);
			shader->enableVertexAttribArray(name);
			//buf.unbind();
		}
		else {
			throw std::logic_error(
				"Not supported attribute: " + name);
		}
	}
	else if (!!(type & RendererParameterType::Uniform)) {
		// Int
		if (!!(type & RendererParameterType::IntUnit)) {
			shader->setUniform1i(
				name, std::get<int>(param.getValue()));
		}
		else if (!!(type & RendererParameterType::IntVec2)) {
			glm::ivec2 iv2 =
				std::get<glm::ivec2>(param.getValue());
			shader->setUniform2i(name, iv2.x, iv2.y);
		}
		else if (!!(type & RendererParameterType::IntVec3)) {
			glm::ivec3 iv3 =
				std::get<glm::ivec3>(param.getValue());
			shader->setUniform3i(name, iv3.x, iv3.y, iv3.z);
		}
		else if (!!(type & RendererParameterType::IntVec4)) {
			glm::ivec4 iv4 =
				std::get<glm::ivec4>(param.getValue());
			shader->setUniform4i(name, iv4.x, iv4.y, iv4.z,
				iv4.w);
			// Float
		}
		else if (!!(type &
			RendererParameterType::FloatUnit)) {
			shader->setUniform1f(
				name, std::get<float>(param.getValue()));
		}
		else if (!!(type &
			RendererParameterType::FloatVec2)) {
			glm::vec2 iv2 =
				std::get<glm::vec2>(param.getValue());
			shader->setUniform2f(name, iv2.x, iv2.y);
		}
		else if (!!(type &
			RendererParameterType::FloatVec3)) {
			glm::vec3 iv3 =
				std::get<glm::vec3>(param.getValue());
			shader->setUniform3f(name, iv3.x, iv3.y, iv3.z);
		}
		else if (!!(type &
			RendererParameterType::FloatVec4)) {
			glm::vec4 iv4 =
				std::get<glm::vec4>(param.getValue());
			shader->setUniform4f(name, iv4.x, iv4.y, iv4.z,
				iv4.w);
			// Matrix
		}
		else if (!!(type &
			RendererParameterType::Matrix4x4)) {
			glm::mat4 mat =
				std::get<glm::mat4>(param.getValue());
			shader->setUniformMatrix4fv(
				name, 1, GL_FALSE, glm::value_ptr(mat));
		}
		else {
			throw std::logic_error(
				"Not supported uniform: " + name);
		}
	}
}
}  // namespace gel