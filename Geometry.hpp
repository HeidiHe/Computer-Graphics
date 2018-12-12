/*
 * File: Geometry.java
 * F18 CS451 Project 1
 * Names: Liwei Jiang, Heidi He
 * Date: 09/22/2018
 */

//geometry parent class
class Geometry {
protected:
    unsigned int vao;
    
public:
    Geometry() {
        glGenVertexArrays(1, &vao);
    }
    
    virtual void Draw() = 0;
};
