#include "../include/render.h"

#define COZINHA 0
#define CHEF    2
#define KNIFE   3
#define BANANA  4
#define MACA    5
#define ABACAXI 6
#define LARANJA 7

Render::Render() {

    this->g_GpuProgramID = 0;
    this->g_NumLoadedTextures = 0;
}

// Carrega um Vertex Shader de um arquivo GLSL. Veja definição de LoadShader() abaixo.
GLuint Render::LoadShader_Vertex(const char* filename)
{
    // Criamos um identificador (ID) para este shader, informando que o mesmo
    // será aplicado nos vértices.
    GLuint vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);

    // Carregamos e compilamos o shader
    this->LoadShader(filename, vertex_shader_id);

    // Retorna o ID gerado acima
    return vertex_shader_id;
}

// Carrega um Fragment Shader de um arquivo GLSL . Veja definição de LoadShader() abaixo.
GLuint Render::LoadShader_Fragment(const char* filename)
{
    // Criamos um identificador (ID) para este shader, informando que o mesmo
    // será aplicado nos fragmentos.
    GLuint fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);

    // Carregamos e compilamos o shader
    this->LoadShader(filename, fragment_shader_id);

    // Retorna o ID gerado acima
    return fragment_shader_id;
}

// Função auxilar, utilizada pelas duas funções acima. Carrega código de GPU de
// um arquivo GLSL e faz sua compilação.
void Render::LoadShader(const char* filename, GLuint shader_id)
{
    // Lemos o arquivo de texto indicado pela variável "filename"
    // e colocamos seu conteúdo em memória, apontado pela variável
    // "shader_string".
    std::ifstream file;
    try {
        file.exceptions(std::ifstream::failbit);
        file.open(filename);
    } catch ( std::exception& e ) {
        fprintf(stderr, "ERROR: Cannot open file \"%s\".\n", filename);
        std::exit(EXIT_FAILURE);
    }
    std::stringstream shader;
    shader << file.rdbuf();
    std::string str = shader.str();
    const GLchar* shader_string = str.c_str();
    const GLint   shader_string_length = static_cast<GLint>( str.length() );

    // Define o código do shader GLSL, contido na string "shader_string"
    glShaderSource(shader_id, 1, &shader_string, &shader_string_length);

    // Compila o código do shader GLSL (em tempo de execução)
    glCompileShader(shader_id);

    // Verificamos se ocorreu algum erro ou "warning" durante a compilação
    GLint compiled_ok;
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &compiled_ok);

    GLint log_length = 0;
    glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &log_length);

    // Alocamos memória para guardar o log de compilação.
    // A chamada "new" em C++ é equivalente ao "malloc()" do C.
    GLchar* log = new GLchar[log_length];
    glGetShaderInfoLog(shader_id, log_length, &log_length, log);

    // Imprime no terminal qualquer erro ou "warning" de compilação
    if ( log_length != 0 )
    {
        std::string  output;

        if ( !compiled_ok )
        {
            output += "ERROR: OpenGL compilation of \"";
            output += filename;
            output += "\" failed.\n";
            output += "== Start of compilation log\n";
            output += log;
            output += "== End of compilation log\n";
        }
        else
        {
            output += "WARNING: OpenGL compilation of \"";
            output += filename;
            output += "\".\n";
            output += "== Start of compilation log\n";
            output += log;
            output += "== End of compilation log\n";
        }

        fprintf(stderr, "%s", output.c_str());
    }

    // A chamada "delete" em C++ é equivalente ao "free()" do C
    delete [] log;
}

// Esta função cria um programa de GPU, o qual contém obrigatoriamente um
// Vertex Shader e um Fragment Shader.
GLuint Render::CreateGpuProgram(GLuint vertex_shader_id, GLuint fragment_shader_id)
{
    // Criamos um identificador (ID) para este programa de GPU
    GLuint program_id = glCreateProgram();

    // Definição dos dois shaders GLSL que devem ser executados pelo programa
    glAttachShader(program_id, vertex_shader_id);
    glAttachShader(program_id, fragment_shader_id);

    // Linkagem dos shaders acima ao programa
    glLinkProgram(program_id);

    // Verificamos se ocorreu algum erro durante a linkagem
    GLint linked_ok = GL_FALSE;
    glGetProgramiv(program_id, GL_LINK_STATUS, &linked_ok);

    // Imprime no terminal qualquer erro de linkagem
    if ( linked_ok == GL_FALSE )
    {
        GLint log_length = 0;
        glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &log_length);

        // Alocamos memória para guardar o log de compilação.
        // A chamada "new" em C++ é equivalente ao "malloc()" do C.
        GLchar* log = new GLchar[log_length];

        glGetProgramInfoLog(program_id, log_length, &log_length, log);

        std::string output;

        output += "ERROR: OpenGL linking of program failed.\n";
        output += "== Start of link log\n";
        output += log;
        output += "\n== End of link log\n";

        // A chamada "delete" em C++ é equivalente ao "free()" do C
        delete [] log;

        fprintf(stderr, "%s", output.c_str());
    }

    // Os "Shader Objects" podem ser marcados para deleção após serem linkados
    glDeleteShader(vertex_shader_id);
    glDeleteShader(fragment_shader_id);

    // Retornamos o ID gerado acima
    return program_id;
}

// Função que carrega uma imagem para ser utilizada como textura
void Render::LoadTextureImage(const char* filename)
{
    printf("Carregando imagem \"%s\"... ", filename);

    // Primeiro fazemos a leitura da imagem do disco
    stbi_set_flip_vertically_on_load(true);
    int width;
    int height;
    int channels;
    unsigned char *data = stbi_load(filename, &width, &height, &channels, 3);

    if ( data == NULL )
    {
        fprintf(stderr, "ERROR: Cannot open image file \"%s\".\n", filename);
        std::exit(EXIT_FAILURE);
    }

    printf("OK (%dx%d).\n", width, height);

    // Agora criamos objetos na GPU com OpenGL para armazenar a textura
    GLuint texture_id;
    GLuint sampler_id;
    glGenTextures(1, &texture_id);
    glGenSamplers(1, &sampler_id);
    // Veja slides 95-96 do documento Aula_20_Mapeamento_de_Texturas.pdf
    glSamplerParameteri(sampler_id, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glSamplerParameteri(sampler_id, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // Parâmetros de amostragem da textura.
    glSamplerParameteri(sampler_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glSamplerParameteri(sampler_id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Agora enviamos a imagem lida do disco para a GPU
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
    glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
    GLuint textureunit = this->g_NumLoadedTextures;
    glActiveTexture(GL_TEXTURE0 + textureunit);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindSampler(textureunit, sampler_id);
    stbi_image_free(data);
    g_NumLoadedTextures += 1;
}

// Função que carrega os shaders de vértices e de fragmentos que serão
// utilizados para renderização. Veja slides 180-200 do documento Aula_03_Rendering_Pipeline_Grafico.pdf.
//
void Render::LoadShadersFromFiles()
{
    // Note que o caminho para os arquivos "shader_vertex.glsl" e
    // "shader_fragment.glsl" estão fixados, sendo que assumimos a existência
    // da seguinte estrutura no sistema de arquivos:
    //
    //    + FCG_Lab_01/
    //    |
    //    +--+ bin/
    //    |  |
    //    |  +--+ Release/  (ou Debug/ ou Linux/)
    //    |     |
    //    |     o-- main.exe
    //    |
    //    +--+ src/
    //       |
    //       o-- shader_vertex.glsl
    //       |
    //       o-- shader_fragment.glsl
    //
    GLuint vertex_shader_id = LoadShader_Vertex("../../src/shader_vertex.glsl");
    GLuint fragment_shader_id = LoadShader_Fragment("../../src/shader_fragment.glsl");

    // Deletamos o programa de GPU anterior, caso ele exista.
    if (this->g_GpuProgramID != 0 )
        glDeleteProgram(this->g_GpuProgramID);

    // Criamos um programa de GPU utilizando os shaders carregados acima.
    this->g_GpuProgramID = CreateGpuProgram(vertex_shader_id, fragment_shader_id);

    // Buscamos o endereço das variáveis definidas dentro do Vertex Shader.
    // Utilizaremos estas variáveis para enviar dados para a placa de vídeo
    // (GPU)! Veja arquivo "shader_vertex.glsl" e "shader_fragment.glsl".
    this->g_model_uniform      = glGetUniformLocation(this->g_GpuProgramID, "model"); // Variável da matriz "model"
    this->g_view_uniform       = glGetUniformLocation(this->g_GpuProgramID, "view"); // Variável da matriz "view" em shader_vertex.glsl
    this->g_projection_uniform = glGetUniformLocation(this->g_GpuProgramID, "projection"); // Variável da matriz "projection" em shader_vertex.glsl
    this->g_object_id_uniform  = glGetUniformLocation(this->g_GpuProgramID, "object_id"); // Variável "object_id" em shader_fragment.glsl
    this->g_bbox_min_uniform   = glGetUniformLocation(this->g_GpuProgramID, "bbox_min");
    this->g_bbox_max_uniform   = glGetUniformLocation(this->g_GpuProgramID, "bbox_max");

    // Variáveis em "shader_fragment.glsl" para acesso das imagens de textura
    glUseProgram(this->g_GpuProgramID);
    glUniform1i(glGetUniformLocation(this->g_GpuProgramID, "TextureCozinha"), 0);
    glUniform1i(glGetUniformLocation(this->g_GpuProgramID, "TextureChef"), 2);
    glUniform1i(glGetUniformLocation(this->g_GpuProgramID, "TextureBanana"), 4);
    glUniform1i(glGetUniformLocation(this->g_GpuProgramID, "TextureMaca"), 5);
    glUniform1i(glGetUniformLocation(this->g_GpuProgramID, "TextureAbacaxi"), 6);
    glUniform1i(glGetUniformLocation(this->g_GpuProgramID, "TextureLaranja"), 7);
    glUniform1i(glGetUniformLocation(this->g_GpuProgramID, "TextureKnife"), 3);

    glUseProgram(0);
}

// Função que desenha um objeto armazenado em g_VirtualScene. Veja definição
// dos objetos na função BuildTrianglesAndAddToVirtualScene().
void Render::DrawVirtualObject(const char* object_name)
{
    // "Ligamos" o VAO. Informamos que queremos utilizar os atributos de
    // vértices apontados pelo VAO criado pela função BuildTrianglesAndAddToVirtualScene(). Veja
    // comentários detalhados dentro da definição de BuildTrianglesAndAddToVirtualScene().
    glBindVertexArray(this->g_VirtualScene[object_name].vertex_array_object_id);

    // Setamos as variáveis "bbox_min" e "bbox_max" do fragment shader
    // com os parâmetros da axis-aligned bounding box (AABB) do modelo.
    glm::vec3 bbox_min = this->g_VirtualScene[object_name].bbox_min;
    glm::vec3 bbox_max = this->g_VirtualScene[object_name].bbox_max;
    glUniform4f(this->g_bbox_min_uniform, bbox_min.x, bbox_min.y, bbox_min.z, 1.0f);
    glUniform4f(this->g_bbox_max_uniform, bbox_max.x, bbox_max.y, bbox_max.z, 1.0f);

    // Pedimos para a GPU rasterizar os vértices dos eixos XYZ
    // apontados pelo VAO como linhas. Veja a definição de
    // g_VirtualScene[""] dentro da função BuildTrianglesAndAddToVirtualScene(), e veja
    // a documentação da função glDrawElements() em
    // http://docs.gl/gl3/glDrawElements.
    glDrawElements(
        this->g_VirtualScene[object_name].rendering_mode,
        this->g_VirtualScene[object_name].num_indices,
        GL_UNSIGNED_INT,
        (void*)(this->g_VirtualScene[object_name].first_index * sizeof(GLuint))
    );

    // "Desligamos" o VAO, evitando assim que operações posteriores venham a
    // alterar o mesmo. Isso evita bugs.
    glBindVertexArray(0);
}

struct Fruit {
    glm::vec3 position;
    glm::vec3 direction;
    glm::vec3 bbox_min;
    glm::vec3 bbox_max;
    float speed;
    int type;

    Fruit() {
        this->position = glm::vec3(0.0f, 0.0f, 0.0f);
        this->direction = glm::vec3(0.0f, 0.0f, 0.0f);
        this->bbox_min = glm::vec3(0.0f, 0.0f, 0.0f);
        this->bbox_max = glm::vec3(0.0f, 0.0f, 0.0f);
        this->speed = 0.0f;
        this->type = 0;
    }

};

bool Kill, ataque = false;
int phase = 0, fruitsPart = 0;
std::vector<Fruit> fruits;

bool collisionScenario(glm::vec3 cubeBbox_min, glm::vec3 cubeBbox_max, glm::vec3 boxBbox_min, glm::vec3 boxBbox_max) {
    if (cubeBbox_min.y >= boxBbox_min.y) {
        return false;
    }
    return true;
}


bool Render::LoadWindow(GLFWwindow* window, Camera &camera, const float &aspectRatio, float &initialTime, float &spawnTime) {
    // Definimos a cor do "fundo" do framebuffer como branco.  Tal cor é
    // definida como coeficientes RGBA: Red, Green, Blue, Alpha; isto é:
    // Vermelho, Verde, Azul, Alpha (valor de transparência).
    // Conversaremos sobre sistemas de cores nas aulas de Modelos de Iluminação.
    //
    //           R     G     B     A
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    // "Pintamos" todos os pixels do framebuffer com a cor definida acima,
    // e também resetamos todos os pixels do Z-buffer (depth buffer).
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Pedimos para a GPU utilizar o programa de GPU criado acima (contendo
    // os shaders de vértice e fragmentos).
    glUseProgram(g_GpuProgramID);

    glm::mat4 model = Matrix_Identity();
    glUniformMatrix4fv(this->g_view_uniform       , 1 , GL_FALSE , glm::value_ptr(camera.getView()));
    glUniformMatrix4fv(this->g_projection_uniform , 1 , GL_FALSE , glm::value_ptr(Matrix_Perspective((M_PI / 3.0f), aspectRatio, camera.nearPlane, camera.farPlane)));

    float currentTime = glfwGetTime();
    float delta_t = currentTime - initialTime;
    initialTime = currentTime;
    float spawn_delta_t = currentTime - spawnTime;
    float spawningTime = 10.0f - (float) phase;
    phase = int(fruitsPart/10);

    if (spawn_delta_t >= spawningTime){
        Fruit fruit;
        fruit.speed = (1.0f + (float) phase)/20.0f;
        fruit.direction = glm::vec3(0.0f, 1.0f, 0.0f);
        for(int i=1; i<5; i++){
            fruit.type = i+3;
            fruit.position = glm::vec3(5.0f, 5.0f, i*3.0f-6.0f);
            fruits.push_back(fruit);
        }
        spawnTime = currentTime;
    }

    camera.updateCamera(delta_t);
    if (camera.ataque) {
        Kill = true;
    }

    for (ObjModel &object : this->models) {
        if (object.objectId == KNIFE) {
            float Speed = 5.0f;
            if (Kill && !ataque) {
                object.position = this->models[CHEF].position;
                object.originalPosition = object.position;
                object.bbox_max = object.position;
                object.bbox_min = object.position;
                object.direction = normalize(glm::vec3(1.0f, 1.0 * cos(this->models[CHEF].rotation) + 1.0 * sin(this->models[CHEF].rotation), 0.0f));
                if (camera.ataque)
                    ataque = true;
            }
            if (ataque) {
                Kill= false;
                object.position = object.position + object.direction * delta_t * Speed;
                object.position.y = 2.0f;
                model = Matrix_Translate(object.position.x, object.position.y, object.position.z);
                model *= Matrix_Scale(object.scale.x, object.scale.y, object.scale.z);
                model *= Matrix_Rotate_Z(Speed*currentTime);
                object.bbox_max = object.position;
                object.bbox_min = object.position;
                glUniformMatrix4fv(this->g_model_uniform, 1, GL_FALSE, glm::value_ptr(model));
                glUniform1i(this->g_object_id_uniform, KNIFE);
                this->DrawVirtualObject(object.name.c_str());
            }
            else
                ataque = false;
        }
        else if (object.objectId >= 4) {

            for (int i = 0; i < int(fruits.size()); i++) {
            if(object.objectId==fruits[i].type)
            {
                if (collisionScenario(fruits[i].bbox_min, fruits[i].bbox_max, this->models[COZINHA].bbox_min, this->models[COZINHA].bbox_max)) {
                    printf("Você perdeu! Uma fruta caiu no chão. Seu score foi de: %d\n", fruitsPart);
                    return false;
                }

                fruits[i].position = fruits[i].position - glm::vec3(0.0f, 8.0f, 0.0f) * delta_t * fruits[i].speed;
                fruits[i].bbox_max = glm::vec3(fruits[i].position.x, fruits[i].position.y, fruits[i].position.z);
                fruits[i].bbox_min = glm::vec3(fruits[i].position.x, fruits[i].position.y, fruits[i].position.z);

                model = Matrix_Translate(fruits[i].position.x, fruits[i].position.y, fruits[i].position.z);
                model *= Matrix_Scale(object.scale.x, object.scale.y, object.scale.z);
                glUniformMatrix4fv(this->g_model_uniform, 1, GL_FALSE, glm::value_ptr(model));
                glUniform1i(this->g_object_id_uniform, object.objectId);
                this->DrawVirtualObject(object.name.c_str());
            }
            }
        }
        else {

            if (object.objectId == CHEF)
            {
                float movee = 2.0f * delta_t;
                object.rotation = atan2f(1.0f, object.direction.x) - atan2f(camera.viewVector.z, camera.viewVector.x);
                glm::vec4 w = -(normalize(camera.viewVector));
                glm::vec4 u = normalize(crossproduct(camera.upVector, w));
                if (camera.up)
                {
                    object.position.x -= w.x * movee;
                    object.position.z -= w.z * movee;
                }
                if (camera.down)
                {
                    object.position.x += w.x * movee;
                    object.position.z += w.z * movee;

                }
                if (camera.left)
                {
                    object.position.x -= u.x * movee;
                    object.position.z -= u.z * movee;

                }
                if (camera.right)
                {
                    object.position.x += u.x * movee;
                    object.position.z += u.z * movee;
                }
                object.position.y = 1.0f;
                camera.cartesianPosition = glm::vec4(object.position.x + 1.0f, object.position.y + 2.0f, object.position.z, 1.0f);

                if (!camera.useFreeCamera) {
                    glm::vec3 pos = object.position;
                    camera.lookAt = glm::vec4(pos.x - 2.0f, pos.y + 2.0f, pos.z, 1.0f);
                }
            }

            model = Matrix_Translate(object.position.x, object.position.y, object.position.z);
            model *= Matrix_Scale(object.scale.x, object.scale.y, object.scale.z);
            object.bbox_max = object.position;
            object.bbox_min = object.position;

            glUniformMatrix4fv(this->g_model_uniform, 1, GL_FALSE, glm::value_ptr(model));
            glUniform1i(this->g_object_id_uniform, object.objectId);
            this->DrawVirtualObject(object.name.c_str());
        }
    }
    // O framebuffer onde OpenGL executa as operações de renderização não
    // é o mesmo que está sendo mostrado para o usuário, caso contrário
    // seria possível ver artefatos conhecidos como "screen tearing". A
    // chamada abaixo faz a troca dos buffers, mostrando para o usuário
    // tudo que foi renderizado pelas funções acima.
    // Veja o link: https://en.wikipedia.org/w/index.php?title=Multiple_buffering&oldid=793452829#Double_buffering_in_computer_graphics
    glfwSwapBuffers(window);

    // Verificamos com o sistema operacional se houve alguma interação do
    // usuário (teclado, mouse, ...). Caso positivo, as funções de callback
    // definidas anteriormente usando glfwSet*Callback() serão chamadas
    // pela biblioteca GLFW.
    glfwPollEvents();
    return true;
}
