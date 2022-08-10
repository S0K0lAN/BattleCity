#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>









int g_windowSizeX = 640;
int g_windowSizeY = 480;


std::vector<GLfloat> get_circle_positions(double pos_x = 0, double pos_y = 0, int smooth = 10, double radius = 0.3 ) {
    // smooth - number of dots in circle
    // coordinates must be the same as in OpenGL
    // smooth must be number of x-coordinates

    std::vector<GLfloat> verts((2 * smooth) * 3);
    int i = 0;
    for (double x = -radius + pos_x; x <= radius + pos_x; x += 2 * radius / smooth) {
        float y1 = pos_y + sqrt(pow(radius, 2) - pow(x, 2) + 2 * pos_x * x - pow(pos_x, 2));
        float y2 = pos_y - sqrt(pow(radius, 2) - pow(x, 2) + 2 * pos_x * x - pow(pos_x, 2));

        verts[i] = x;
        verts[i + 1] = y1;
        verts[i + 2] = 0;
        verts[verts.size() - 3 - i] = x;
        verts[verts.size() - 2 - i] =  y2;
        verts[verts.size() - 1 - i] = 0;

        

        i += 3;
    }

    return verts;
}



GLfloat points[] = {
    0.0f, 0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f
};


GLfloat colors[] = {
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f
};


const char* vertex_shader =
"#version 460\n"
"layout(location = 0) in vec3 vertex_position;"
"layout(location = 1) in vec3 vertex_color;"
"out vec3 color;"
"void main() {"
"   color = vertex_color;"
"   gl_Position = vec4(vertex_position, 1.0);"
"}";

const char* fragment_shader =
"#version 460"
"in vec3 color;"
"out vec4 frag_color;"
"void main() {"
"   frag_color = vec4(color, 1);"
"}";






void glfwWindowSizeCallback(GLFWwindow* pWindow, int width, int height)
{
    g_windowSizeX = width;
    g_windowSizeY = height;
    glViewport(0, 0, g_windowSizeX, g_windowSizeY);
}

void glfwKeyCallback(GLFWwindow* pWindow, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) 
    {
        glfwSetWindowShouldClose(pWindow, GL_TRUE);
    }
}

int main(void)
{

    std::vector<GLfloat> verts = get_circle_positions();

    for (int i = 0; i < verts.size() - 1; i += 3)
        std::cout << verts[i] << " " << verts[i + 1] << " " << verts[i + 2] << std::endl;











    /* Initialize the library */
    if (!glfwInit())
    {
        std::cout << "glfwInit failed!" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow* pWindow = glfwCreateWindow(g_windowSizeX, g_windowSizeY, "Battle City", nullptr, nullptr);
    if (!pWindow)
    {
        std::cout << "glfwCreateWindow failed!" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwSetWindowSizeCallback(pWindow, glfwWindowSizeCallback);
    glfwSetKeyCallback(pWindow, glfwKeyCallback);

    /* Make the window's context current */
    glfwMakeContextCurrent(pWindow);

    if(!gladLoadGL()) 
    {
        std::cout << "Can't load GLAD" << std::endl;
        return -1;
    }

    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

    glClearColor(0, 0, 1, 1);


    //Creation of vertex and fragment shader 
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertex_shader, nullptr);
    glCompileShader(vs);

    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragment_shader, nullptr);
    glCompileShader(fs);

    GLuint shader_program = glCreateProgram();
    glAttachShader(shader_program, vs);
    glAttachShader(shader_program, fs);
    glLineWidth(shader_program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    GLuint points_vbo = 0;
    glGenBuffers(1, &points_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

    GLuint colors_vbo = 0;
    glGenBuffers(1, &colors_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);



    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(pWindow))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        /* Swap front and back buffers */
        glfwSwapBuffers(pWindow);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}