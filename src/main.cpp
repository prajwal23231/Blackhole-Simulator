#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>

using namespace std;

int main(){
    if(!glfwInit()){
        cerr << "Failed to initialize GLFW\n";
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(1280, 720, "BlackHole Sim", nullptr, nullptr);

    if(window == nullptr){
        glfwTerminate();
        cerr << "Failed to create window\n";
        return -1;
    }

    glfwMakeContextCurrent(window);

    if(!gladLoadGL((GLADloadfunc)glfwGetProcAddress)){
        glfwDestroyWindow(window);
        glfwTerminate();
        cerr << "Failed to initialise glad2\n";
        return -1;
    }

    while(!glfwWindowShouldClose(window)){
        if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
            glfwSetWindowShouldClose(window, true);
        }
        
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
}