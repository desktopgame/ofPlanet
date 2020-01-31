#pragma once
#ifndef WORLD_GRAPHICSBATCH_HPP
#define WORLD_GRAPHICSBATCH_HPP
#include <ofShader.h>

namespace planet {
class GraphicsBatch {
public:
	GraphicsBatch(ofShader& shader);
	virtual ~GraphicsBatch(){}

	virtual void putFront(float x, float y, float z) = 0;
	virtual void putBack(float x, float y, float z) = 0;
	virtual void putLeft(float x, float y, float z) = 0;
	virtual void putRight(float x, float y, float z) = 0;
	virtual void putTop(float x, float y, float z) = 0;
	virtual void putBottom(float x, float y, float z) = 0;
	virtual void clear() = 0;
	virtual void update() = 0;
	virtual void render(GLuint texture) = 0;

protected:
	ofShader& shader;
};
}
#endif