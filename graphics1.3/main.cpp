/*
 * File: Main.java
 * F18 CS451 Project 1
 * Names: Liwei Jiang, Heidi He
 * Date: 09/22/2018
 */

#include "main.hpp"
#include "mathop.hpp"
#include "Geometry.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "Light.hpp"
#include "Material.hpp"
#include "Camera.hpp"
#include "Mesh.hpp"
#include "Object.hpp"
#include "snow.hpp"

#include "Geometry/InfiniteQuadGeometry.hpp"
#include "Geometry/PolygonalMesh.hpp"
#include "Shader/MeshShader.hpp"
#include "Shader/InfiniteQuadShader.hpp"
#include "Shader/ShadowShader.hpp"

int count = 0;

/* The main scene constructed, place and draw every object. It also enables control for the heli.
   A Scene has Light, Camera, Shaders, and Objects.
   Note that Light and Camera are initited in the global scope.*/
class Scene {
    //link to shaders
    MeshShader *meshShader;
    InfiniteQuadShader *infShader;
    ShadowShader* shadowShader;
    
    //avatar is the heli that the user control
    Object* avatar;
    
    //for special condition
    GLboolean cameraWatch;
    GLboolean natureLighting;
    
    //object list
    std::vector<Texture*> textures;
    std::vector<Material*> materials;
    std::vector<Geometry*> geometries;
    std::vector<Mesh*> meshes;
    std::vector<Object*> objects;
    
public:
    Scene() {
        meshShader = 0;
        infShader = 0;
        shadowShader = 0;
        cameraWatch = false;
        natureLighting = true;
    }
    
    ~Scene() {
        for(int i = 0; i < textures.size(); i++) delete textures[i];
        for(int i = 0; i < materials.size(); i++) delete materials[i];
        for(int i = 0; i < geometries.size(); i++) delete geometries[i];
        for(int i = 0; i < meshes.size(); i++) delete meshes[i];
        for(int i = 0; i < objects.size(); i++) delete objects[i];
        
        if(meshShader) delete meshShader;
        if(infShader) delete infShader;
        if(shadowShader) delete shadowShader;
    }
    
    void Initialize() {
        //initialize snow
//        initSnow();
        
        meshShader = new MeshShader();
        infShader = new InfiniteQuadShader();
        shadowShader = new ShadowShader();
        
        
        // [0] heli body
        textures.push_back(new Texture("heli/heli_purple.jpg"));
        materials.push_back(new Material(meshShader, textures[0], vec3(0.1, 0.1, 0.1), vec3(0.9, 0.9, 0.9), vec3(0.0, 0.0, 0.0), 50));
        geometries.push_back(new PolygonalMesh("heli/heli2.obj"));
        meshes.push_back(new Mesh(geometries[0], materials[0]));
        
        // [1] heli rotor
        textures.push_back(new Texture("heli/heli_red.jpg"));
        materials.push_back(new Material(meshShader, textures[1], vec3(0.01, 0.01, 0.01), vec3(0.9, 0.9, 0.9), vec3(0.0, 0.0, 0.0), 0.0));
        geometries.push_back(new PolygonalMesh("heli/mainrotor.obj"));
        meshes.push_back(new Mesh(geometries[1], materials[1]));
        
        // [2] sky
        textures.push_back(new Texture("sky/starry_night.jpg"));
        materials.push_back(new Material(meshShader, textures[2], vec3(0.1,0.1,0.1), vec3(0.9,0.9,0.9), vec3(0.0,0.0,0.0),0.5));
        geometries.push_back(new PolygonalMesh("sky/sky.obj"));
        meshes.push_back(new Mesh(geometries[2], materials[2]));
        
        // [3] ground
        textures.push_back(new Texture("ground/ground_grass.jpg"));
        materials.push_back(new Material(infShader, textures[3], vec3(0.1,0.1,0.1), vec3(0.6,0.6,0.6), vec3(0.0, 0.0, 0.0),0.5));
        geometries.push_back(new InfiniteQuadGeometry());
        meshes.push_back(new Mesh(geometries[3], materials[3]));
        
        // [4] bridge
        textures.push_back(new Texture("bridge/bridge1.jpg"));
        materials.push_back(new Material(meshShader, textures[4], vec3(0.1,0.1,0.1), vec3(0.6,0.6,0.6), vec3(0.1,0.1,0.1),0.5));
        geometries.push_back(new PolygonalMesh("bridge/bridge.obj"));
        meshes.push_back(new Mesh(geometries[4], materials[4]));
        
        // [5] rock1
        textures.push_back(new Texture("rock/rock1.png"));
        materials.push_back(new Material(meshShader, textures[5], vec3(0.1,0.1,0.1), vec3(0.9,0.9,0.9), vec3(0.0,0.0,0.0),0.5));
        geometries.push_back(new PolygonalMesh("rock/rock1.obj"));
        meshes.push_back(new Mesh(geometries[5], materials[5]));
        
        // [6] mars1
        textures.push_back(new Texture("mars/mars1.png"));
        materials.push_back(new Material(meshShader, textures[6], vec3(0.1,0.1,0.1), vec3(0.9,0.9,0.9), vec3(0.0,0.0,0.0),0.5));
        geometries.push_back(new PolygonalMesh("mars/mars.obj"));
        meshes.push_back(new Mesh(geometries[6], materials[6]));
        
        // [7] rock2
        textures.push_back(new Texture("rock/rock2.png"));
        materials.push_back(new Material(meshShader, textures[7], vec3(0.1,0.1,0.1), vec3(0.9,0.9,0.9), vec3(0.0,0.0,0.0),0.5));
        geometries.push_back(new PolygonalMesh("rock/rock2.obj"));
        meshes.push_back(new Mesh(geometries[7], materials[7]));
        
        
        // push objects into the list
        // object configuration: vec3(position), vec3(scaling), float(orientation)
        avatar = new Avatar(meshes[0], vec3(0.0, -0.6, 0.0), vec3(0.05, 0.05, 0.05), 0);
        objects.push_back(avatar);                                                                                  // [0] heli body
        objects.push_back(new Avatar(meshes[1], vec3(0.0, 1.5, 0.0), vec3(0.05, 0.05, 0.05), 0));                   // [1] heli rotor
        objects.push_back(new StillObject(meshes[2], vec3(128, -1, 0), vec3(1.515, 1.515, 1.515), -60));                 // [2] sky
        objects.push_back(new StillObject(meshes[3], vec3(0, -1, 0), vec3(1, 1, 1), -60));                               // [3] ground
        objects.push_back(new StillObject(meshes[4], vec3(0.0, -1.0, -8), vec3(0.2, 0.2, 0.2), -60.0));                 // [4] bridge
        //add random rocks
        for (int i=0; i<20; i++){                                                                                   // [5] rocks1
            int value1 = rand() % 50;
            int value2 = rand() % 50;
            int value3 = rand() % 10;
            objects.push_back(new StillObject(meshes[5], vec3(value1, -1.0, value2), vec3(0.001*value3, 0.001*value3, 0.001*value3), 3*value2));
        }
        
        //add random mars
        for (int i=0; i<20; i++){                                                                                   // [6] mars
            int value1 = rand() % 40;
            int value2 = rand() % 40;
            int value3 = rand() % 40;
            Object* curObjet = new Ball(meshes[6], vec3(value1, value3, value2), vec3(0.01*value3, 0.01*value3, 0.01*value3), 3*value2);
            curObjet->setGravity(true);
            objects.push_back(curObjet);
//            objects.push_back(new Object(meshes[6], vec3(value1, 5.0, value2), vec3(0.01*value3, 0.01*value3, 0.01*value3), 3*value2));
            
        }
        //add random rocks v2
        for (int i=0; i<20; i++){                                                                                   // [5] rocks1
            int value1 = rand() % 50;
            int value2 = rand() % 50;
            int value3 = rand() % 10;
            objects.push_back(new StillObject(meshes[7], vec3(value1, -1.0, value2), vec3(0.001*value3, 0.001*value3, 0.001*value3), 3*value2));
        }
    }
    
    
    //draw objects
    void Draw() {
        //detect light specified
        if(natureLighting){
            light = natureLight;
        }
        else{
            light = spotlight;
            vec3 eye = avatar->GetPosition();
            light.SetPointLightSource(eye);
        }
        
        //draw ground
        objects[2]->Draw();
        objects[3]->Draw();
        
        //if in spotlight environment, attach the light to the heli
        vec3 spot = avatar->GetPosition();
        spotlight.SetPointLightSource(spot);
        vec3 avatarPosition = avatar->GetPosition();
        vec3 rotorPosition = vec3(avatarPosition.x, avatarPosition.y+1.5, avatarPosition.z);
        avatar->setGravity(true);
//        avatar->setVelocity(vec3(0,0, -0.1));
        
        //draw every object
        for(int i = 0; i < objects.size(); i++){
            // heli rotor animated
            if(i == 1){
                objects[i]->SetPosition(rotorPosition);
                objects[i]->Rotate();
            }
            if(objects[i]->getVisible()){
               objects[i]->Draw();
                
                if (!(i == 2 || i == 3)){
                    objects[i]->DrawShadow(shadowShader);
                }
            }
  
        }
    }
    
    
    // a series of keyboard control for the heli and the environment
    void Move(float dt) {
        bool stop = false;
        dt *= 5; // speed up
        vec3 curPosistion = avatar->GetPosition();
        float newO = avatar->GetOrientation();
        float av = 0;
        
        for(int i = 0; i < objects.size(); i++){
            objects[i]->Move(dt);
        }
        
        /*check intersection*/
        for(int i = 0; i < objects.size(); i++){
            /*not sky or ground*/
            if (i>3){
                if(avatar->checkCollision(objects[i])){
                    stop = true;
                }

            }
            
        }
    
        //forward
        if( (stop==false) && (keyboardState['i'])){
            newO =((newO-90)/180)*M_PI;
            curPosistion.x = curPosistion.x - dt*cos(newO);
            curPosistion.z = curPosistion.z - dt*sin(newO);
            avatar->SetPosition(curPosistion);
            if(cameraWatch){
                camera.updateEye(3);
            }
        }
        //backward
        if(keyboardState['k']){
            newO =((newO-90)/180)*M_PI;
            curPosistion.x += dt*cos(newO);
            curPosistion.z += dt*sin(newO);
            avatar->SetPosition(curPosistion);
            if(cameraWatch){
                camera.updateEye(3);
            }
        }
        
        //turn left
        if(keyboardState['j']){
            avatar->SetOrientation(newO+1);
            float newRoll = avatar->GetRoll()-0.1;
            if (newRoll > -10){
                objects[0]->SetRoll(newRoll);
            }
            av = -1*dt;
            if(cameraWatch){
                camera.Rotate(av);
            }
        }
        //turn right
        else if(keyboardState['l']){
            avatar->SetOrientation(newO-1);
            float newRoll = avatar->GetRoll()+0.1;
            if (newRoll < 10){
                objects[0]->SetRoll(newRoll);
            }
            av = 1*dt;
            if(cameraWatch){
                camera.Rotate(av);
            }
        }
        else{
            avatar->SetRoll(0);
            objects[1]->SetRoll(0);
        }

        
        //camera watch
        if(keyboardState['c']){
            cameraWatch = true;
            camera.setEyePosition(vec3(curPosistion.x+3, 0, curPosistion.z));
        }
        //return to normal camera view
        if(keyboardState['v']){
            cameraWatch = false;
            vec3 newPos = vec3(curPosistion.x, 0, curPosistion.z+2);
            camera.setCamera(newPos, vec3(curPosistion.x, 0, curPosistion.z));
        }
        // in camera watch mode
        if(cameraWatch){
            camera.setLookat(curPosistion);
        }
        //switch light
        if(keyboardState['b']){
            SwitchLight();
        }
        
    }
    
    void SwitchLight(){
        natureLighting =! natureLighting;
    }
    
    GLboolean GetCameraWatch(){
        return cameraWatch;
    }
};

Scene scene;

//initialize the scene
void onInitialization() {
    glViewport(0, 0, windowWidth, windowHeight);
    scene.Initialize();
}

//when exit
void onExit() {
    printf("exit");
}

//displaying the scene
void onDisplay() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    scene.Draw();
    glutSwapBuffers();
}

//detect keyboard input
void onKeyboard(unsigned char key, int x, int y) {
    keyboardState[key] = true;
}
void onKeyboardUp(unsigned char key, int x, int y) {
    keyboardState[key] = false;
}

//resize the window
void onReshape(int w, int h) {
    camera.SetAspectRatio((float)w / h);
//    glViewport(0, 0, winWidth, winHeight);
    if (h == 0) h = 1;
    
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    gluPerspective(45, (float) w / (float) h, .1, 200);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

//get elapsed time
void onIdle( ) {
    double t = glutGet(GLUT_ELAPSED_TIME) * 0.001; //Number of milliseconds since glutInit called
    static double lastTime = 0.0;
    double dt = t - lastTime;
    lastTime = t;
    
    camera.Control();
    if(scene.GetCameraWatch()==false){
        camera.Move(dt);
    }
    scene.Move(dt);
    glutPostRedisplay();
}

float xRotated = 90.0, yRotated = 0.0, zRotated = 0.0;

//------------------------------  reshapeFunc  ---------------------------------

void reshapeFunc (int x, int y)
{
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity();
    
    gluPerspective (40.0, (GLdouble)x / (GLdouble)y, 0.5, 20.0);
    glMatrixMode   (GL_MODELVIEW);
    glViewport     (0, 0, x, y);
}

//------------------------------  Draw_Spheres   -------------------------------

void Draw_Spheres (void)
{
    glMatrixMode   (GL_MODELVIEW);
    glClear        (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity ();
    glTranslatef    (0.0, 0.0, -15.0);
    
    glColor3f (0.8, 0.2, 0.1);              // Red ball displaced to left.
    glPushMatrix ();
    glTranslatef    (-1.5, 0.0, 0.0);
    glRotatef       (60.0, 1,0,0);
    glRotatef       (zRotated*2.0, 0,0,1);   // Red ball rotates at twice the rate of blue ball.
    glutSolidSphere (1.0, 20, 50);
    glPopMatrix ();
    
    glColor3f (0.1, 0.2, 0.8);              // Blue ball displaced to right.
    glPushMatrix ();
    glTranslatef    (1.5, 0.0, 0.0);
    glRotatef       (60.0, 1,0,0);
    glRotatef       (zRotated, 0,0,1);
    glutSolidSphere (1.0, 20, 50);
    glPopMatrix ();
    
    glutSwapBuffers();
}

//--------------------------------  idleFunc  ----------------------------------

void idleFunc (void)
{
    zRotated += 0.3;
    glutPostRedisplay();
}

int main(int argc, char * argv[]) {
    glutInit(&argc, argv);
    glutInitWindowSize(windowWidth, windowHeight);
//    glutInitWindowPosition(50, 50);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_3_2_CORE_PROFILE );
    glutCreateWindow("Hey Game");
    glGetIntegerv(GL_MAJOR_VERSION, &majorVersion);
    glGetIntegerv(GL_MINOR_VERSION, &minorVersion);
    
    onInitialization();
    
    glutDisplayFunc(onDisplay);
    glutIdleFunc(onIdle);
    glutKeyboardFunc(onKeyboard);
    glutKeyboardUpFunc(onKeyboardUp);
    glutReshapeFunc(onReshape);
    

    
    glutMainLoop();
    onExit();
    return 1;
}
