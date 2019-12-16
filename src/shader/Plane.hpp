#pragma once
#ifndef SHADER_PLANE_HPP
#define SHADER_PLANE_HPP
#include <ofVbo.h>

#include "Buffer.hpp"
#include "CameraObserver.hpp"
#include "NameSet.hpp"
#include "VertexArray.hpp"
namespace planet {

enum class PlaneType : unsigned int {
        Front = 0,
        Back = 1,
        Left = 2,
        Right = 3,
        Top = 4,
        Bottom = 5,
        Count = 6,
};

class Shader;
class Plane : public CameraObserver {
       public:
        explicit Plane(PlaneType type, const NameSet& nameSet,
                       const glm::vec3 size);
        void draw();
        void drawInstanced(int count);

        void onRehash(std::shared_ptr<const Camera> camera) override;

        ofVbo& getVAO();
        const ofVbo& getVAO() const;
        NameSet getNameSet() const;

       private:
        void setupOfVboData(std::vector<float> vertex,
                            std::vector<float> normal, std::vector<float> uv);
        void setupOfVbo(PlaneType type, const NameSet& nameSet,
                        const glm::vec3 size);
        Plane(const Plane& obj) = delete;
        Plane& operator=(const Plane&) = delete;

        static std::vector<float> createFrontVertex(glm::vec3 size);
        static std::vector<float> createFrontNormal(glm::vec3 size);
        static std::vector<float> createFrontUV();

        static std::vector<float> createBackVertex(glm::vec3 size);
        static std::vector<float> createBackNormal(glm::vec3 size);
        static std::vector<float> createBackUV();

        static std::vector<float> createLeftVertex(glm::vec3 size);
        static std::vector<float> createLeftNormal(glm::vec3 size);
        static std::vector<float> createLeftUV();

        static std::vector<float> createRightVertex(glm::vec3 size);
        static std::vector<float> createRightNormal(glm::vec3 size);
        static std::vector<float> createRightUV();

        static std::vector<float> createTopVertex(glm::vec3 size);
        static std::vector<float> createTopNormal(glm::vec3 size);
        static std::vector<float> createTopUV();

        static std::vector<float> createBottomVertex(glm::vec3 size);
        static std::vector<float> createBottomNormal(glm::vec3 size);
        static std::vector<float> createBottomUV();

        PlaneType type;
        ofVbo ofVAO;
        NameSet nameSet;
};
}  // namespace planet
#endif  // !SHADER_PLANE_HPP
