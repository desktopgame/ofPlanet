#include "Layer.hpp"
#include <iostream>
namespace gel {
	Layer::Layer(const std::shared_ptr<Shader>& shader, const NameRule nameRule)
		: shader(shader),
		  screenBuffer(shader, nameRule),
		  initFlag(false)
	{
	}
	void Layer::init(int width, int height)
	{
		initFlag.check(false, "already initialized");
		initFlag.enable();
		this->width = width;
		this->height = height;
		screenBuffer.init(width, height);
	}
	void Layer::destroy()
	{
		initFlag.check(true, "should be call init");
		auto iter = children.begin();
		while (iter != children.end()) {
			auto val = (*iter).second;
			val->destroy();
			++iter;
		}
		children.clear();
	}

	std::shared_ptr<Layer> Layer::remove(int depth)
	{
		initFlag.check(true, "should be call init");
		std::shared_ptr<Layer> ret = nullptr;
		if (children.count(depth)) {
			ret = children[depth];
			children.erase(depth);
		}
		return ret;
	}
	std::shared_ptr<Layer> Layer::get(int depth) const
	{
		initFlag.check(true, "should be call init");
		return children.at(depth);
	}
	void Layer::batch()
	{
		initFlag.check(true, "should be call init");
		auto iter = children.begin();
		while (iter != children.end()) {
			auto val = (*iter).second;
			val->batch();
			++iter;
		}
		screenBuffer.bind();
		batchLayer();
		screenBuffer.unbind();
	}
	void Layer::render()
	{
		initFlag.check(true, "should be call init");
		auto iter = children.begin();
		while (iter != children.end()) {
			auto val = (*iter).second;
			val->render();
			++iter;
		}
		screenBuffer.render();
	}
	std::shared_ptr<Shader> Layer::getShader() const
	{
		return shader;
	}
	void Layer::batchLayer()
	{
	}
	DelegateLayer::DelegateLayer(const std::shared_ptr<Shader>& shader, const NameRule nameRule, Action action)
		: Layer(shader,nameRule), action(action)
	{
	}
	void DelegateLayer::batchLayer()
	{
		action();
	}
}
