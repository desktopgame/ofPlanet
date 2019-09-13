#include "FilterBuffer.hpp"
namespace gel {
	FilterBuffer::FilterBuffer(const std::shared_ptr<Shader>& shader,
		const NameRule nameRule)
		: pixelBuffer(), 
		  vao(), 
		  vertex(GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW),
		  uv(GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW),
		  shader(shader),
		  nameRule(nameRule)
	{
	}
	void FilterBuffer::init(int width, int height, int ch)
	{
		initRect();
		pixelBuffer.init(width, height, ch);
		// generate texture
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
			GL_UNSIGNED_BYTE, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	void FilterBuffer::destroy()
	{
		pixelBuffer.destroy();
	}
	void FilterBuffer::render(FilterFunc func)
	{
		pixelBuffer.bind();
		func(pixelBuffer.read(), pixelBuffer.getWidth(), pixelBuffer.getHeight());
		pixelBuffer.transport(texture);
		pixelBuffer.unbind();
		// draw to screen
		glDisable(GL_DEPTH_TEST);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shader->use();
		glBindTexture(GL_TEXTURE_2D, texture);
		vao.bind();
		glDrawArrays(GL_TRIANGLES, 0, 6);
		vao.unbind();
		shader->unuse();
	}

	// private
	void FilterBuffer::initRect() {
		vao.init();
		vertex.init();
		uv.init();

		auto& vSeq = vertex.modify();
		vSeq.emitVec4(-1, 1, 0, 1);
		vSeq.emitVec4(-1, -1, 0, 1);
		vSeq.emitVec4(1, -1, 0, 1);
		vSeq.emitVec4(-1, 1, 0, 1);
		vSeq.emitVec4(1, -1, 0, 1);
		vSeq.emitVec4(1, 1, 0, 1);

		auto& uvSeq = uv.modify();
		uvSeq.emitVec2(0, 1);
		uvSeq.emitVec2(0, 0);
		uvSeq.emitVec2(1, 0);
		uvSeq.emitVec2(0, 1);
		uvSeq.emitVec2(1, 0);
		uvSeq.emitVec2(1, 1);

		vertex.bind();
		vertex.update();
		vertex.unbind();

		uv.bind();
		uv.update();
		uv.unbind();
		// setup vao
		shader->use();
		shader->setUniform1i("uTexture", 0);
		vao.bind();

		vertex.bind();
		shader->setVertexAttribPointer(nameRule.attribVertex, 4, GL_FLOAT,
			GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(
			shader->getAttribLocation(nameRule.attribVertex));

		uv.bind();
		shader->setVertexAttribPointer(nameRule.attribUV, 2, GL_FLOAT, GL_FALSE,
			0, NULL);
		glEnableVertexAttribArray(shader->getAttribLocation(nameRule.attribUV));

		vao.unbind();
		shader->unuse();
		vertex.unbind();
		uv.unbind();
	}
}
