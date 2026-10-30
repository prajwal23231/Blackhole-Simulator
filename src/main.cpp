#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "shader.h"
#include <vector>
#include <cmath>
#include <cstdlib>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

float yaw = -90.0f;
float pitch = 0.0f;
float fov = 45.0f;
float lastX = 1280.0f / 2.0f;
float lastY = 720.0f / 2.0f;
bool firstMouse = true;

glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);

void frambuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn){
    if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) != GLFW_PRESS){
        firstMouse = true;
        return ;
    }

    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if(firstMouse){
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if(pitch > 89.0f) pitch = 89.0f;
    if(pitch < -89.0f) pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
    fov -= (float)yoffset * 2.0f;
    if(fov < 5.0f) fov = 5.0f;
    if(fov > 80.0f) fov = 80.0f;
}

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

    glfwSetFramebufferSizeCallback(window, frambuffer_size_callback);

    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glEnable(GL_PROGRAM_POINT_SIZE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    Shader starShader("shaders/star.vert","shaders/star.frag");

    const int NUM_STARS = 1500;
    std::vector<float> starVertices;
    starVertices.reserve(NUM_STARS*3);

    for(int i=0; i<NUM_STARS; ++i){
        float x = ((float)rand() / RAND_MAX) * 2.0f - 1.0f;
        float y = ((float)rand() / RAND_MAX) * 2.0f - 1.0f;
        float z = ((float)rand() / RAND_MAX) * 2.0f - 1.0f;

        float len = std::sqrt(x*x + y*y + z*z);
        if(len == 0.0f) len = 1.0f;

        float radius = 150.0f + ((float)rand() / RAND_MAX) * 150.0f;
        starVertices.push_back((x/len) * radius);
        starVertices.push_back((y/len) * radius);
        starVertices.push_back((z/len) * radius);
    }

    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, starVertices.size()*sizeof(float), starVertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    while(!glfwWindowShouldClose(window)){
        if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
            glfwSetWindowShouldClose(window, true);
        }
        
        glClear(GL_COLOR_BUFFER_BIT);

        starShader.use();

        glm::mat4 projection = glm::perspective(glm::radians(fov), 1280.0f/720.0f, 0.1f, 600.0f);
        float time = (float)glfwGetTime() * 0.1f;
        glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, glm::vec3(0.0f, 1.0f, 0.0f));

        glUniformMatrix4fv(glGetUniformLocation(starShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(starShader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));

        glBindVertexArray(VAO);
        glDrawArrays(GL_POINTS, 0, NUM_STARS);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
}