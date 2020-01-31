#pragma once
#ifndef WORLD_GRAPHICSRENDERER_HPP
#define WORLD_GRAPHICSRENDERER_HPP
#include <ofShader.h>
#include <unordered_map>
#include <memory>


namespace planet {
class World;
class GraphicsBatch;
class GraphicsRenderer {
public:
	GraphicsRenderer(const World& world, ofShader& shader);

	void putFront(GLuint texture, float x, float y, float z);
	void putBack(GLuint texture, float x, float y, float z);
	void putLeft(GLuint texture, float x, float y, float z);
	void putRight(GLuint texture, float x, float y, float z);
	void putTop(GLuint texture, float x, float y, float z);
	void putBottom(GLuint texture, float x, float y, float z);
	void clear();
	void update();
	void render();
protected:
	virtual std::shared_ptr<GraphicsBatch> createBatch(const World& world, ofShader& shader) = 0;
private:
	const World& world;
	ofShader& shader;
	std::unordered_map<GLuint, std::shared_ptr<GraphicsBatch> > map;
	std::shared_ptr<GraphicsBatch> ref(GLuint texture);
};
}
#endif