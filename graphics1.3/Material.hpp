/*
 * File: Material.hpp
 * F18 CS451 Project 1
 * Names: Liwei Jiang, Heidi He
 * Date: 09/22/2018
 */

// derives material from a texture image
class Material {
    Shader* shader;
    Texture* texture;
    vec3 ka; //original color
    vec3 kd; //for diffussion
    vec3 ks; //for shining
    float shininess;
    
public:
    Material(Shader* s, Texture* t=0, vec3 _ka=vec3(0,0,0), vec3 _kd=vec3(0,0,0), vec3 _ks=vec3(0,0,0), float _shs=0.5) {
        ka = _ka;
        kd = _kd;
        ks = _ks;
        shininess = _shs;
        shader = s;
        texture = t;
    }
    
    Shader* GetShader() {
        return shader;
    }
    
    //upload attribute from shader
    void UploadAttributes() {
        if(texture) {
            shader->UploadMaterialAttributes(ka, kd, ks, shininess);
            shader->UploadSamplerID();
            texture->Bind();
        }
    }
};
