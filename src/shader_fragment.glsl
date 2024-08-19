#version 330 core

// Atributos de fragmentos recebidos como entrada ("in") pelo Fragment Shader.
// Neste exemplo, este atributo foi gerado pelo rasterizador como a
// interpolação da posição global e a normal de cada vértice, definidas em
// "shader_vertex.glsl" e "main.cpp".
in vec4 position_world;
in vec4 normal;

// Posição do vértice atual no sistema de coordenadas local do modelo.
in vec4 position_model;

// Coordenadas de textura obtidas do arquivo OBJ (se existirem!)
in vec2 texcoords;

// Matrizes computadas no código C++ e enviadas para a GPU
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// Identificador que define qual objeto está sendo desenhado no momento
#define COZINHA 0
#define CHEF    2
#define KNIFE   3
#define BANANA  4
#define MACA    5
#define ABACAXI 6
#define LARANJA 7
uniform int object_id;

// Parâmetros da axis-aligned bounding box (AABB) do modelo
uniform vec4 bbox_min;
uniform vec4 bbox_max;

// Variáveis para acesso das imagens de textura
uniform sampler2D TextureCozinha;
uniform sampler2D TextureChef;
uniform sampler2D TextureBanana;
uniform sampler2D TextureMaca;
uniform sampler2D TextureAbacaxi;
uniform sampler2D TextureLaranja;
uniform sampler2D TextureKnife;

// O valor de saída ("out") de um Fragment Shader é a cor final do fragmento.
out vec4 color;

void main()
{
    // Obtemos a posição da câmera utilizando a inversa da matriz que define o
    // sistema de coordenadas da câmera.
    vec4 origin = vec4(0.0, 0.0, 0.0, 1.0);
    vec4 camera_position = inverse(view) * origin;

    // O fragmento atual é coberto por um ponto que percente à superfície de um
    // dos objetos virtuais da cena. Este ponto, p, possui uma posição no
    // sistema de coordenadas global (World coordinates). Esta posição é obtida
    // através da interpolação, feita pelo rasterizador, da posição de cada
    // vértice.
    vec4 p = position_world;

    // Normal do fragmento atual, interpolada pelo rasterizador a partir das
    // normais de cada vértice.
    vec4 n = normalize(normal);

    // Vetor que define o sentido da fonte de luz em relação ao ponto atual.
    vec4 l = normalize(vec4(1.0,1.0,0.0,0.0));

    // Vetor que define o sentido da câmera em relação ao ponto atual.
    vec4 v = normalize(camera_position - p);

    vec3 Kd; // Refletância difusa
    vec3 Ks; // Refletância especular
    vec3 Ka; // Refletância ambiente
    float q; // Expoente especular para o modelo de iluminação de Phong

    // Coordenadas de textura U e V
    float U = 0.0;
    float V = 0.0;
    vec4 r = -l + 2 * n * dot(n, l);
    vec3 I = vec3(0.4,0.4,0.4);
    vec3 Ia = vec3(0.2,0.2,0.2);

    if ( object_id == COZINHA )
    {
        Kd = texture(TextureCozinha, texcoords).rgb;
        Ks = vec3(0.8,0.8,0.8);
        Ka = Kd / 2.0;
        q = 32.0;
    }
    else if ( object_id == CHEF )
    {
        Kd = texture(TextureChef, texcoords).rgb;
        Ks = vec3(0.8,0.8,0.8);
        Ka = Kd / 2.0;
        q = 32.0;
    }
    else if ( object_id == KNIFE )
    {
        Kd = texture(TextureKnife, texcoords).rgb;
        Ks = vec3(0.8,0.8,0.8);
        Ka = Kd / 2.0;
        q = 32.0;
    }
    else if ( object_id == BANANA )
    {
        Kd = texture(TextureBanana, texcoords).rgb;
        Ks = vec3(0.8,0.8,0.8);
        Ka = Kd / 2.0;
        q = 32.0;
    }
    else if ( object_id == MACA )
    {
        Kd = texture(TextureMaca, texcoords).rgb;
        Ks = vec3(0.8,0.8,0.8);
        Ka = Kd / 2.0;
        q = 32.0;

    }
    else if ( object_id == ABACAXI )
    {
        Kd = texture(TextureAbacaxi, texcoords).rgb;
        Ks = vec3(0.8,0.8,0.8);
        Ka = Kd / 2.0;
        q = 32.0;

    }
    else if ( object_id == LARANJA )
    {
        Kd = texture(TextureLaranja, texcoords).rgb;
        Ks = vec3(0.8,0.8,0.8);
        Ka = Kd / 2.0;
        q = 32.0;

    }
    else
    {
        Kd = vec3(0.0,0.0,0.0);
        Ks = vec3(0.0,0.0,0.0);
        Ka = vec3(0.0,0.0,0.0);
        q = 1.0;
    }

    // NOTE: Se você quiser fazer o rendering de objetos transparentes, é
    // necessário:
    // 1) Habilitar a operação de "blending" de OpenGL logo antes de realizar o
    //    desenho dos objetos transparentes, com os comandos abaixo no código C++:
    //      glEnable(GL_BLEND);
    //      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // 2) Realizar o desenho de todos objetos transparentes *após* ter desenhado
    //    todos os objetos opacos; e
    // 3) Realizar o desenho de objetos transparentes ordenados de acordo com
    //    suas distâncias para a câmera (desenhando primeiro objetos
    //    transparentes que estão mais longe da câmera).
    // Alpha default = 1 = 100% opaco = 0% transparente
    color.a = 1;

    // Cor final com correção gamma, considerando monitor sRGB.
    // Veja https://en.wikipedia.org/w/index.php?title=Gamma_correction&oldid=751281772#Windows.2C_Mac.2C_sRGB_and_TV.2Fvideo_standard_gammas
    color.rgb = Kd * I * max(0, dot(n, l)) + Ka * Ia + Ks * I * pow(max(0, dot(r, v)), q);
    color.rgb = pow(color.rgb, vec3(1.0,1.0,1.0)/2.2);
}

