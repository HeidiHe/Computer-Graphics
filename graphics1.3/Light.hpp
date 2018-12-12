/*
 * File: Light.java
 * F18 CS451 Project 1
 * Names: Liwei Jiang, Heidi He
 * Date: 09/22/2018
 */


class Light {
    vec3 La;
    vec3 Le;
    vec4 worldLightPosition;
    
public:
    //    Light():La(La), Le(Le), worldLightPosition(worldLightPosition)
    Light(vec3 la=(1,1,1), vec3 le=(1,1,1), vec4 wlp=(1,1,1,1)) {
        La = la; 
        Le = le; //light color
        worldLightPosition = wlp;
    }
    
    void UploadAttributes(Shader* shader){
        shader->UploadLightAttributes(La, Le, worldLightPosition);
        
    }
    
    void combineLight(Light newLight){
        
    }
    
    vec3 getLa(){
        return La;
    }
    
    vec3 getLe(){
        return Le;
    }
    
    vec4 getWlp(){
        return worldLightPosition;
    }
    
    //the last entry for point light source is 1
    void SetPointLightSource(vec3& pos) {
        worldLightPosition = vec4(pos.x, pos.y, pos.z, 1.0);
    }
    
    //the last entry for point light source is 0
    void SetDirectionalLightSource(vec3& dir){
        worldLightPosition = vec4(dir.x, dir.y, dir.z, 0.0);
    }
    
};
