#define GLAD_GL_IMPLEMENTATION
#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <iostream>

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/gl3ext.h>
#endif

// #include "linmath.h"
#include <stdlib.h>
#include <stddef.h>
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>
// #include <glm/gtc/matrix_transform.hpp>

#include "utils/utils.hh"
#include "utils/errorhandler.hh"

using namespace std;
using namespace glm;

typedef struct Vertex
{
    vec2 pos;
    vec3 col;
} Vertex;
 
static const Vertex vertices[3] =
{
    { vec2(-0.6f, -0.4f), vec3(1.f, 0.f, 0.f) },
    { vec2(0.6f, -0.4f), vec3(0.f, 1.f, 0.f) },
    { vec2(0.f,  0.6f), vec3(0.f, 0.f, 1.f) }
};
 
static const char* vertex_shader_text =
"#version 330\n"
"uniform mat4 MVP;\n"
"in vec3 vCol;\n"
"in vec2 vPos;\n"
"out vec3 color;\n"
"void main()\n"
"{\n"
"    gl_Position = MVP * vec4(vPos, 0.0, 1.0);\n"
"    color = vCol;\n"
"}\n";
 
static const char* fragment_shader_text =
"#version 330\n"
"in vec3 color;\n"
"out vec4 fragment;\n"
"void main()\n"
"{\n"
"    fragment = vec4(color, 1.0);\n"
"}\n";
 
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

// tutorial from: https://www.glfw.org/docs/latest/quick_guide.html

int main() {
    glfwSetErrorCallback(error_callback);

    // Initialize glfw
    if (!glfwInit())
    {
        // Initialization failed
        cerr << "Anh: Failed to initialize GLFW!" << endl;
    }
    GLFWwindow* window = glfwCreateWindow(640, 480, "My Title", NULL, NULL);
    if (!window)
    {
        // Window or OpenGL context creation failed
        cerr << "Anh: Failed to create Window or OpenGL context!" << endl;
    }
    glfwSetKeyCallback(window, key_callback);
    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glfwSwapInterval(1); // avoid screen tearing
    // --------------------------------------------

    //triangle stuff______________________
    // NOTE: OpenGL error checks have been omitted for brevity
    GLuint vertex_buffer;
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
 
    const GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    if (!vertex_shader) {
        cerr << "Anh: Could not generate vertex shader object!" << endl;
    }
    glShaderSource(vertex_shader, 1, &vertex_shader_text, NULL);
    glCompileShader(vertex_shader);
 
    const GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    if (!fragment_shader) {
        cerr << "Anh: Could not generate fragment shader object!" << endl;
    }
    glShaderSource(fragment_shader, 1, &fragment_shader_text, NULL);
    glCompileShader(fragment_shader);
 
    const GLuint program = glCreateProgram();
    if (!program) {
        cerr << "Anh: Could not generate program object!" << endl;
    }
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    ErrorHandler::glCheckError();
 
    const GLint mvp_location = glGetUniformLocation(program, "MVP");
    const GLint vpos_location = glGetAttribLocation(program, "vPos");
    const GLint vcol_location = glGetAttribLocation(program, "vCol");
    if (mvp_location == -1) {
        cerr << "Anh: mvp_location Invalid names!" << endl;
    }
    if (vpos_location == -1) {
        cerr << "Anh: vpos_location Invalid names!" << endl;
    }
    if (vcol_location == -1) {
        cerr << "Anh: vcol_location Invalid names!" << endl;
    }
 
    GLuint vertex_array;
    glGenVertexArrays(1, &vertex_array);
    glBindVertexArray(vertex_array);
    glEnableVertexAttribArray(vpos_location);
    glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE,
                          sizeof(Vertex), (void*) offsetof(Vertex, pos));
    glEnableVertexAttribArray(vcol_location);
    glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE,
                          sizeof(Vertex), (void*) offsetof(Vertex, col));
    //_____________________ triangle stuff

    glClearColor(1.f, 0.f, 0.f, 1.f);

    cout << "Start creating window!" << endl;
    // engine logic
    while (!glfwWindowShouldClose(window))
    {
        // cout << "In loop!" << endl;
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        const float ratio = width / (float) height;

        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);
        
        mat4x4 m(1.f), p, mvp;
        // mat4x4_rotate_Z(m, m, (float) glfwGetTime());
        m = rotate(m, (float) glfwGetTime(), vec3(0.f, 0.f, 1.f));
        // mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
        p = ortho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
        // mat4x4_mul(mvp, p, m);
        mvp = p * m;
 
        glUseProgram(program);
        glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*) &mvp);
        glBindVertexArray(vertex_array);
        glDrawArrays(GL_TRIANGLES, 0, 3);
 
        glfwSwapBuffers(window); // swap front & back buffers
        glfwPollEvents(); // to poll events in events queue & process them immediately
    }
    cout << "Out of loop!" << endl;

    // --------------------------------------------
    // Deinitialize glfw
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}