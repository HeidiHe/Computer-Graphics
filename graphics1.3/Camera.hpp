/*
 * File: Camera.hpp
 * F18 CS451 Project 1
 * Names: Liwei Jiang, Heidi He
 * Date: 09/22/2018
 */


//camera that is able to takes in keyboard control and is able to look at object
class Camera {
    vec3  wEye, wLookat, wVup, velocity;
    float fov, asp, fp, bp, angularVelocity;
    
public:
    Camera() {
        wEye = vec3(0.0, 0.0, 2.0);
        wLookat = vec3(0.0, 0.0, 0.0);
        wVup = vec3(0.0, 1.0, 0.0);
        
        velocity = vec3(0.0,0.0,0.0);
        angularVelocity = 0.0;
        fov = M_PI / 4.0;
        asp = 1.0; //aspect ratio
        fp = 0.01;
        bp = 1000000.0; //background curtain
    }
    
    void SetAspectRatio(float a) {
        asp = a;
    }
    
    //view matrix
    mat4 GetViewMatrix() {
        vec3 w = (wEye - wLookat).normalize();
        vec3 u = cross(wVup, w).normalize();
        vec3 v = cross(w, u);
        
        return
        mat4(
             1.0f,    0.0f,    0.0f,    0.0f,
             0.0f,    1.0f,    0.0f,    0.0f,
             0.0f,    0.0f,    1.0f,    0.0f,
             -wEye.x, -wEye.y, -wEye.z, 1.0f ) *
        mat4(
             u.x,  v.x,  w.x,  0.0f,
             u.y,  v.y,  w.y,  0.0f,
             u.z,  v.z,  w.z,  0.0f,
             0.0f, 0.0f, 0.0f, 1.0f );
    }
    
    // projection matrix
    mat4 GetProjectionMatrix() {
        float sy = 1/tan(fov/2);
        return mat4(
                    sy/asp, 0.0f,  0.0f,               0.0f,
                    0.0f,   sy,    0.0f,               0.0f,
                    0.0f,   0.0f, -(fp+bp)/(bp - fp), -1.0f,
                    0.0f,   0.0f, -2*fp*bp/(bp - fp),  0.0f);
    }
    
    //camera control
    void Control() {
        velocity = 0.0; angularVelocity = 0.0;
        if(keyboardState['a']) angularVelocity = 0.1*5;
        if(keyboardState['d']) angularVelocity = -0.1*5;
        if(keyboardState['s']) {
            velocity=(wEye - wLookat).normalize()*5;
        }
        if(keyboardState['w']) {
            velocity=(wLookat - wEye).normalize()*5;
        }
        
    }
    
    //rotate by angle. av is angular velocity
    void Rotate(float av) {
        float a = av;
        float l = (wEye - wLookat).length();
        vec3 w = (wEye - wLookat).normalize();
        vec3 u = cross(wVup, w).normalize();
        vec3 wNew = w*cos(a) + u*sin(a);
        wEye=wLookat+wNew*l;
    }
    
    //look at the avatar all the time
    void Track(float dt) {
        vec3 d = dt*sin(dt);
        float a = sin(dt);
        wEye = wEye + d;
        wLookat = wLookat + d;
        float l = (wEye - wLookat).length();
        vec3 w = (wEye - wLookat).normalize();
        vec3 u = cross(wVup, w).normalize();
        vec3 wNew = w*cos(a) + u * sin(a);
        wEye = wLookat + wNew*l;
    }
    
    //move forward or backward
    void Move(float dt) {
        vec3 d = velocity * dt;
        float a = angularVelocity * dt *2;
        wEye = wEye + d;
        wLookat = wLookat + d;
        float l = (wEye - wLookat).length();
        vec3 w = (wEye - wLookat).normalize();
        vec3 u = cross(wVup, w).normalize();
        vec3 wNew = w*cos(a) + u*sin(a);
        wLookat = wEye - wNew*l;
    }
    
    //shader that upload attribute
    void UploadAttributes(Shader* shader) {
        shader->UploadEyePosition(wEye);
    }
    
    
    vec3 getEyePosition() {
        return wEye;
    }
    
    vec3 getLookat() {
        return wLookat;
    }
    
    void updateEye(float distance) {
        vec3 w = (wEye - wLookat).normalize();
        wEye=wLookat+w*distance;
    }
    
    void setEyePosition(vec3 newEye) {
        wEye = newEye;
    }
    
    void setLookat(vec3 newLookat) {
        wLookat = newLookat;
    }
    
    void setCamera(vec3 newEye, vec3 newLookat) {
        wEye = newEye;
        wLookat = newLookat;
        wVup = vec3(0.0, 1.0, 0.0);
    }
};
