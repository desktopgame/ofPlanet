#include "FbxModel.hpp"
#include <algorithm>
#include <cassert>
#include <iostream>
#include <stdexcept>
#include "../shader/ShaderRegistry.hpp"
namespace gel {
FbxModel::FbxModel(FbxManager* fbxManager, const std::string& textureShaderName,
                   const std::string& colorShaderName, const NameRule& nameRule)
    : fbxManager(fbxManager),
      fbxScene(nullptr),
      aabb(),
      textureShaderName(textureShaderName),
      colorShaderName(colorShaderName),
      nameRule(nameRule) {}

void FbxModel::load(const std::string& path) {
        this->fbxScene = FbxScene::Create(this->fbxManager, "");
        this->fbxImporter = FbxImporter::Create(this->fbxManager, "");
        if (!fbxImporter->Initialize(path.c_str())) {
                throw std::logic_error("fatal error: FbxImporter#Initialize");
        }
        if (!fbxImporter->Import(fbxScene)) {
                throw std::logic_error("fatal error: FbxImporter#Initialize");
        }
        FbxGeometryConverter geometryConverter(fbxManager);
        geometryConverter.Triangulate(fbxScene, true);
        auto rootNode = fbxScene->GetRootNode();
        this->meshInfo = procRecNode(rootNode);
        std::vector<FbxMesh*> meshes;
        std::vector<glm::vec3> vvertex;
        this->aabb = AABB(vvertex);
}

void FbxModel::unload(const std::string& path) {
        fbxImporter->Destroy();
        fbxScene->Destroy();
}

void FbxModel::draw() { drawMeshInfo(meshInfo); }

AABB FbxModel::getAABB() const { return aabb; }

// private
void FbxModel::drawMeshInfo(std::shared_ptr<FbxMeshInfo> meshInfo) {
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_NORMAL_ARRAY);
        size_t size = meshInfo->getMaterialCount();
        for (int i = 0; i < (signed)size; i++) {
                auto& mat = meshInfo->getMaterialAt(i);
                auto& shader = ShaderRegistry::getInstance().get(mat.shader);
                applyShader(mat);
                shader.use();
                if (mat.textureNo > 0) {
                        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
                        glEnable(GL_TEXTURE_2D);
                        glBindTexture(
                            GL_TEXTURE_2D,
                            meshInfo->getTextureAt(mat.textureNo - 1)->getID());
                } else {
                        glDisable(GL_TEXTURE_2D);
                        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
                }
                if (mat.triangles.size() > 1) {
                        mat.triVAO.bind();
                        glDrawArrays(GL_TRIANGLES, 0, mat.triangles.size());
                        mat.triVAO.unbind();
                }
                if (mat.quads.size() > 1) {
                        mat.quadVAO.bind();
                        glDrawArrays(GL_QUADS, 0, mat.quads.size());
                        mat.quadVAO.unbind();
                }
                shader.unuse();
        }
        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_NORMAL_ARRAY);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        glDisable(GL_TEXTURE_2D);
        for (int i = 0; i < meshInfo->getChildCount(); i++) {
                drawMeshInfo(meshInfo->getChildAt(i));
        }
}

std::shared_ptr<FbxMeshInfo> FbxModel::procRecNode(FbxNode* node) {
        return procRecNode(node, 0);
}

std::shared_ptr<FbxMeshInfo> FbxModel::procRecNode(FbxNode* node, int depth) {
        std::string name = node->GetName();
        for (int i = 0; i < depth; i++) {
                std::cout << "    ";
        }
        std::cout << name << std::endl;
        int count = node->GetChildCount();
        auto ret = std::make_shared<FbxMeshInfo>();
        procNode(node, ret);
        for (int i = 0; i < count; i++) {
                FbxNode* child = node->GetChild(i);
                ret->addChild(procRecNode(child, depth + 1));
        }
        return ret;
}

void FbxModel::procNode(FbxNode* node, std::shared_ptr<FbxMeshInfo> target) {
        auto attr = node->GetNodeAttribute();
        if (attr == NULL) {
                return;
        }
        auto type = attr->GetAttributeType();
        if (type != FbxNodeAttribute::EType::eMesh) {
                return;
        }
        procVertex(node, target);
        procIndex(node, target);
        procNormal(node, target);
        procUV(node, target);
        procMaterial(node, target);
        procSide(node, target);
}

void FbxModel::procVertex(FbxNode* node, std::shared_ptr<FbxMeshInfo> target) {
        FbxMesh* fbxMesh = node->GetMesh();
        FbxVector4* coord = fbxMesh->GetControlPoints();
        for (int i = 0; i < fbxMesh->GetControlPointsCount(); i++) {
                float x = (float)coord[i][0];
                float y = (float)coord[i][1];
                float z = (float)coord[i][2];
                target->addVertex(glm::vec4(x, y, z, 1));
        }
}

void FbxModel::procIndex(FbxNode* node, std::shared_ptr<FbxMeshInfo> target) {
        FbxMesh* fbxMesh = node->GetMesh();
        int* vindex = fbxMesh->GetPolygonVertices();
        for (int i = 0; i < fbxMesh->GetPolygonCount() * 3; i++) {
                target->addIndex(vindex[i]);
        }
}

void FbxModel::procNormal(FbxNode* node, std::shared_ptr<FbxMeshInfo> target) {
        FbxMesh* fbxMesh = node->GetMesh();
        FbxLayerElementNormal* elementNormal =
            fbxMesh->GetLayer(0)->GetNormals();
        if (elementNormal->GetMappingMode() ==
            FbxLayerElement::eByPolygonVertex) {
                if (elementNormal->GetReferenceMode() ==
                    FbxLayerElement::eDirect) {
                        for (int i = 0;
                             i < elementNormal->GetDirectArray().GetCount();
                             i++) {
                                float x = (float)elementNormal->GetDirectArray()
                                              .GetAt(i)[0];
                                float y = (float)elementNormal->GetDirectArray()
                                              .GetAt(i)[1];
                                float z = (float)elementNormal->GetDirectArray()
                                              .GetAt(i)[2];
                                target->addNormal(glm::vec4(x, y, z, 0));
                        }
                } else {
                        throw std::logic_error("unsupported file structure");
                }
        } else if (elementNormal->GetMappingMode() ==
                   FbxLayerElement::eByControlPoint) {
                if (elementNormal->GetReferenceMode() ==
                    FbxLayerElement::eDirect) {
                        for (int i = 0;
                             i < elementNormal->GetDirectArray().GetCount();
                             i++) {
                                float x = (float)elementNormal->GetDirectArray()
                                              .GetAt(i)[0];
                                float y = (float)elementNormal->GetDirectArray()
                                              .GetAt(i)[1];
                                float z = (float)elementNormal->GetDirectArray()
                                              .GetAt(i)[2];
                                target->addNormal(glm::vec4(x, y, z, 0));
                        }
                } else {
                        throw std::logic_error("unsupported file structure");
                }
        } else {
                throw std::logic_error("unsupported file structure");
        }
}

void FbxModel::procUV(FbxNode* node, std::shared_ptr<FbxMeshInfo> target) {
        FbxMesh* fbxMesh = node->GetMesh();
        assert(fbxMesh != NULL);
        FbxLayer* fbxLayer = fbxMesh->GetLayer(0);
        assert(fbxLayer != NULL);
        FbxLayerElementUV* uvs = fbxLayer->GetUVs();
        if (uvs == NULL) {
                return;
        }
        int uvsize = std::max(uvs->GetIndexArray().GetCount(),
                              uvs->GetDirectArray().GetCount());
        FbxLayerElement::EMappingMode map = uvs->GetMappingMode();
        FbxLayerElement::EReferenceMode ref = uvs->GetReferenceMode();
        if (map == FbxLayerElement::eByPolygonVertex) {
                if (ref == FbxLayerElement::eDirect) {
                        for (int i = 0; i < uvsize; i++) {
                                FbxVector2 v2 = uvs->GetDirectArray().GetAt(i);
                                target->addUV(glm::vec2((float)v2[0],
                                                        1.0f - (float)v2[1]));
                        }
                } else if (ref == FbxLayerElement::eIndexToDirect) {
                        for (int i = 0; i < uvsize; i++) {
                                int index = uvs->GetIndexArray().GetAt(i);
                                FbxVector2 v2 =
                                    uvs->GetDirectArray().GetAt(index);
                                target->addUV(glm::vec2((float)v2[0],
                                                        1.0f - (float)v2[1]));
                        }
                } else {
                        throw std::logic_error("unsupported file structure");
                }
        } else {
                throw std::logic_error("unsupported file structure");
        }
}

void FbxModel::procMaterial(FbxNode* node,
                            std::shared_ptr<FbxMeshInfo> target) {
        FbxMesh* fbxMesh = node->GetMesh();
        // FbxNode* node = fbxMesh->GetNode();
        for (int i = 0; i < node->GetMaterialCount(); i++) {
                FbxMaterial mtl;
                mtl.shader = textureShaderName;
                FbxSurfaceMaterial* material = node->GetMaterial(i);
                FbxSurfacePhong* phong =
                    static_cast<FbxSurfacePhong*>(material);
                FbxPropertyT<FbxDouble3> d3ambient = phong->Ambient;
                mtl.color.ambient.r = (float)d3ambient.Get()[0];
                mtl.color.ambient.g = (float)d3ambient.Get()[1];
                mtl.color.ambient.b = (float)d3ambient.Get()[2];
                mtl.color.ambient.a = 1;
                FbxPropertyT<FbxDouble3> d3diffuse = phong->Diffuse;
                mtl.color.diffuse.r = (float)d3diffuse.Get()[0];
                mtl.color.diffuse.g = (float)d3diffuse.Get()[1];
                mtl.color.diffuse.b = (float)d3diffuse.Get()[2];
                mtl.color.diffuse.a = 1;
                FbxPropertyT<FbxDouble3> d3specular = phong->Specular;
                mtl.color.specular.r = (float)d3specular.Get()[0];
                mtl.color.specular.g = (float)d3specular.Get()[1];
                mtl.color.specular.b = (float)d3specular.Get()[2];
                mtl.color.specular.a = 1;
                mtl.shininess = phong->Shininess.Get();
                FbxProperty IProperty =
                    material->FindProperty(FbxSurfaceMaterial::sDiffuse);
                FbxFileTexture* tex = IProperty.GetSrcObject<FbxFileTexture>();
                if (tex) {
                        auto ptex = std::make_shared<PngTexture>();
                        ptex->load(tex->GetFileName());
                        target->addTexture(ptex);
                        mtl.textureNo = target->getTextureCount();
                        mtl.name = tex->GetFileName();
						mtl.type = FbxMaterialType::Texture;
                } else {
                        mtl.shader = colorShaderName;
                        mtl.type = FbxMaterialType::Color;
                }
                target->addMaterial(mtl);
                assert(!(target->getMaterialCount() == 0));
        }
}

void FbxModel::procSide(FbxNode* node, std::shared_ptr<FbxMeshInfo> target) {
        // assert(!dest.materials.empty());
        FbxMesh* fbxMesh = node->GetMesh();
        FbxLayer* fbxLayer = fbxMesh->GetLayer(0);
        FbxLayerElementMaterial* layerMat = fbxLayer->GetMaterials();
        assert(fbxLayer != NULL);
        if (layerMat == NULL) {
                return;
        }
        for (int i = 0; i < target->getMaterialCount(); i++) {
                auto& mat = target->getMaterialAt(i);
                mat.triVAO.init();
                mat.triVertex.init();
                mat.triUV.init();
                mat.triNormal.init();
                mat.quadVAO.init();
                mat.quadVertex.init();
                mat.quadUV.init();
                mat.quadNormal.init();
        }
        int count = 0;
        for (int k = 0; k < fbxMesh->GetPolygonCount(); k++) {
                int matId = layerMat->GetIndexArray().GetAt(k);
                int polygonSize = fbxMesh->GetPolygonSize(k);
                if (polygonSize == 3) {
                        for (int j = 0; j < 3; j++) {
                                Triangle tria;
                                tria.ver = target->getVertexAt(
                                    fbxMesh->GetPolygonVertex(k, j));
                                tria.nor = target->getNormalAt(count + j);
                                tria.uv = target->getUVAt(count + j);
                                target->getMaterialAt(matId)
                                    .triangles.push_back(tria);
                                target->getMaterialAt(matId)
                                    .triVertex.modify()
                                    .emitVec4(tria.ver.x, tria.ver.y,
                                              tria.ver.z, tria.ver.w);
                                target->getMaterialAt(matId)
                                    .triNormal.modify()
                                    .emitVec4(tria.nor.x, tria.nor.y,
                                              tria.nor.z, tria.nor.w);
                                target->getMaterialAt(matId)
                                    .triUV.modify()
                                    .emitVec2(tria.uv.x, tria.uv.y);
                        }
                        count += 3;
                } else if (polygonSize == 4) {
                        for (int j = 0; j < 4; j++) {
                                Quadrangle quad;
                                quad.ver = target->getVertexAt(
                                    fbxMesh->GetPolygonVertex(k, j));
                                quad.nor = target->getNormalAt(count + j);
                                quad.uv = target->getUVAt(count + j);
                                target->getMaterialAt(matId).quads.push_back(
                                    quad);

                                target->getMaterialAt(matId)
                                    .quadVertex.modify()
                                    .emitVec4(quad.ver.x, quad.ver.y,
                                              quad.ver.z, quad.ver.w);
                                target->getMaterialAt(matId)
                                    .quadNormal.modify()
                                    .emitVec4(quad.nor.x, quad.nor.y,
                                              quad.nor.z, quad.nor.w);
                                target->getMaterialAt(matId)
                                    .quadUV.modify()
                                    .emitVec2(quad.uv.x, quad.uv.y);
                        }
                        count += 4;
                } else {
                        throw std::logic_error("unsupported file structure");
                }
        }
        for (int i = 0; i < target->getMaterialCount(); i++) {
                auto& mat = target->getMaterialAt(i);
                mat.triVertex.bind();
                mat.triVertex.update();
                mat.triVertex.unbind();

                mat.triUV.bind();
                mat.triUV.update();
                mat.triUV.unbind();

                mat.triNormal.bind();
                mat.triNormal.update();
                mat.triNormal.unbind();

                mat.quadVertex.bind();
                mat.quadVertex.update();
                mat.quadVertex.unbind();

                mat.quadUV.bind();
                mat.quadUV.update();
                mat.quadUV.unbind();

                mat.quadNormal.bind();
                mat.quadNormal.update();
                mat.quadNormal.unbind();
                applyShader(mat);
        }
}

void FbxModel::applyShader(FbxMaterial& mat) {
        Shader& shader = ShaderRegistry::getInstance().get(mat.shader);
        shader.use();
        applyTriangle(mat);
        applyQuad(mat);
        if (mat.type == FbxMaterialType::Color) {
                shader.setUniform4f(nameRule.uniformColor, mat.mainColor.r,
                                    mat.mainColor.g, mat.mainColor.b,
                                    mat.mainColor.a);
        }
        shader.unuse();
}

void FbxModel::applyTriangle(FbxMaterial& mat) {
        Shader& shader = ShaderRegistry::getInstance().get(mat.shader);
        GLuint vertexAttrib = shader.getAttribLocation(nameRule.attribVertex);
        GLuint uvAttrib = shader.getAttribLocation(nameRule.attribUV);
        GLuint normalAttrib = shader.getAttribLocation(nameRule.attribNormal);
        mat.triVAO.bind();
        mat.triVertex.bind();
        glVertexAttribPointer(vertexAttrib, 4, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(vertexAttrib);
        mat.triNormal.bind();
        glVertexAttribPointer(normalAttrib, 4, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(normalAttrib);
        if (mat.type == FbxMaterialType::Texture) {
                mat.triUV.bind();
                glVertexAttribPointer(uvAttrib, 2, GL_FLOAT, GL_FALSE, 0, NULL);
                glEnableVertexAttribArray(uvAttrib);
        }
        mat.triVAO.unbind();
        mat.triVertex.unbind();
        mat.triNormal.unbind();
        if (mat.type == FbxMaterialType::Texture) {
                mat.triUV.unbind();
        }
}

void FbxModel::applyQuad(FbxMaterial& mat) {
        Shader& shader = ShaderRegistry::getInstance().get(mat.shader);
        GLuint vertexAttrib = shader.getAttribLocation(nameRule.attribVertex);
        GLuint uvAttrib = shader.getAttribLocation(nameRule.attribUV);
        GLuint normalAttrib = shader.getAttribLocation(nameRule.attribNormal);
        mat.quadVAO.bind();
        mat.quadVertex.bind();
        glVertexAttribPointer(vertexAttrib, 4, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(vertexAttrib);
        mat.quadNormal.bind();
        glVertexAttribPointer(normalAttrib, 4, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(normalAttrib);
        if (mat.type == FbxMaterialType::Texture) {
                mat.quadUV.bind();
                glVertexAttribPointer(uvAttrib, 2, GL_FLOAT, GL_FALSE, 0, NULL);
                glEnableVertexAttribArray(uvAttrib);
        }
        mat.quadVAO.unbind();
        mat.quadVertex.unbind();
        mat.quadNormal.unbind();
        if (mat.type == FbxMaterialType::Texture) {
                mat.quadUV.unbind();
        }
}
}  // namespace gel