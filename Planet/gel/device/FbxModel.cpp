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
      model(IRModel::create()),
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

std::shared_ptr<IRModel> FbxModel::getIRModel() const { return model; }

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
        FbxDouble3 pos = node->LclTranslation.Get();
        FbxDouble3 rot = node->LclRotation.Get();
        FbxDouble3 scl = node->LclScaling.Get();
        mesh->setLocalPosition(glm::vec4(pos[0], pos[1], pos[2], 1.0f));
        mesh->setLocalRotation(glm::vec4(rot[0], rot[1], rot[2], 0.0f));
        mesh->setLocalScale(glm::vec4(scl[0], scl[1], scl[2], 1.0f));

        procIRVertex(node, mesh);
        procIRIndex(node, mesh);
        procIRNormal(node, mesh);
        procIRUV(node, mesh);

        procIRMaterials(node, mesh);
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
        FbxLayerElementNormal* elementNormal = findElementNormal(fbxMesh);
        FbxLayerElement::EMappingMode map = elementNormal->GetMappingMode();
        FbxLayerElement::EReferenceMode ref = elementNormal->GetReferenceMode();
        if (map == FbxLayerElement::eByPolygonVertex) {
                if (ref == FbxLayerElement::eDirect) {
                        auto directArray = elementNormal->GetDirectArray();
                        for (int i = 0; i < directArray.GetCount(); i++) {
                                float x = (float)directArray.GetAt(i)[0];
                                float y = (float)directArray.GetAt(i)[1];
                                float z = (float)directArray.GetAt(i)[2];
                                mesh->getShape()->addNormal(
                                    glm::vec4(x, y, z, 0));
                        }
                } else {
                        throw std::logic_error("unsupported file structure");
                }
        } else if (map == FbxLayerElement::eByControlPoint) {
                if (ref == FbxLayerElement::eDirect) {
                        auto directArray = elementNormal->GetDirectArray();
                        for (int i = 0; i < directArray.GetCount(); i++) {
                                float x = (float)directArray.GetAt(i)[0];
                                float y = (float)directArray.GetAt(i)[1];
                                float z = (float)directArray.GetAt(i)[2];
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
        FbxLayerElementUV* uvs = findElementUV(fbxMesh);
        FbxLayerElement::EMappingMode map = uvs->GetMappingMode();
        FbxLayerElement::EReferenceMode ref = uvs->GetReferenceMode();
        if (map == FbxLayerElement::eByPolygonVertex) {
                if (ref == FbxLayerElement::eDirect) {
                        auto directArray = uvs->GetDirectArray();
                        int directLength = directArray.GetCount();
                        for (int i = 0; i < directLength; i++) {
                                FbxVector2 v2 = directArray.GetAt(i);
                                mesh->getShape()->addUV(glm::vec2(
                                    (float)v2[0], 1.0f - (float)v2[1]));
                        }
                } else if (ref == FbxLayerElement::eIndexToDirect) {
                        auto indexArray = uvs->GetIndexArray();
                        int indexLength = indexArray.GetCount();
                        for (int i = 0; i < indexLength; i++) {
                                int index = indexArray.GetAt(i);
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

void FbxModel::procIRMaterials(FbxNode* node, std::shared_ptr<IRMesh> mesh) {
        for (int i = 0; i < node->GetMaterialCount(); i++) {
                auto mat = node->GetMaterial(i);
                std::cout << "material [" << node->GetName() << "] "
                          << mat->GetName() << std::endl;
                procIRMaterial(node, mesh, mat);
        }
}

void FbxModel::procIRMaterial(FbxNode* node, std::shared_ptr<IRMesh> mesh,
                              FbxSurfaceMaterial* fbxMat) {
        FbxClassId classId = fbxMat->GetClassId();
        if (classId.Is(FbxSurfaceLambert::ClassId)) {
                procIRLambert(node, mesh, (FbxSurfaceLambert*)fbxMat);
        } else if (classId.Is(FbxSurfacePhong::ClassId)) {
                procIRPhong(node, mesh, (FbxSurfacePhong*)fbxMat);
        } else {
                assert(false);
        }
}

void FbxModel::procIRLambert(FbxNode* node, std::shared_ptr<IRMesh> mesh,
                             FbxSurfaceLambert* fbxMat) {
        std::shared_ptr<IRMaterial> mtl = mesh->addMaterial();
        mtl->setShader(textureShaderName);
        FbxPropertyT<FbxDouble3> d3ambient = fbxMat->Ambient;
        FbxPropertyT<FbxDouble3> d3diffuse = fbxMat->Diffuse;
        // FbxPropertyT<FbxDouble3> d3specular = fbxMat->Specular;
        mtl->setAmbient(Color4((float)d3ambient.Get()[0],
                               (float)d3ambient.Get()[1],
                               (float)d3ambient.Get()[2], 1));
        mtl->setDiffuse(Color4((float)d3diffuse.Get()[0],
                               (float)d3diffuse.Get()[1],
                               (float)d3diffuse.Get()[2], 1));
        mtl->setSpecular(Color4(1, 1, 1, 1));
        mtl->setShininess(1);
        procIRTexture(node, mesh, fbxMat, mtl);
}

void FbxModel::procIRPhong(FbxNode* node, std::shared_ptr<IRMesh> mesh,
                           FbxSurfacePhong* fbxMat) {
        std::shared_ptr<IRMaterial> mtl = mesh->addMaterial();
        mtl->setShader(textureShaderName);
        FbxPropertyT<FbxDouble3> d3ambient = fbxMat->Ambient;
        FbxPropertyT<FbxDouble3> d3diffuse = fbxMat->Diffuse;
        FbxPropertyT<FbxDouble3> d3specular = fbxMat->Specular;
        mtl->setAmbient(Color4((float)d3ambient.Get()[0],
                               (float)d3ambient.Get()[1],
                               (float)d3ambient.Get()[2], 1));
        mtl->setDiffuse(Color4((float)d3diffuse.Get()[0],
                               (float)d3diffuse.Get()[1],
                               (float)d3diffuse.Get()[2], 1));
        mtl->setSpecular(Color4((float)d3specular.Get()[0],
                                (float)d3specular.Get()[1],
                                (float)d3specular.Get()[2], 1));
        mtl->setShininess(fbxMat->Shininess.Get());
        procIRTexture(node, mesh, fbxMat, mtl);
}

void FbxModel::procIRSide(FbxNode* node, std::shared_ptr<IRMesh> mesh) {
        // assert(!dest.materials.empty());
        std::string nodename = node->GetName();
        FbxMesh* fbxMesh = node->GetMesh();
        FbxLayerElementMaterial* layerMat = findElementMaterial(fbxMesh);
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
                                int vindex = fbxMesh->GetPolygonVertex(k, j);
                                tria.ver = shape->getVertexAt(vindex);
                                tria.nor = shape->getNormalAt(count + j);
                                tria.uv = shape->getUVAt(count + j);
                                mesh->getMaterial(matId)->addTriangle(tria);
                        }
                        count += 3;
                } else if (polygonSize == 4) {
                        for (int j = 0; j < 4; j++) {
                                Quadrangle quad;
                                int vindex = fbxMesh->GetPolygonVertex(k, j);
                                quad.ver = shape->getVertexAt(vindex);
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
void FbxModel::procIRTexture(FbxNode* node, std::shared_ptr<IRMesh> mesh,
                             FbxSurfaceMaterial* fbxMat,
                             std::shared_ptr<IRMaterial> irMat) {
        FbxProperty IProperty =
            fbxMat->FindProperty(FbxSurfaceMaterial::sDiffuse);
        FbxFileTexture* tex = IProperty.GetSrcObject<FbxFileTexture>();
        if (tex) {
                auto ptex = std::make_shared<PngTexture>();
                auto path = tex->GetFileName();
                if (!exists(path)) {
                        throw std::logic_error("file is not found");
                }
                ptex->load(path);
                mesh->addTexture(ptex);
                irMat->setTexture(mesh->getTextureCount());
                irMat->setShader(textureShaderName);
                irMat->setType(IRMaterialType::Texture);
        } else {
                irMat->setShader(colorShaderName);
                irMat->setType(IRMaterialType::Color);
        }
}
bool FbxModel::hasMeshAttr(FbxNode* node) const {
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

FbxLayerElementNormal* FbxModel::findElementNormal(FbxMesh* mesh) {
        for (int i = 0; i < mesh->GetLayerCount(); i++) {
                auto layer = mesh->GetLayer(i);
                auto normals = layer->GetNormals();
                if (normals != NULL) {
                        return normals;
                }
        }
        return nullptr;
}

FbxLayerElementUV* FbxModel::findElementUV(FbxMesh* mesh) {
        for (int i = 0; i < mesh->GetLayerCount(); i++) {
                auto layer = mesh->GetLayer(i);
                auto uvs = layer->GetUVs();
                if (uvs != NULL) {
                        return uvs;
                }
        }
        return nullptr;
}
FbxLayerElementMaterial* FbxModel::findElementMaterial(FbxMesh* mesh) {
        for (int i = 0; i < mesh->GetLayerCount(); i++) {
                auto layer = mesh->GetLayer(i);
                auto mat = layer->GetMaterials();
                if (mat != NULL) {
                        return mat;
                }
        }
        return nullptr;
}
}  // namespace gel