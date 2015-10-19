#ifndef UGLY_TEST_GLFWAPP_H
#define UGLY_TEST_GLFWAPP_H

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>


class glfwApp {
  private:
    static unsigned _refs;
    static bool _done;
    static void key_callback(GLFWwindow*, int, int, int, int);
    static void on_error(int, const char*);

  public:
    glfwApp(int major = 4, int minor = 1);
    ~glfwApp();

  public:
    void update();
    bool done() const;
    void make_current();
    int width() const;
    int height() const;

  private:
    GLFWwindow* _window { nullptr };

};

#endif

