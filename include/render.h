#ifndef RENDER_H_
#define RENDER_H_

#include "matrices.h"
#include "camera.h"
#include "glad/glad.h"
#include "objmodel.h"
#include "glm/gtc/type_ptr.hpp"
#include "GLFW/glfw3.h"
#include <fstream>
#include <sstream>
#include <map>
#include "../include/stb_image.h"
#define M_PI   3.14159265358979323846

class Render{
    public:
        // Variáveis que definem um programa de GPU (shaders). Veja função LoadShadersFromFiles().
        GLuint g_GpuProgramID = 0;
        GLint g_model_uniform;
        GLint g_view_uniform;
        GLint g_projection_uniform;
        GLint g_object_id_uniform;
        GLint g_bbox_min_uniform;
        GLint g_bbox_max_uniform;
        GLuint g_NumLoadedTextures = 0;
        void DrawVirtualObject(const char* object_name); // Desenha um objeto armazenado em g_VirtualScene
        GLuint LoadShader_Vertex(const char* filename);   // Carrega um vertex shader
        GLuint LoadShader_Fragment(const char* filename); // Carrega um fragment shader
        void LoadShader(const char* filename, GLuint shader_id); // Função utilizada pelas duas acima
        GLuint CreateGpuProgram(GLuint vertex_shader_id, GLuint fragment_shader_id); // Cria um programa de GPU
        Render();
        std::vector<ObjModel> models;
        std::map<std::string, SceneObject> g_VirtualScene;
        void LoadShadersFromFiles(); // Carrega os shaders de vértice e fragmento, criando um programa de GPU
        void LoadTextureImage(const char* filename); // Função que carrega imagens de textura
        bool LoadWindow(GLFWwindow* window, Camera &camera, const float &aspectRatio, float &initialTime, float &spawnTime);
};

#endif //RENDER_H_
