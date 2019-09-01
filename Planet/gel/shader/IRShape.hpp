#pragma once
#ifndef GEL_SHADER_IRSHAPE_HPP
#define GEL_SHADER_IRSHAPE_HPP
#include <glm/glm.hpp>
#include <vector>

namespace gel {
class IRShape {
       public:
        explicit IRShape();
        ~IRShape();

        void addVertex(const glm::vec4 vertex);
        glm::vec4 getVertexAt(int index) const;
        int getVertexCount() const;

        void addUV(const glm::vec2 uv);
        glm::vec2 getUVAt(int index) const;
        int getUVCount() const;

        void addNormal(const glm::vec4 normal);
        glm::vec4 getNormalAt(int index) const;
        int getNormalCount() const;

        void addIndex(int index);
        int getIndexAt(int index) const;
        int getIndexCount() const;

       private:
        std::vector<glm::vec4> vertexBuf;
        std::vector<glm::vec4> normalBuf;
        std::vector<glm::vec2> uvBuf;
        std::vector<int> indexBuf;
};
}  // namespace gel
#endif