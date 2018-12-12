/*
 * File: InfiniteQuadShader.hpp
 * F18 CS451 Project 1
 * Names: Liwei Jiang, Heidi He
 * Date: 09/22/2018
 */

class InfiniteQuadShader : public Shader {
public:
    InfiniteQuadShader() {
        const char *vertexSource = R"(
            #version 150
            precision highp float;
        
            in vec4 vertexPosition;
            in vec2 vertexTexCoord;
            in vec3 vertexNormal;
            uniform mat4 M, InvM, MVP;
        
            out vec2 texCoord;
            out vec4 worldPosition;
            out vec3 worldNormal;
        
            void main() {
                texCoord = vertexTexCoord;
                worldPosition = vertexPosition * M;
                worldNormal = (InvM * vec4(vertexNormal, 0.0)).xyz;
                gl_Position = vertexPosition * MVP;
            }
        )";
        
        const char *fragmentSource = R"(
            #version  150
            precision highp float;
            uniform sampler2D samplerUnit;
            uniform vec3 La, Le;
            uniform vec3 ka, kd, ks;
            uniform float shininess;
            uniform vec3 worldEyePosition;
            uniform vec4 worldLightPosition;
            in vec2 texCoord;
            in vec4 worldPosition;
            in vec3 worldNormal;
            out vec4 fragmentColor;
            void main() {
                vec3 N = normalize(worldNormal);
                vec3 V = normalize(worldEyePosition * worldPosition.w - worldPosition.xyz);
                vec3 L = normalize(worldLightPosition.xyz * worldPosition.w - worldPosition.xyz * worldLightPosition.w);
                vec3 H = normalize(V + L);
                vec2 position = worldPosition.xz / worldPosition.w;
                vec2 tex = position.xy - floor(position.xy);
                vec3 texel = texture(samplerUnit, tex).xyz;
                vec3 color = La * ka + Le * kd * texel * max(0.0, dot(L, N)) + Le * ks * pow(max(0.0, dot(H, N)), shininess);
                fragmentColor = vec4(color, 1);
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
    
    void UploadSamplerID() {
        int samplerUnit = 0;
        int location = glGetUniformLocation(shaderProgram, "samplerUnit");
        glUniform1i(location, samplerUnit);
        glActiveTexture(GL_TEXTURE0 + samplerUnit);
    }
    
    void UploadInvM(mat4& InvM) {
        int location = glGetUniformLocation(shaderProgram, "InvM");
        if (location >= 0) glUniformMatrix4fv(location, 1, GL_TRUE, InvM);
        else printf("uniform InvM cannot be set\n");
    }
    
    void UploadMVP(mat4& MVP) {
        int location = glGetUniformLocation(shaderProgram, "MVP");
        if (location >= 0) glUniformMatrix4fv(location, 1, GL_TRUE, MVP);
        else printf("uniform MVP cannot be set\n");
    }
    
    void UploadM(mat4& M) {
        int location = glGetUniformLocation(shaderProgram, "M");
        if (location >= 0) glUniformMatrix4fv(location, 1, GL_TRUE, M);
        else printf("uniform M cannot be set\n");
    }
    
    void UploadMaterialAttributes(vec3& ka,vec3& kd, vec3& ks, float shininess){
        int location = glGetUniformLocation(shaderProgram, "ka");
        if (location >= 0) glUniform3fv(location, 1, &ka.x);
        else printf("uniform ka cannot be set\n");
        
        location = glGetUniformLocation(shaderProgram, "kd");
        if (location >= 0) glUniform3fv(location, 1, &kd.x);
        else printf("uniform kd cannot be set\n");
        
        location = glGetUniformLocation(shaderProgram, "ks");
        if (location >= 0) glUniform3fv(location, 1, &ks.x);
        else printf("uniform ks cannot be set\n");
        
        location = glGetUniformLocation(shaderProgram, "shininess");
        if (location >= 0) glUniform1f(location, shininess);
        else printf("uniform shininess cannot be set\n");
        
    }
    
    void UploadLightAttributes(vec3& la,vec3& le, vec4& worldLightPosition) {
        int location = glGetUniformLocation(shaderProgram, "La");
        if (location >= 0) glUniform3fv(location, 1, &la.x);
        else printf("uniform la cannot be set\n");
        
        location = glGetUniformLocation(shaderProgram, "Le");
        if (location >= 0) glUniform3fv(location, 1, &le.x);
        else printf("uniform le cannot be set\n");
        
        location = glGetUniformLocation(shaderProgram, "worldLightPosition");
        if (location >= 0) glUniform4fv(location, 1, &worldLightPosition.v[0]);
        else printf("uniform ka cannot be set\n");
    }
    
    void UploadEyePosition(vec3& wEye) {
        int location = glGetUniformLocation(shaderProgram, "worldEyePosition");
        if (location >= 0) glUniform3fv(location, 1, &wEye.x);
        else printf("uniform wEye cannot be set\n");
    }
};
