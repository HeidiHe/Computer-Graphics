/*
 * File: Texture.java
 * F18 CS451 Project 1
 * Names: Liwei Jiang, Heidi He
 * Date: 09/22/2018
 */

extern "C" unsigned char* stbi_load(char const *filename, int *x, int *y, int *comp, int req_comp);

//texture object takes in an output image file for the material construction
class Texture {
    unsigned int textureId;
    
public:
    Texture(const std::string& inputFileName) {
        unsigned char* data;
        int width; int height; int nComponents = 4;
        
        data = stbi_load(inputFileName.c_str(), &width, &height, &nComponents, 0);
        
        if(data == NULL) {
            return;
        }
        
        glGenTextures(1, &textureId);
        glBindTexture(GL_TEXTURE_2D, textureId);
        
        if(nComponents == 3) glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        if(nComponents == 4) glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        
        delete data;
    }
    
    void Bind() {
        glBindTexture(GL_TEXTURE_2D, textureId);
    }
};
