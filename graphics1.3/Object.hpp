/*
 * File: Object.java
 * F18 CS451 Project 1
 * Names: Liwei Jiang, Heidi He
 * Date: 09/22/2018
 */

//build up camera in the scene
Camera camera;

//build up light source in the scene
Light natureLight = *new Light(vec3(1,1,1), vec3(1,1,1), vec4(1000,1000,0,0));
Light spotlight = *new Light(vec3(0.0,0.6,0.6), vec3(0.6,0.6,0.6), vec4(1,1,1,1));
Light light = natureLight;

/*an object is made up of a mesh with orientation, Scale, Position*/
class Object {
    Shader* shader;
    Mesh *mesh;
    vec3 position;
    vec3 scaling;
    float orientation;
    float roll=0;
    float pitch=0;
    
    //for game & animation
    protected: vec3 velocity;
    protected: vec3 acceleration;
    protected: float angularVelocity;
    protected: float angularAcceleration;
    
    //for gravity
    bool hasGravity;
    enum OBJECT_TYPE { Avatar, Ball, StillObject};
    
    //either visibale
    bool visible;


    
public:
    Object(Mesh *m, vec3 position = vec3(0.0, 0.0, 0.0), vec3 scaling = vec3(1.0, 1.0, 1.0), float orientation = 0.0) : position(position), scaling(scaling), orientation(orientation) {
        shader = m->GetShader();
        mesh = m;
        velocity = vec3(0,0,0);
        acceleration = vec3(0,0,0);
        angularVelocity = 0;
        angularAcceleration = 0;
        
        hasGravity = false;
        visible = true;
    }
    
    /* virtual class for collision detection*/
    virtual bool Interact(Object* object) = 0;
    
    virtual OBJECT_TYPE GetType() = 0;
    
    virtual bool checkCollision(Object* object) = 0;
    
    void setVisible(bool vis){
        visible = vis;
    }
    
    bool getVisible(){
        return visible;
    }
    
    void setVelocity(vec3 newV){
        velocity = newV;
    }
    
    /*updates velocity, angularVelocity, position, and orientation depending on acceleration, angularAcceleration, and dt*/
    void Move(float dt) {
        velocity = velocity + acceleration*dt;
        /* assume ground is y=0 */
        if(touchGround()){
            
            float square = pow(velocity.y,2);
            float squareroot = sqrt(square);
            /*implement energy lost*/
            if(squareroot>0.05){
                velocity.y = squareroot-0.05;
            }else{
                velocity.y=0;
            }
            
        }else{
            /* if has gracity, then fall down */
            if(hasGravity){
                velocity = velocity + vec3(0, -0.02, 0)*dt;
            } 
        }
        
        angularVelocity += angularAcceleration*dt;
        
        position = position + velocity*dt;
        orientation += angularVelocity*dt;
    }
    
    /*detect whether has touched ground*/
    bool touchGround(){
        bool hasTouchedGround = false;
        float radius = scaling.y;
        float bottomPos = position.y - radius;
//        printf("bottomPos is %f, Y pos is %f, radius is %f \n", bottomPos, position.y, radius);
        if(bottomPos<-0.5){
            hasTouchedGround = true;
        }
        return hasTouchedGround;
    }
    
    
//    void accelerate(float dt){
//        acceleration += dt;
//    }
//    
    
    void setGravity(bool newG ){
        hasGravity = newG;
    }
    
    bool getGravity(){
        return hasGravity;
    }
    
    vec3& GetPosition() {
        return position;
    }

    void SetPosition(vec3& newPosition) {
        position = newPosition;
    }
    
    float GetOrientation() {
        return orientation;
    }
    
    void SetOrientation(float newO){
        orientation=newO;
    }
    
    //draw: using shader to upload attribute from camera and light
    void Draw(){
        shader->Run();
        UploadAttributes(shader);
        
        camera.UploadAttributes(shader);
        light.UploadAttributes(shader);
        mesh->Draw();
    }
    
    //draw shadow by shader
    void DrawShadow(Shader* shadowShader) {
        shadowShader->Run();
        UploadAttributes(shadowShader);
        
        light.UploadAttributes(shadowShader);
        camera.UploadAttributes(shadowShader);
        
        mesh->Draw();
    }
    
    //rotate by increasing orientation
    void Rotate() {
        orientation = orientation+1;
    }
    
    //pitch the object (pitch: wing axis
    void SetPitch(float pitchAngle) {
        pitch = pitchAngle;
    }
    
    //roll the object (roll: forward axis
    void SetRoll(float rollAngle) {
        roll = rollAngle;
    }
    
    float GetRoll() {
        return roll;
    }
    
    vec3 GetScale(){
        return scaling;
    }
    
    //upload vertix attribute
    void UploadAttributes(Shader* thisShader) {
        mat4 T = mat4(
                      1.0,            0.0,            0.0,            0.0,
                      0.0,            1.0,            0.0,            0.0,
                      0.0,            0.0,            1.0,            0.0,
                      position.x,        position.y,        position.z,        1.0);
        
        mat4 InvT = mat4(
                         1.0,            0.0,            0.0,            0.0,
                         0.0,            1.0,            0.0,            0.0,
                         0.0,            0.0,            1.0,            0.0,
                         -position.x,    -position.y,    -position.z,    1.0);
        
        mat4 S = mat4(
                      scaling.x,        0.0,            0.0,            0.0,
                      0.0,            scaling.y,        0.0,            0.0,
                      0.0,            0.0,            scaling.z,        0.0,
                      0.0,            0.0,            0.0,            1.0);
        
        mat4 InvS = mat4(
                         1.0/scaling.x,    0.0,            0.0,            0.0,
                         0.0,            1.0/scaling.y,    0.0,            0.0,
                         0.0,            0.0,            1.0/scaling.z,    0.0,
                         0.0,            0.0,            0.0,            1.0);
        
        float alpha = orientation / 180.0 * M_PI;
        
        mat4 R = mat4(
                      cos(alpha),        0.0,            sin(alpha),        0.0,
                      0.0,            1.0,            0.0,            0.0,
                      -sin(alpha),    0.0,            cos(alpha),        0.0,
                      0.0,            0.0,            0.0,            1.0);
        
        mat4 R2 = mat4(1,0,0,0,
                       0,1,0,0,
                       0,0,1,0,
                       0,0,0,1);
        
        if (roll!=0) {
            float alpha2 = roll / 180.0 * M_PI;
            R2 = mat4(
                      cos(alpha2),        -sin(alpha2),            0.0,        0.0,
                      sin(alpha2),        cos(alpha2),            0.0,            0.0,
                      0.0,          0.0,            1.0,        0.0,
                      0.0,            0.0,            0.0,            1.0);
        }
        
        R = R2 * R;
        
        mat4 InvR = mat4(
                         cos(alpha),        0.0,            -sin(alpha),    0.0,
                         0.0,            1.0,            0.0,            0.0,
                         sin(alpha),        0.0,            cos(alpha),        0.0,
                         0.0,            0.0,            0.0,            1.0);
        
        mat4 M = S * R * T;
        mat4 InvM = InvT * InvR * InvS;
        
        mat4 MVP = M * camera.GetViewMatrix() * camera.GetProjectionMatrix();
        mat4 VP = camera.GetViewMatrix() * camera.GetProjectionMatrix();
        thisShader->UploadM(M);
        thisShader->UploadInvM(InvM);
        thisShader->UploadMVP(MVP);
        thisShader->UploadVP(VP);
    }
};



//the avater object of the scene
class Avatar: public Object{
public:
    Avatar(Mesh *m, vec3 position, vec3 scaling, float orientation) :
        Object(m, position, scaling,orientation){}
    
    bool checkCollision(Object* object){
        float rangeXH = object->GetPosition().x + object->GetScale().x + 1;
        float rangeXL = object->GetPosition().x - object->GetScale().x - 1;
        float rangeY = object->GetPosition().y + object->GetScale().y;
        float rangeZH = object->GetPosition().z + object->GetScale().z +1 ;
        float rangeZL = object->GetPosition().z - object->GetScale().z - 1;
        if((GetPosition().x<rangeXH)&&(GetPosition().x>rangeXL)){
//            if((GetPosition().y<rangeY+1)&&(GetPosition().y>rangeY-1)){
                if((GetPosition().z<rangeZH)&&(GetPosition().z>rangeZL)){
                    printf("COLLIDE!!!");
                    return Interact(object);
                }
//            }
        }
        return false;
    }
    
    bool Interact(Object* object) {
        switch (object->GetType()) {
            /* avatar: both bounce*/
            case Object::Avatar:
                printf("Avatar bounce to itself \n");
                return false;
                break;
                
            /* ball: ball explode*/
            case Object::Ball:
                printf("ball explode, collision wave impact the avatar \n");
                velocity = vec3(0,0,0)-velocity;
                angularVelocity = -angularVelocity;
                object->setVisible(false);
                return false;
                break;
                
            /* still object: bounce back*/
            case Object::StillObject:
                printf("hit on an object \n");
//                velocity = vec3(0,0,0)-velocity;
                velocity = vec3(0,0,0);
                printf("velocity z is %f \n", velocity.z);
                angularVelocity = -angularVelocity;
                return true;
                break;
                
            default:
                printf("no type \n");
                return false;
                break;
        }
        return false;

    }
    
    OBJECT_TYPE GetType(){
        return OBJECT_TYPE::Avatar;
    }
};



//balls that will move during collision
class Ball : public Object{
public:
    Ball(Mesh *m, vec3 position, vec3 scaling, float orientation) :
        Object(m, position, scaling,orientation){}
    
    bool Interact(Object* object) {
        return false;
    
    }
    
    OBJECT_TYPE GetType(){
        return OBJECT_TYPE::Ball;
    }
    
    bool checkCollision(Object* object){
        return false;
    }
    
    void Explode(){
        
    }
};




//still objects that is fix to the ground
class StillObject : public Object{
public:
    StillObject(Mesh *m, vec3 position, vec3 scaling, float orientation) :
        Object(m, position, scaling, orientation){}
    
    bool Interact(Object* object) {
        return false;
        
        
    }
    
    bool checkCollision(Object* object){
        return false;
    }
    
    OBJECT_TYPE GetType(){
        return OBJECT_TYPE::StillObject;
    }
};

