#include "GraphicsBatch.hpp"
namespace planet {
GraphicsBatch::GraphicsBatch(const World& world, ofShader & shader)
 : world(world), shader(shader) {
}
}