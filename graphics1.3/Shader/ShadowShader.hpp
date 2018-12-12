/*
 * File: ShadowShader   .java
 * F18 CS451 Project 1
 * Names: Liwei Jiang, Heidi He
 * Date: 09/22/2018
 */

class ShadowShader : public Shader {
public:
    ShadowShader() {
        //vertex shader
        const char *vertexSource = R"(
            #version 150
            precision highp float;
        
            in vec3 vertexPosition;
            in vec2 vertexTexCoord;
            in vec3 vertexNormal;
            uniform mat4 M, VP;
            uniform vec4 worldLightPosition;
        
            void main() {
                vec4 p = vec4(vertexPosition, 1) * M;
                vec3 s;
                s.y = -0.999;
                s.x = (p.x - worldLightPosition.x) / (p.y - worldLightPosition.y) * (s.y - worldLightPosition.y) + worldLightPosition.x;
                s.z = (p.z - worldLightPosition.z) / (p.y - worldLightPosition.y) * (s.y - worldLightPosition.y) + worldLightPosition.z;
                gl_Position = vec4(s, 1) * VP;
            }
        )";
        
        //fragment shader
        const char *fragmentSource = R"(
            #version 150
            precision highp float;
        
            out vec4 fragmentColor;
        
            void main() {
                fragmentColor = vec4(0.0, 0.1, 0.0, 1);
            }
        )";
        
        unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
        if (!vertexShader) {
            printf("Error in vertex shader creation\n");
            exit(1);
        }
        glShaderSource(vertexShader, 1, &vertexSource, NULL);
        glCompileShader(vertexShader);
        
        unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        if (!fragmentShader) {
            printf("Error in fragment shader creation\n");
            exit(1);
        }
        glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
        glCompileShader(fragmentShader);
        
        shaderProgram = glCreateProgram();
        if (!shaderProgram) {
            printf("Error in shader program creation\n");
            exit(1);
        }
        
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        
        glBindAttribLocation(shaderProgram, 0, "vertexPosition");
        glBindAttribLocation(shaderProgram, 1, "vertexTexCoord");
        glBindAttribLocation(shaderProgram, 2, "vertexNormal");
        
        glBindFragDataLocation(shaderProgram, 0, "fragmentColor");
        
        glLinkProgram(shaderProgram);
    }
    
    void UploadVP(mat4& VP) {
        int location = glGetUniformLocation(shaderProgram, "VP");
        if (location >= 0) glUniformMatrix4fv(location, 1, GL_TRUE, VP);
        else printf("uniform VP cannot be set\n");
    }
    
    void UploadM(mat4& M) {
        int location = glGetUniformLocation(shaderProgram, "M");
        if (location >= 0) glUniformMatrix4fv(location, 1, GL_TRUE, M);
        else printf("uniform M cannot be set\n");
    }
    
    void UploadLightAttributes(vec3& la,vec3& le, vec4& worldLightPosition) {
        int location = glGetUniformLocation(shaderProgram, "worldLightPosition");
        if (location >= 0) glUniform4fv(location, 1, &worldLightPosition.v[0]);
        else printf("uniform ka cannot be set\n");
    }
};
