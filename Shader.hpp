/*
 * File: Shader.java
 * F18 CS451 Project 1
 * Names: Liwei Jiang, Heidi He
 * Date: 09/22/2018
 */

//shader program parent class
class Shader {
protected:
    unsigned int shaderProgram;
    
public:
    Shader() {
        shaderProgram = 0;
    }
    
    ~Shader() {
        if(shaderProgram) glDeleteProgram(shaderProgram);
    }
    
    void Run() {
        if(shaderProgram) glUseProgram(shaderProgram);
    }
    
    virtual void UploadInvM(mat4& InVM) { }
    
    virtual void UploadMVP(mat4& MVP) { }
    
    virtual void UploadM(mat4& M) { }
    
    virtual void UploadMaterialAttributes(vec3& ka, vec3& kd, vec3& ks, float shine){}
    
    virtual void UploadLightAttributes(vec3& la, vec3& le, vec4& wlp){}
    
    virtual void UploadEyePosition(vec3& wEye){}
    
    virtual void UploadSamplerID() { }
    
    virtual void UploadVP(mat4& VP){}
};
