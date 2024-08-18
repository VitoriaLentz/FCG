#include <cmath>
#include <cstdio>
#include <cstdlib>
#include "../include/render.h"
#include "../include/camera.h"
#include "../include/glad/glad.h"
#include "../include/GLFW/glfw3.h"

#define COZINHA 0
#define CHEF    2
#define KNIFE   3
#define BANANA  4
#define MACA    5
#define ABACAXI 6
#define LARANJA 7

int _height = 800;
int _width = 800;
Camera camera = Camera();
Render render = Render();
// Variáveis globais que armazenam a última posição do cursor do mouse, para
// que possamos calcular quanto que o mouse se movimentou entre dois instantes
// de tempo. Utilizadas no callback CursorPosCallback() abaixo.
double g_LastCursorPosX = 0, g_LastCursorPosY = 0;

glm::vec4 normalize (glm::vec4 v) {
    return v / norm(v);
}

// Definimos o callback para impressão de erros da GLFW no terminal
void ErrorCallback(int error, const char* description)
{
    fprintf(stderr, "ERROR: GLFW: %s\n", description);
}

// Definição da função que será chamada sempre que a janela do sistema
// operacional for redimensionada, por consequência alterando o tamanho do
// "framebuffer" (região de memória onde são armazenados os pixels da imagem).
void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    // Indicamos que queremos renderizar em toda região do framebuffer. A
    // função "glViewport" define o mapeamento das "normalized device
    // coordinates" (NDC) para "pixel coordinates".  Essa é a operação de
    // "Screen Mapping" ou "Viewport Mapping" vista em aula ({+ViewportMapping2+}).
    glViewport(0, 0, width, height);
    _height = height;
    _width = width;
}

// Função callback chamada sempre que o usuário aperta algum dos botões do mouse
void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        camera.ataque = true;
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
        camera.ataque = false;
}

// Função callback chamada sempre que o usuário movimentar o cursor do mouse em
// cima da janela OpenGL.
void CursorPosCallback(GLFWwindow* window, double xpos, double ypos)
{
    float dx = xpos - g_LastCursorPosX;
    float dy = ypos - g_LastCursorPosY;

    if (camera.useFreeCamera)
    {
        float angleX = dx/((float) _width/2)  * M_PI * 2;
        float angleY = dy/((float) _height/2) * M_PI * 2;
        camera.updateViewVector(angleX, angleY);
        camera.updateSphericAngles(angleX);
    }
    else
        camera.updateSphericAngles(dx, dy);
    g_LastCursorPosX = xpos;
    g_LastCursorPosY = ypos;
}

// Função callback chamada sempre que o usuário movimenta a "rodinha" do mouse.
void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.updateSphericDistance(yoffset);
}

// Definição da função que será chamada sempre que o usuário pressionar alguma
// tecla do teclado. Veja http://www.glfw.org/docs/latest/input_guide.html#input_key
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mod)
{
    // Se o usuário pressionar a tecla ESC, fechamos a janela.
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if (key == GLFW_KEY_C && action == GLFW_PRESS)
        camera.useFreeCamera = !camera.useFreeCamera;

    glfwSetInputMode(glfwGetCurrentContext(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    switch (key)
    {
        case GLFW_KEY_UP:
            if (action == GLFW_PRESS)
                camera.up = true;
            else if (action == GLFW_RELEASE)
                camera.up = false;
            break;
        case GLFW_KEY_LEFT:
            if (action == GLFW_PRESS)
                camera.left = true;
            else if (action == GLFW_RELEASE)
                camera.left = false;
        break;
        case GLFW_KEY_DOWN:
            if (action == GLFW_PRESS)
                camera.down = true;
            else if (action == GLFW_RELEASE)
                camera.down = false;

            break;
        case GLFW_KEY_RIGHT:
            if (action == GLFW_PRESS)
                camera.right = true;
            else if (action == GLFW_RELEASE)
                camera.right = false;
            break;
    };
}

int main(int argc, char* argv[])
{

    // Inicializamos a biblioteca GLFW, utilizada para criar uma janela do
    // sistema operacional, onde poderemos renderizar com OpenGL.
    int success = glfwInit();
    if (!success)
    {
        fprintf(stderr, "ERROR: glfwInit() failed.\n");
        std::exit(EXIT_FAILURE);
    }

    GLFWwindow* window;

    // Definimos o callback para impressão de erros da GLFW no terminal
    glfwSetErrorCallback(ErrorCallback);

    // Pedimos para utilizar OpenGL versão 3.3 (ou superior)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    #ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    // Pedimos para utilizar o perfil "core", isto é, utilizaremos somente as
    // funções modernas de OpenGL.
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Criamos uma janela do sistema operacional
    // de pixels, e com título "INF01047 ...".
    window = glfwCreateWindow(_width, _height, "INF01047 - Ultimate Fruit Ninja", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        fprintf(stderr, "ERROR: glfwCreateWindow() failed.\n");
        std::exit(EXIT_FAILURE);
    }

    // Definimos a função de callback que será chamada sempre que o usuário
    // pressionar alguma tecla do teclado ...
    glfwSetKeyCallback(window, KeyCallback);
    // ... ou clicar os botões do mouse ...
    glfwSetMouseButtonCallback(window, MouseButtonCallback);
    // ... ou movimentar o cursor do mouse em cima da janela ...
    glfwSetCursorPosCallback(window, CursorPosCallback);
    // ... ou rolar a "rodinha" do mouse.
    glfwSetScrollCallback(window, ScrollCallback);

    // Indicamos que as chamadas OpenGL deverão renderizar nesta janela
    glfwMakeContextCurrent(window);

    // Carregamento de todas funções definidas por OpenGL 3.3, utilizando a
    // biblioteca GLAD.
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);

    // Definimos a função de callback que será chamada sempre que a janela for
    // redimensionada, por consequência alterando o tamanho do "framebuffer"
    // (região de memória onde são armazenados os pixels da imagem).
    glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
    FramebufferSizeCallback(window, _width, _height);

    glfwSetWindowSize(window, _width, _height);

    // Imprimimos no terminal informações sobre a GPU do sistema
    const GLubyte *vendor      = glGetString(GL_VENDOR);
    const GLubyte *renderer    = glGetString(GL_RENDERER);
    const GLubyte *glversion   = glGetString(GL_VERSION);
    const GLubyte *glslversion = glGetString(GL_SHADING_LANGUAGE_VERSION);

    printf("GPU: %s, %s, OpenGL %s, GLSL %s\n", vendor, renderer, glversion, glslversion);

    // Carregamos os shaders de vértices e de fragmentos que serão utilizados
    // para renderização. Veja slides 180-200 do documento Aula_03_Rendering_Pipeline_Grafico.pdf.
    render.LoadShadersFromFiles();

    // Carregamos imagens para serem utilizadas como textura
    render.LoadTextureImage("../../data/madeira.jpg");
    render.LoadTextureImage("../../data/chef.jpg");
    // render.LoadTextureImage("../../data/knife.png");
    render.LoadTextureImage("../../data/banana.png");
    render.LoadTextureImage("../../data/maca.jpg");
    render.LoadTextureImage("../../data/abacaxi.jpg");
    render.LoadTextureImage("../../data/laranja.jpg");

    ObjModel cozinhaModel(COZINHA,
                  glm::vec3(0.0f, 0.0f, 0.0f),
                  glm::vec3(8.0f, 8.0f, 8.0f),
                  glm::vec3(0.0f, 0.0f, 0.0f),
                  0.0f,
                  "cozinha",
                  "../../data/cozinha.obj",
                  render.g_VirtualScene);
    render.models.push_back(cozinhaModel);

    ObjModel chefModel(CHEF,
                 glm::vec3(0.0f, 0.0f, 0.0f),
                 glm::vec3(0.4f, 0.4f, 0.4f),
                 glm::vec3(0.0f, -8.0f, 1.0f),
                  0.0f,
                  "chef",
                  "../../data/chef.obj",
                  render.g_VirtualScene);
    render.models.push_back(chefModel);

    ObjModel knifeModel(KNIFE,
                 glm::vec3(0.0f, 0.0f, 0.0f),
                 glm::vec3(0.02f, 0.02f, 0.02f),
                 glm::vec3(0.0f, 0.0f, 0.0f),
                 0.0f,
                 "Knife_1",
                 "../../data/knife.obj",
                 render.g_VirtualScene);
    render.models.push_back(knifeModel);

    ObjModel bananaModel(BANANA,
                 glm::vec3(0.0f, 8.0f, 0.0f),
                 glm::vec3(1.2f, 1.2f, 1.2f),
                 glm::vec3(0.0f, 0.0f, 0.0f),
                 0.0f,
                 "Banana",
                 "../../data/banana.obj",
                 render.g_VirtualScene);
    render.models.push_back(bananaModel);

    ObjModel macaModel(MACA,
                    glm::vec3(10.0f, 17.7f, 0.0f),
                    glm::vec3(14.0f, 14.0f, 14.0f),
                    glm::vec3(0.0f, 0.0f, 0.0f),
                    M_PI / 2,
                    "maca",
                    "../../data/maca.obj",
                    render.g_VirtualScene);
    render.models.push_back(macaModel);

    ObjModel abacaxiModel(ABACAXI,
                    glm::vec3(0.0f, 8.0f, 0.0f),
                    glm::vec3(0.1f, 0.1f, 0.1f),
                    glm::vec3(0.0f, 0.0f, 0.0f),
                    M_PI / 2,
                    "Pineapple",
                    "../../data/abacaxi.obj",
                    render.g_VirtualScene);
    render.models.push_back(abacaxiModel);

    ObjModel laranjaModel(LARANJA,
                    glm::vec3(0.0f, 8.0f, 0.0f),
                    glm::vec3(0.2f, 0.2f, 0.2f),
                    glm::vec3(0.0f, 0.0f, 0.0f),
                    M_PI / 2,
                    "Orange",
                    "../../data/laranja.obj",
                    render.g_VirtualScene);
    render.models.push_back(laranjaModel);

    // Habilitamos o Z-buffer. Veja slides 104-116 do documento Aula_09_Projecoes.pdf.
    glEnable(GL_DEPTH_TEST);

    // Habilitamos o Backface Culling. Veja slides 8-13 do documento Aula_02_Fundamentos_Matematicos.pdf,
    // slides 23-34 do documento Aula_13_Clipping_and_Culling.pdf e slides 112-123
    // do documento Aula_14_Laboratorio_3_Revisao.pdf.
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    auto prevTime = (float) glfwGetTime();
    auto spawnTime = prevTime;

    // Ficamos em um loop infinito, renderizando, até que o usuário feche a janela
    while (!glfwWindowShouldClose(window))
        if (!render.LoadWindow(window, camera, ((float) _width / (float) _height), prevTime, spawnTime))
            break;

    // Finalizamos o uso dos recursos do sistema operacional
    glfwTerminate();

    // Fim do programa
    return 0;
}


