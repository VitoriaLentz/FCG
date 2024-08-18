#ifndef SCENEOBJECT_H_
#define SCENEOBJECT_H_

#include <string>
#include "glad/glad.h"
#include "glm/vec3.hpp"

class SceneObject{
    public:
        std::string  name;
        size_t       first_index;
        size_t       num_indices;
        GLenum       rendering_mode;
        GLuint       vertex_array_object_id;
        glm::vec3    bbox_min;
        glm::vec3    bbox_max;
        //FONTE: laborat�rios da disciplina
        SceneObject(std::string name, size_t first_index, size_t num_indices, GLenum rendering_mode, GLuint vertex_array_object_id, glm::vec3 bbox_min, glm::vec3 bbox_max);
        SceneObject();
};

#endif //SCENEOBJECT_H_
