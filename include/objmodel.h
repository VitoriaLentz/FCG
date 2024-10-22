#ifndef OBJMODEL_H_
#define OBJMODEL_H_

#include "sceneobject.h"
#include "camera.h"
#include "tiny_obj_loader.h"
#include <string>
#include "glm/vec4.hpp"

class Obj {
    public:
        tinyobj::attrib_t                 attrib;
        std::vector<tinyobj::shape_t>     shapes;
        std::vector<tinyobj::material_t>  materials;
        Obj(const char* filename, const char* basepath = NULL, bool triangulate = true);
        Obj();
};

class ObjModel {
    public:
        glm::vec3 scale;
        std::string name;
        int objectId;
        glm::vec3 position;
        glm::vec3 direction;
        float rotation;
        glm::vec3 originalPosition;
        Obj obj;
        //FONTE: laboratórios da disciplina
        void BuildTrianglesAndAddToVirtualScene(std::map<std::string, SceneObject> &virtualScene); // Constrói representação de um ObjModel como malha de triângulos para renderização
        void ComputeNormals(); // Computa normais de um ObjModel, caso não existam.
        ObjModel(int id, glm::vec3 position, glm::vec3 scale, glm::vec3 direction, float rotation, const char* name, const char* path, std::map<std::string, SceneObject> &virtualScene);
        glm::vec3 bbox_min;
        glm::vec3 bbox_max;

};


#endif //OBJMODEL_H_
