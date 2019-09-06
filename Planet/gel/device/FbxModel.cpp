#include "FbxModel.hpp"
#include <algorithm>
#include <cassert>
#include <iostream>
#include <stdexcept>
#include "../shader/IRMaterial.hpp"
#include "../shader/IRMesh.hpp"
#include "../shader/IRShape.hpp"
#include "../shader/ShaderRegistry.hpp"
#include "../util/io.hpp"
namespace gel {
FbxModel::FbxModel(FbxManager* fbxManager, const std::string& textureShaderName,
                   const std::string& colorShaderName, const NameRule& nameRule)
    : fbxManager(fbxManager),
      fbxScene(nullptr),
      aabb(),
      model(std::make_shared<IRModel>()),
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
        procIRRec(rootNode, model->getMesh(), 0);
}

void FbxModel::unload(const std::string& path) {
        fbxImporter->Destroy();
        fbxScene->Destroy();
}

void FbxModel::draw() { drawIR(model->getMesh()); }

AABB FbxModel::getAABB() const { return aabb; }

// private
void FbxModel::drawIR(std::shared_ptr<IRMesh> mesh) {
        for (int i = 0; i < mesh->getMaterialCount(); i++) {
                mesh->getMaterial(i)->draw(mesh, nameRule);
        }
        for (int i = 0; i < mesh->getMeshCount(); i++) {
                drawIR(mesh->getMesh(i));
        }
}

void FbxModel::procIRRec(FbxNode* node, std::shared_ptr<IRMesh> mesh,
                         int depth) {
        indent(depth);
        std::cout << node->GetName() << std::endl;
        for (int i = 0; i < node->GetChildCount(); i++) {
                FbxNode* child = node->GetChild(i);
                procIRRec(child, mesh->addMesh(child->GetName()), depth + 1);
        }
        procIR(node, mesh, depth);
}

void FbxModel::procIR(FbxNode* node, std::shared_ptr<IRMesh> mesh, int depth) {
        if (!hasMeshAttr(node)) {
                return;
        }
        /*
std::string nodename = node->GetName();
std::shared_ptr<IRShape> shape = mesh->getShape();
// get vertex and normals
FbxMesh* fbxMesh = node->GetMesh();
FbxVector4 vn;
const int sVertexCount = fbxMesh->GetControlPointsCount();
int sPolygonCount = fbxMesh->GetPolygonCount();
for (int i = 0; i < sPolygonCount; i++) {
        int sPolygonSize = fbxMesh->GetPolygonSize(i);
        for (int pol = 0; pol < sPolygonSize; pol++) {
                int vertex_index = fbxMesh->GetPolygonVertex(i, pol);
                FbxVector4 vt =
                    fbxMesh->GetControlPointAt(vertex_index);
                assert(fbxMesh->GetPolygonVertexNormal(i, pol, vn));
                shape->addVertex(glm::vec4(vt[0], vt[1], vt[2], vt[3]));
                //                        shape->addNormal(glm::vec4(vn[0],
                //                        vn[1], vn[2], vn[3]));
                indent(depth + 1);
                std::cout << "vt " << vt[0] << " " << vt[1] << " "
                          << vt[2] << " " << vt[3] << std::endl;
                indent(depth + 1);
                std::cout << "vn " << vn[0] << " " << vn[1] << " "
                          << vn[2] << " " << vn[3] << std::endl;
        }
}
        */
        /*
// get uv name
FbxVector2 texCoord;
FbxStringList uvsetName;
fbxMesh->GetUVSetNames(uvsetName);

int sNumUVSet = uvsetName.GetCount();
bool bIsUnmapped = false;
for (int i = 0; i < sNumUVSet; i++) {
        for (int j = 0; j < sPolygonCount; j++) {
                int sPolygonsize = fbxMesh->GetPolygonSize(i);
                for (int k = 0; k < sPolygonsize; k++) {
                        FbxString name = uvsetName.GetStringAt(i);
                        fbxMesh->GetPolygonVertexUV(
                            j, k, name, texCoord, bIsUnmapped);
                        shape->addUV(
                            glm::vec2(texCoord[0], 1.0f - texCoord[1]));
                }
        }
}
        */
        procIRVertex(node, mesh);
        procIRIndex(node, mesh);
        procIRNormal(node, mesh);
        procIRUV(node, mesh);

        procIRMaterial(node, mesh);
        procIRSide(node, mesh);
}

void FbxModel::procIRVertex(FbxNode* node, std::shared_ptr<IRMesh> mesh) {
        FbxMesh* fbxMesh = node->GetMesh();
        FbxVector4* coord = fbxMesh->GetControlPoints();
        for (int i = 0; i < fbxMesh->GetControlPointsCount(); i++) {
                float x = (float)coord[i][0];
                float y = (float)coord[i][1];
                float z = (float)coord[i][2];
                mesh->getShape()->addVertex(glm::vec4(x, y, z, 1));
        }
}

void FbxModel::procIRIndex(FbxNode* node, std::shared_ptr<IRMesh> mesh) {
        FbxMesh* fbxMesh = node->GetMesh();
        int* vindex = fbxMesh->GetPolygonVertices();
        for (int i = 0; i < fbxMesh->GetPolygonCount() * 3; i++) {
                mesh->getShape()->addIndex(vindex[i]);
        }
}

void FbxModel::procIRNormal(FbxNode* node, std::shared_ptr<IRMesh> mesh) {
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
                                mesh->getShape()->addNormal(
                                    glm::vec4(x, y, z, 0));
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
                                mesh->getShape()->addNormal(
                                    glm::vec4(x, y, z, 0));
                        }
                } else {
                        throw std::logic_error("unsupported file structure");
                }
        } else {
                throw std::logic_error("unsupported file structure");
        }
}

void FbxModel::procIRUV(FbxNode* node, std::shared_ptr<IRMesh> mesh) {
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
                                mesh->getShape()->addUV(glm::vec2(
                                    (float)v2[0], 1.0f - (float)v2[1]));
                        }
                } else if (ref == FbxLayerElement::eIndexToDirect) {
                        for (int i = 0; i < uvsize; i++) {
                                int index = uvs->GetIndexArray().GetAt(i);
                                FbxVector2 v2 =
                                    uvs->GetDirectArray().GetAt(index);
                                mesh->getShape()->addUV(glm::vec2(
                                    (float)v2[0], 1.0f - (float)v2[1]));
                        }
                } else {
                        throw std::logic_error("unsupported file structure");
                }
        } else {
                throw std::logic_error("unsupported file structure");
        }
}

void FbxModel::procIRMaterial(FbxNode* node, std::shared_ptr<IRMesh> mesh) {
        FbxMesh* fbxMesh = node->GetMesh();
        int matCount = node->GetMaterialCount();
        for (int i = 0; i < matCount; i++) {
                std::shared_ptr<IRMaterial> mtl = mesh->addMaterial();
                mtl->setShader(textureShaderName);
                FbxSurfaceMaterial* material = node->GetMaterial(i);
                FbxSurfacePhong* phong =
                    static_cast<FbxSurfacePhong*>(material);
                FbxPropertyT<FbxDouble3> d3ambient = phong->Ambient;
                FbxPropertyT<FbxDouble3> d3diffuse = phong->Diffuse;
                FbxPropertyT<FbxDouble3> d3specular = phong->Specular;
                mtl->setAmbient(Color4((float)d3ambient.Get()[0],
                                       (float)d3ambient.Get()[1],
                                       (float)d3ambient.Get()[2], 1));
                mtl->setDiffuse(Color4((float)d3diffuse.Get()[0],
                                       (float)d3diffuse.Get()[1],
                                       (float)d3diffuse.Get()[2], 1));
                mtl->setSpecular(Color4((float)d3specular.Get()[0],
                                        (float)d3specular.Get()[1],
                                        (float)d3specular.Get()[2], 1));
                mtl->setShininess(phong->Shininess.Get());
                FbxProperty IProperty =
                    material->FindProperty(FbxSurfaceMaterial::sDiffuse);
                FbxFileTexture* tex = IProperty.GetSrcObject<FbxFileTexture>();
                if (tex) {
                        // mtl->setShader(colorShaderName);
                        // mtl->setType(IRMaterialType::Color);
                        //*
                        auto ptex = std::make_shared<PngTexture>();
                        auto path = tex->GetFileName();
                        if (!exists(path)) {
                                throw std::logic_error("file is not found");
                        }
                        ptex->load(path);
                        mesh->addTexture(ptex);
                        mtl->setTexture(mesh->getTextureCount());
                        mtl->setShader(textureShaderName);
                        mtl->setType(IRMaterialType::Texture);
                        //*/
                } else {
                        mtl->setShader(colorShaderName);
                        mtl->setType(IRMaterialType::Color);
                }
        }
}

void FbxModel::procIRSide(FbxNode* node, std::shared_ptr<IRMesh> mesh) {
        // assert(!dest.materials.empty());
        std::string nodename = node->GetName();
        FbxMesh* fbxMesh = node->GetMesh();
        FbxLayer* fbxLayer = fbxMesh->GetLayer(0);
        FbxLayerElementMaterial* layerMat = fbxLayer->GetMaterials();
        assert(fbxLayer != NULL);
        if (layerMat == NULL) {
                return;
        }
        for (int i = 0; i < mesh->getMaterialCount(); i++) {
                auto mat = mesh->getMaterial(i);
                mat->init();
        }
        int count = 0;
        std::shared_ptr<IRShape> shape = mesh->getShape();
        int poly = fbxMesh->GetPolygonCount();
        for (int k = 0; k < poly; k++) {
                int matId = layerMat->GetIndexArray().GetAt(k);
                int polygonSize = fbxMesh->GetPolygonSize(k);
                if (polygonSize == 3) {
                        for (int j = 0; j < 3; j++) {
                                Triangle tria;
                                tria.ver = shape->getVertexAt(
                                    fbxMesh->GetPolygonVertex(k, j));
                                tria.nor = shape->getNormalAt(count + j);
                                tria.uv = shape->getUVAt(count + j);
                                mesh->getMaterial(matId)->addTriangle(tria);
                        }
                        count += 3;
                } else if (polygonSize == 4) {
                        for (int j = 0; j < 4; j++) {
                                Quadrangle quad;
                                quad.ver = shape->getVertexAt(
                                    fbxMesh->GetPolygonVertex(k, j));
                                quad.nor = shape->getNormalAt(count + j);
                                quad.uv = shape->getUVAt(count + j);
                                mesh->getMaterial(matId)->addQuad(quad);
                        }
                        count += 4;
                } else {
                        throw std::logic_error("unsupported file structure");
                }
        }
        for (int i = 0; i < mesh->getMaterialCount(); i++) {
                std::shared_ptr<IRMaterial> mat = mesh->getMaterial(i);
                mat->update();
        }
}
bool FbxModel::hasMeshAttr(FbxNode* node) const {
        /*
if (node) {
                int attrCount = node->GetNodeAttributeCount();

                for (int i = 0; attrCount > i; i++) {
                                FbxNodeAttribute::EType attrType =
                                        node->GetNodeAttributeByIndex(i)
                                                ->GetAttributeType();

                                // ノードがメッシュにつながっているかチェック
                                if (attrType == FbxNodeAttribute::EType::eMesh)
{ return true;
                                }
                }
}
return false;
                */
        auto attr = node->GetNodeAttribute();
        if (attr == NULL) {
                return false;
        }
        auto type = attr->GetAttributeType();
        if (type == FbxNodeAttribute::EType::eMesh) {
                return true;
        }
        return false;
}
void FbxModel::indent(int depth) const {
        for (int i = 0; i < depth; i++) {
                std::cout << "    ";
        }
}
}  // namespace gel