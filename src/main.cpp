#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdio.h>
#include <vector>
#include "Shader.h"
#include "Eigen/Core"
#include "stb_image.h"
#include "EulerOperationTest.h"
#include "earcut.h"
#include "CameraPOV.h"
#include "testVisual.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void processInput(GLFWwindow* window);
Eigen::Matrix4f getProjectionMatrix();
Eigen::Matrix4f getProjectionMatrixTest();

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 800;
CameraPOV camera(Eigen::Vector3f({ 0.0f, 0.0f, 0.0f }));
float lastX = WIDTH / 2.0f;
float lastY = HEIGHT / 2.0f;
bool firstMouse = true;
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main()
{
    glfwInit();
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Create a GLFW window object that we can use for GLFW's functions
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "CAD", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    
    // Set the required callback functions
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    glewInit();

    glEnable(GL_DEPTH_TEST);

    // Define the shader
    Shader shader("./shader/vertex.shader", "./shader/fragment.shader");
    shader.Use();


    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,
    };

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    int pointNum = visualCube(VBO, VAO, EBO);

    // position
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (GLvoid*)0);
    //glEnableVertexAttribArray(0);
    // color
    //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    //glEnableVertexAttribArray(1);



    // Game loop
    
    while (!glfwWindowShouldClose(window))
    {
        float curFrame = static_cast<float>(glfwGetTime());
        deltaTime = curFrame - lastFrame;
        lastFrame = curFrame;

        // Check if any events have been activated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents();
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);


        glBindVertexArray(VAO);

        shader.Use();
        Eigen::Matrix4f projection = getProjectionMatrixTest();
        Eigen::Matrix4f view = camera.GetViewMatrix();
        Eigen::Matrix4f model = Eigen::Matrix4f::Identity();
        shader.setMat4("view", view);
        shader.setMat4("model", model);
        shader.setMat4("projection", projection);

        glDrawElements(GL_TRIANGLES, pointNum, GL_UNSIGNED_INT, 0);
        

        // Swap the screen buffers
        glfwSwapBuffers(window);
    }
    
    // Properly de-allocate all resources once they've outlived their purpose
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &EBO);
    glDeleteBuffers(1, &VBO);

    // Terminate GLFW, clearing any resources allocated by GLFW.
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

int main1() {
    // constructCube();
    // The number type to use for tessellation
    using Coord = double;

    // The index type. Defaults to uint32_t, but you can also pass uint16_t if you know that your
    // data won't have more than 65536 vertices.
    using N = uint32_t;

    // Create array
    using Point = std::array<Coord, 2>;
    std::vector<std::vector<Point>> polygon;

    // Fill polygon structure with actual data. Any winding order works.
    // The first polyline defines the main polygon.
    polygon.push_back({ {100, 0}, {100, 100}, {0, 100}, {0, 0} });
    // Following polylines define holes.
    polygon.push_back({ {75, 25}, {75, 75}, {25, 75}, {25, 25} });

    // Run tessellation
    // Returns array of indices that refer to the vertices of the input polygon.
    // e.g: the index 6 would refer to {25, 75} in this example.
    // Three subsequent indices form a triangle. Output triangles are clockwise.
    std::vector<N> indices = mapbox::earcut<N>(polygon);

    for (int i = 0; i < indices.size(); ++i) {
        std::cout << indices[i] << " ";
    }
    return 0;

}


void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    float newxpos = static_cast<float>(xpos);
    float newypos = static_cast<float>(ypos);
    if (firstMouse) {
        lastX = newxpos;
        lastY = newypos;
        firstMouse = false;
    }

    float xoffset = newxpos - lastX;
    float yoffset = lastY - newypos;

    lastX = newxpos;
    lastY = newypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyBoard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyBoard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyBoard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyBoard(RIGHT, deltaTime);


}

Eigen::Matrix4f getProjectionMatrix() {
    float n = 0.1f, f = 100.0f;
    float height = n * tan(camera.Zoom / 2) * 2, width = WIDTH * 1.0f / HEIGHT * height;
    Eigen::Matrix4f projection = Eigen::Matrix4f::Identity();
    Eigen::Matrix4f scale;
    scale << 2 / width, 0, 0, 0,
        0, 2 / height, 0, 0,
        0, 0, 2 / (f - n), 0,
        0, 0, 0, 1;
    Eigen::Matrix4f translate;
    translate << 1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, -(n + f) / 2,
        0, 0, 0, 1;
    Eigen::Matrix4f ortho;
    ortho << n, 0, 0, 0,
        0, n, 0, 0,
        0, 0, n + f, -n * f,
        0, 0, -1, 0;
    projection = scale * translate * ortho;
    return projection;
}

Eigen::Matrix4f getProjectionMatrixTest() {
    float n = 0.1f, f = 100.0f;
    float height = n * tan(camera.Zoom / 2) * 2, width = WIDTH * 1.0f / HEIGHT * height;
    Eigen::Matrix4f projection;
    projection << 2 * n / width, 0, 0, 0,
        0, 2 * n / height, 0, 0,
        0, 0, (f + n) / (n - f), 2 * f * n / (n - f),
        0, 0, -1, 0;
    //projection << 2 / width, 0, 0, 0,
    //    0, 1 / height, 0, 0,
    //    0, 0, -2 * (f - n), (f + n) / (n - f),
    //    0, 0, 0, 1;

    return projection;
}




