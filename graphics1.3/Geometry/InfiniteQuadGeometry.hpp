/*
 * File: InfiniteQuadGeometry.java
 * F18 CS451 Project 1
 * Names: Liwei Jiang, Heidi He
 * Date: 09/22/2018
 */

/*
 * inifnite ground
 */
class InfiniteQuadGeometry : public Geometry {
    unsigned int vbo[3];
    
public:
    InfiniteQuadGeometry() {
        glBindVertexArray(vao);
        
        glGenBuffers(3, &vbo[0]);
        
        //Vertex Position Array
        glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
        static float vertexCoords[] = {
             0, 0,  0, 1,
            -1, 0, -1, 0,
            -1, 0,  1, 0,
             1, 0,  1, 0,
             1, 0, -1, 0,
            -1, 0, -1, 0
        };
        
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertexCoords), vertexCoords, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);
        
        //Texture Coordinates Array
        glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
        static float textCoords[] = {
            0, 0,
            1, 0,
            0, 1,
            1, 1 };
        
        glBufferData(GL_ARRAY_BUFFER, sizeof(textCoords), textCoords, GL_STATIC_DRAW);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);
        
        
        //Normal Array
        glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
        static float normalCoords[] = {
            0, 1, 0,
            0, 1, 0,
            0, 1, 0,
            0, 1, 0
        };    // vertex data on the CPU
        
        glBufferData(GL_ARRAY_BUFFER,    // copy to the GPU
                     sizeof(normalCoords),    // size of the vbo in bytes
                     normalCoords,        // address of the data array on the CPU
                     GL_STATIC_DRAW);    // copy to that part of the memory which is not modified
        
        // map Attribute Array 0 to the currently bound vertex buffer (vbo)
        glEnableVertexAttribArray(2);
        
        // data organization of Attribute Array 2
        glVertexAttribPointer(2,    // Attribute Array 2
                              3, GL_FLOAT,        // components/attribute, component type
                              GL_FALSE,        // not in fixed point format, do not normalized
                              0, NULL);        // stride and offset: it is tightly packed
    }
    
    void Draw() {
        glEnable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 6);
        glDisable(GL_BLEND);
        glDisable(GL_DEPTH_TEST);
    }
};
