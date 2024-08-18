#ifndef CALLBACKS_H_
#define CALLBACKS_H_

#include "camera.h"
#include "render.h"
//#include "glad/glad.h"
//#include <GLFW/glfw3.h>


class Callbacks {
    public:
        int _height;
        int _width;
        double g_LastCursorPosX;
        double g_LastCursorPosY;
        // Funções callback para comunicação com o sistema operacional e interação do
        // usuário. Veja mais comentários nas definições das mesmas.
        void FramebufferSizeCallback(int width, int height);
        void ErrorCallback(int error, const char* description);
        void KeyCallback(int key, int scancode, int action, int mode);
        void MouseButtonCallback(int button, int action, int mods);
        void CursorPosCallback(double xpos, double ypos);
        void ScrollCallback(double xoffset, double yoffset);
        Callbacks();
};


#endif //CALLBACKS_H_
