/*
 * File: PolygonalMesh.java
 * F18 CS451 Project 1
 * Names: Liwei Jiang, Heidi He
 * Date: 09/22/2018
 */


//build mesh geometry from obj file
class   PolygonalMesh : public Geometry {
    struct  Face {
        int       positionIndices[4];
        int       normalIndices[4];
        int       texcoordIndices[4];
        bool      isQuad;
    };
    
    std::vector<std::string*> rows;
    std::vector<vec3*> positions;
    std::vector<std::vector<Face*>> submeshFaces;
    std::vector<vec3*> normals;
    std::vector<vec2*> texcoords;
    
    int nTriangles;
    
public:
    PolygonalMesh(const char *filename);
    ~PolygonalMesh();
    
    void Draw();
};


PolygonalMesh::PolygonalMesh(const char *filename) {
    std::fstream file(filename);
    if(!file.is_open()) {
        return;
    }
    
    char buffer[256];
    while(!file.eof()) {
        file.getline(buffer,256);
        rows.push_back(new std::string(buffer));
    }
    
    submeshFaces.push_back(std::vector<Face*>());
    std::vector<Face*>* faces = &submeshFaces.at(submeshFaces.size()-1);
    
    //parse in the obj file
    for(int i = 0; i < rows.size(); i++) {
        if(rows[i]->empty() || (*rows[i])[0] == '#')
            continue;
        else if((*rows[i])[0] == 'v' && (*rows[i])[1] == ' ') {
            float tmpx,tmpy,tmpz;
            sscanf(rows[i]->c_str(), "v %f %f %f" ,&tmpx,&tmpy,&tmpz);
            positions.push_back(new vec3(tmpx,tmpy,tmpz));
        }
        else if((*rows[i])[0] == 'v' && (*rows[i])[1] == 'n') {
            float tmpx,tmpy,tmpz;
            sscanf(rows[i]->c_str(), "vn %f %f %f" ,&tmpx,&tmpy,&tmpz);
            normals.push_back(new vec3(tmpx,tmpy,tmpz));
        }
        else if((*rows[i])[0] == 'v' && (*rows[i])[1] == 't') {
            float tmpx,tmpy;
            sscanf(rows[i]->c_str(), "vt %f %f" ,&tmpx,&tmpy);
//            printf("%f %f\n", tmpx, tmpy);
            texcoords.push_back(new vec2(tmpx,tmpy));
        }
        else if((*rows[i])[0] == 'f') {
            if(count(rows[i]->begin(), rows[i]->end(), ' ') == 3) {
                Face* f = new Face();
                f->isQuad = false;
                sscanf(rows[i]->c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d",
                       &f->positionIndices[0], &f->texcoordIndices[0], &f->normalIndices[0],
                       &f->positionIndices[1], &f->texcoordIndices[1], &f->normalIndices[1],
                       &f->positionIndices[2], &f->texcoordIndices[2], &f->normalIndices[2]);
                faces->push_back(f);
            }
            else {
                Face* f = new Face();
                f->isQuad = true;
                sscanf(rows[i]->c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d",
                       &f->positionIndices[0], &f->texcoordIndices[0], &f->normalIndices[0],
                       &f->positionIndices[1], &f->texcoordIndices[1], &f->normalIndices[1],
                       &f->positionIndices[2], &f->texcoordIndices[2], &f->normalIndices[2],
                       &f->positionIndices[3], &f->texcoordIndices[3], &f->normalIndices[3]);
                faces->push_back(f);
            }
        }
        else if((*rows[i])[0] == 'g') {
            if(faces->size() > 0) {
                submeshFaces.push_back(std::vector<Face*>());
                faces = &submeshFaces.at(submeshFaces.size()-1);
            }
        }
    }
    


    //get in faces
    int numberOfTriangles = 0;
    for(int iSubmesh=0; iSubmesh<submeshFaces.size(); iSubmesh++) {
        std::vector<Face*>& faces = submeshFaces.at(iSubmesh);
        
        for(int i=0;i<faces.size();i++) {
            if(faces[i]->isQuad) {
                numberOfTriangles += 2;
            }
            else {
                    numberOfTriangles += 1;
            }
        }
    }
    
    nTriangles = numberOfTriangles;
    
    float *vertexCoords = new float[numberOfTriangles * 9];
    float *vertexTexCoords = new float[numberOfTriangles * 6];
    float *vertexNormalCoords = new float[numberOfTriangles * 9];
    
    int triangleIndex = 0;
    for(int iSubmesh=0; iSubmesh<submeshFaces.size(); iSubmesh++) {
        std::vector<Face*>& faces = submeshFaces.at(iSubmesh);
        
        for(int i=0;i<faces.size();i++) {
            if(faces[i]->isQuad) {
                //pass in texture coordinate
                vertexTexCoords[triangleIndex * 6 + 0] = texcoords[faces[i]->texcoordIndices[0]-1]->x;
                vertexTexCoords[triangleIndex * 6 + 1] = 1-texcoords[faces[i]->texcoordIndices[0]-1]->y;
                
                vertexTexCoords[triangleIndex * 6 + 2] = texcoords[faces[i]->texcoordIndices[1]-1]->x;
                vertexTexCoords[triangleIndex * 6 + 3] = 1-texcoords[faces[i]->texcoordIndices[1]-1]->y;
                
                vertexTexCoords[triangleIndex * 6 + 4] = texcoords[faces[i]->texcoordIndices[2]-1]->x;
                vertexTexCoords[triangleIndex * 6 + 5] = 1-texcoords[faces[i]->texcoordIndices[2]-1]->y;
                
                //pass in vertex coordinate
                vertexCoords[triangleIndex * 9 + 0] = positions[faces[i]->positionIndices[0]-1]->x;
                vertexCoords[triangleIndex * 9 + 1] = positions[faces[i]->positionIndices[0]-1]->y;
                vertexCoords[triangleIndex * 9 + 2] = positions[faces[i]->positionIndices[0]-1]->z;
                
                vertexCoords[triangleIndex * 9 + 3] = positions[faces[i]->positionIndices[1]-1]->x;
                vertexCoords[triangleIndex * 9 + 4] = positions[faces[i]->positionIndices[1]-1]->y;
                vertexCoords[triangleIndex * 9 + 5] = positions[faces[i]->positionIndices[1]-1]->z;
                
                vertexCoords[triangleIndex * 9 + 6] = positions[faces[i]->positionIndices[2]-1]->x;
                vertexCoords[triangleIndex * 9 + 7] = positions[faces[i]->positionIndices[2]-1]->y;
                vertexCoords[triangleIndex * 9 + 8] = positions[faces[i]->positionIndices[2]-1]->z;
                
                //pass in vertex normal coordinate
                vertexNormalCoords[triangleIndex * 9 + 0] = normals[faces[i]->normalIndices[0]-1]->x;
                vertexNormalCoords[triangleIndex * 9 + 1] = normals[faces[i]->normalIndices[0]-1]->y;
                vertexNormalCoords[triangleIndex * 9 + 2] = normals[faces[i]->normalIndices[0]-1]->z;
                
                vertexNormalCoords[triangleIndex * 9 + 3] = normals[faces[i]->normalIndices[1]-1]->x;
                vertexNormalCoords[triangleIndex * 9 + 4] = normals[faces[i]->normalIndices[1]-1]->y;
                vertexNormalCoords[triangleIndex * 9 + 5] = normals[faces[i]->normalIndices[1]-1]->z;
                
                vertexNormalCoords[triangleIndex * 9 + 6] = normals[faces[i]->normalIndices[2]-1]->x;
                vertexNormalCoords[triangleIndex * 9 + 7] = normals[faces[i]->normalIndices[2]-1]->y;
                vertexNormalCoords[triangleIndex * 9 + 8] = normals[faces[i]->normalIndices[2]-1]->z;
                
                triangleIndex++; // divided the quad into 2 triangles
                
                //pass in texture coordinate
                vertexTexCoords[triangleIndex * 6 + 0] = texcoords[faces[i]->texcoordIndices[0]-1]->x;
                vertexTexCoords[triangleIndex * 6 + 1] = 1-texcoords[faces[i]->texcoordIndices[0]-1]->y;

                vertexTexCoords[triangleIndex * 6 + 2] = texcoords[faces[i]->texcoordIndices[2]-1]->x;
                vertexTexCoords[triangleIndex * 6 + 3] = 1-texcoords[faces[i]->texcoordIndices[2]-1]->y;

                vertexTexCoords[triangleIndex * 6 + 4] = texcoords[faces[i]->texcoordIndices[3]-1]->x;
                vertexTexCoords[triangleIndex * 6 + 5] = 1-texcoords[faces[i]->texcoordIndices[3]-1]->y;

                //pass in vertex coordinate
                vertexCoords[triangleIndex * 9 + 0] = positions[faces[i]->positionIndices[0]-1]->x;
                vertexCoords[triangleIndex * 9 + 1] = positions[faces[i]->positionIndices[0]-1]->y;
                vertexCoords[triangleIndex * 9 + 2] = positions[faces[i]->positionIndices[0]-1]->z;

                vertexCoords[triangleIndex * 9 + 3] = positions[faces[i]->positionIndices[2]-1]->x;
                vertexCoords[triangleIndex * 9 + 4] = positions[faces[i]->positionIndices[2]-1]->y;
                vertexCoords[triangleIndex * 9 + 5] = positions[faces[i]->positionIndices[2]-1]->z;

                vertexCoords[triangleIndex * 9 + 6] = positions[faces[i]->positionIndices[3]-1]->x;
                vertexCoords[triangleIndex * 9 + 7] = positions[faces[i]->positionIndices[3]-1]->y;
                vertexCoords[triangleIndex * 9 + 8] = positions[faces[i]->positionIndices[3]-1]->z;

                //pass in vertex normal coordinate
                vertexNormalCoords[triangleIndex * 9 + 0] = normals[faces[i]->normalIndices[0]-1]->x;
                vertexNormalCoords[triangleIndex * 9 + 1] = normals[faces[i]->normalIndices[0]-1]->y;
                vertexNormalCoords[triangleIndex * 9 + 2] = normals[faces[i]->normalIndices[0]-1]->z;

                vertexNormalCoords[triangleIndex * 9 + 3] = normals[faces[i]->normalIndices[2]-1]->x;
                vertexNormalCoords[triangleIndex * 9 + 4] = normals[faces[i]->normalIndices[2]-1]->y;
                vertexNormalCoords[triangleIndex * 9 + 5] = normals[faces[i]->normalIndices[2]-1]->z;

                vertexNormalCoords[triangleIndex * 9 + 6] = normals[faces[i]->normalIndices[3]-1]->x;
                vertexNormalCoords[triangleIndex * 9 + 7] = normals[faces[i]->normalIndices[3]-1]->y;
                vertexNormalCoords[triangleIndex * 9 + 8] = normals[faces[i]->normalIndices[3]-1]->z;

                triangleIndex++;
            }
            else {
                //pass in texture coordinate
                vertexTexCoords[triangleIndex * 6 + 0] = texcoords[faces[i]->texcoordIndices[0]-1]->x;
                vertexTexCoords[triangleIndex * 6 + 1] = 1-texcoords[faces[i]->texcoordIndices[0]-1]->y;
                
                vertexTexCoords[triangleIndex * 6 + 2] = texcoords[faces[i]->texcoordIndices[1]-1]->x;
                vertexTexCoords[triangleIndex * 6 + 3] = 1-texcoords[faces[i]->texcoordIndices[1]-1]->y;
                
                vertexTexCoords[triangleIndex * 6 + 4] = texcoords[faces[i]->texcoordIndices[2]-1]->x;
                vertexTexCoords[triangleIndex * 6 + 5] = 1-texcoords[faces[i]->texcoordIndices[2]-1]->y;
                
                //pass in vertex coordinate
                vertexCoords[triangleIndex * 9 + 0] = positions[faces[i]->positionIndices[0]-1]->x;
                vertexCoords[triangleIndex * 9 + 1] = positions[faces[i]->positionIndices[0]-1]->y;
                vertexCoords[triangleIndex * 9 + 2] = positions[faces[i]->positionIndices[0]-1]->z;
                
                vertexCoords[triangleIndex * 9 + 3] = positions[faces[i]->positionIndices[1]-1]->x;
                vertexCoords[triangleIndex * 9 + 4] = positions[faces[i]->positionIndices[1]-1]->y;
                vertexCoords[triangleIndex * 9 + 5] = positions[faces[i]->positionIndices[1]-1]->z;
                
                vertexCoords[triangleIndex * 9 + 6] = positions[faces[i]->positionIndices[2]-1]->x;
                vertexCoords[triangleIndex * 9 + 7] = positions[faces[i]->positionIndices[2]-1]->y;
                vertexCoords[triangleIndex * 9 + 8] = positions[faces[i]->positionIndices[2]-1]->z;
                
                //pass in vertex normal coordinate
                vertexNormalCoords[triangleIndex * 9 + 0] = normals[faces[i]->normalIndices[0]-1]->x;
                vertexNormalCoords[triangleIndex * 9 + 1] = normals[faces[i]->normalIndices[0]-1]->y;
                vertexNormalCoords[triangleIndex * 9 + 2] = normals[faces[i]->normalIndices[0]-1]->z;
                
                vertexNormalCoords[triangleIndex * 9 + 3] = normals[faces[i]->normalIndices[1]-1]->x;
                vertexNormalCoords[triangleIndex * 9 + 4] = normals[faces[i]->normalIndices[1]-1]->y;
                vertexNormalCoords[triangleIndex * 9 + 5] = normals[faces[i]->normalIndices[1]-1]->z;
                
                vertexNormalCoords[triangleIndex * 9 + 6] = normals[faces[i]->normalIndices[2]-1]->x;
                vertexNormalCoords[triangleIndex * 9 + 7] = normals[faces[i]->normalIndices[2]-1]->y;
                vertexNormalCoords[triangleIndex * 9 + 8] = normals[faces[i]->normalIndices[2]-1]->z;
                
                triangleIndex++;
            }
        }
    }
    
    //vertex array: VAO
    glBindVertexArray(vao);
    
    //vertex buffer: VBO
    unsigned int vbo[3];
    glGenBuffers(3, &vbo[0]);
    
    //Vertex Position Array
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, nTriangles * 9 * sizeof(float), vertexCoords, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    
     //Texture Coordinates Array
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, nTriangles * 6 * sizeof(float), vertexTexCoords, GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    
    //normal array
    glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
    glBufferData(GL_ARRAY_BUFFER, nTriangles * 9 * sizeof(float), vertexNormalCoords, GL_STATIC_DRAW);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    
    delete vertexCoords;
    delete vertexTexCoords;
    delete vertexNormalCoords;
}


void PolygonalMesh::Draw() {
    glEnable(GL_DEPTH_TEST);
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, nTriangles * 3);
    glDisable(GL_DEPTH_TEST);
}

PolygonalMesh::~PolygonalMesh() {
    for(unsigned int i = 0; i < rows.size(); i++) delete rows[i];
    for(unsigned int i = 0; i < positions.size(); i++) delete positions[i];
    for(unsigned int i = 0; i < submeshFaces.size(); i++)
        for(unsigned int j = 0; j < submeshFaces.at(i).size(); j++)
            delete submeshFaces.at(i).at(j);
    for(unsigned int i = 0; i < normals.size(); i++) delete normals[i];
    for(unsigned int i = 0; i < texcoords.size(); i++) delete texcoords[i];
}
