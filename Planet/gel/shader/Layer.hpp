#pragma once
#ifndef GEL_SHAFER_LAYER_HPP
#define GEL_SHAFER_LAYER_HPP
#include <memory>
#include <map>
#include <vector>
#include <functional>
#include "NameRule.hpp"
#include "PixelBuffer.hpp"
#include "ScreenBuffer.hpp"
#include "RawTexture.hpp"
#include "../util/Flag.hpp"

namespace gel {
class Shader;
class Layer : public std::enable_shared_from_this<Layer> {
public:
	explicit Layer(const std::shared_ptr<Shader>& shader,
		const NameRule nameRule);
	virtual ~Layer() = default;
	void init(int width, int height);
	void destroy();

	template<typename T, typename ...Args>
	std::shared_ptr<T> put(int depth, Args... args);

	std::shared_ptr<Layer> remove(int depth);
	std::shared_ptr<Layer> get(int depth) const;

	void batch();
	void render();

	std::shared_ptr<Shader> getShader() const;
protected:
	virtual void batchLayer();
private:
	Flag initFlag;
	int width;
	int height;
	std::shared_ptr<Shader> shader;
	NameRule nameRule;
	ScreenBuffer screenBuffer;
	std::map<int, std::shared_ptr<Layer> > children;
};

class DelegateLayer : public Layer {
public:
	using Action = std::function<void()>;
	explicit DelegateLayer(const std::shared_ptr<Shader>& shader,
		const NameRule nameRule, Action action);
protected:
	void batchLayer() override;
private:
	Action action;
};
template<typename T, typename ...Args>
inline std::shared_ptr<T> Layer::put(int depth, Args... args) {
	initFlag.check(true, "should be call init");
	auto layer = std::make_shared<T>(args...);
	layer->init(width, height);
	children[depth] = layer;
	return layer;
}
}
#endif