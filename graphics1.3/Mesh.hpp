/*
 * File: Mesh.hpp
 * F18 CS451 Project 1
 * Names: Liwei Jiang, Heidi He
 * Date: 09/22/2018
 */


//Mesh Object links Geometry (from obj file) and Material
class Mesh {
    Geometry* geometry;
    Material* material;
    
public:
    Mesh(Geometry* g, Material* m) {
        geometry = g;
        material = m;
    }
    
    Shader* GetShader() { return material->GetShader(); }
    
    void Draw() {
        material->UploadAttributes();
        geometry->Draw();
    }
};
