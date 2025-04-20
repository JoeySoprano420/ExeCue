#include <iostream>
#include <vector>
#include <memory>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Error handling for OpenGL
void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
                                GLsizei length, const GLchar *message, const void *userParam) {
    std::cerr << "GL Error: " << message << std::endl;
}

// Shader class to load and compile GLSL shaders
class Shader {
public:
    GLuint programID;
    Shader(const std::string& vertexSource, const std::string& fragmentSource) {
        GLuint vertexShader = compileShader(vertexSource, GL_VERTEX_SHADER);
        GLuint fragmentShader = compileShader(fragmentSource, GL_FRAGMENT_SHADER);
        programID = linkProgram(vertexShader, fragmentShader);
    }

    void use() {
        glUseProgram(programID);
    }

    GLuint compileShader(const std::string& source, GLenum shaderType) {
        GLuint shader = glCreateShader(shaderType);
        const char* src = source.c_str();
        glShaderSource(shader, 1, &src, nullptr);
        glCompileShader(shader);
        checkShaderCompile(shader);
        return shader;
    }

    GLuint linkProgram(GLuint vertexShader, GLuint fragmentShader) {
        GLuint program = glCreateProgram();
        glAttachShader(program, vertexShader);
        glAttachShader(program, fragmentShader);
        glLinkProgram(program);
        checkProgramLink(program);
        return program;
    }

    void checkShaderCompile(GLuint shader) {
        GLint success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            char infoLog[512];
            glGetShaderInfoLog(shader, 512, nullptr, infoLog);
            std::cerr << "Shader Compilation Failed: " << infoLog << std::endl;
        }
    }

    void checkProgramLink(GLuint program) {
        GLint success;
        glGetProgramiv(program, GL_LINK_STATUS, &success);
        if (!success) {
            char infoLog[512];
            glGetProgramInfoLog(program, 512, nullptr, infoLog);
            std::cerr << "Program Linking Failed: " << infoLog << std::endl;
        }
    }
};

// Vertex and Index Buffer Object Class for managing mesh data
class Mesh {
public:
    GLuint VAO, VBO, EBO;
    std::vector<GLfloat> vertices;
    std::vector<GLuint> indices;

    Mesh(const std::vector<GLfloat>& vertices, const std::vector<GLuint>& indices) {
        this->vertices = vertices;
        this->indices = indices;
        setupMesh();
    }

    void setupMesh() {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void draw() {
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
};

// Camera Class for controlling the view matrix and projection
class Camera {
public:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::mat4 projection;

    Camera(glm::vec3 pos, glm::vec3 f, glm::vec3 u) : position(pos), front(f), up(u) {
        projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
    }

    glm::mat4 getViewMatrix() {
        return glm::lookAt(position, position + front, up);
    }

    glm::mat4 getProjectionMatrix() {
        return projection;
    }
};

// Render System for managing overall graphics rendering
class RenderSystem {
public:
    GLFWwindow* window;
    Shader* shader;
    Mesh* mesh;
    Camera* camera;
    bool isRunning;

    RenderSystem() : window(nullptr), shader(nullptr), mesh(nullptr), camera(nullptr), isRunning(true) {}

    bool initialize() {
        if (!glfwInit()) {
            std::cerr << "GLFW Initialization Failed!" << std::endl;
            return false;
        }

        window = glfwCreateWindow(800, 600, "EXECUE Render", nullptr, nullptr);
        if (!window) {
            std::cerr << "Window Creation Failed!" << std::endl;
            glfwTerminate();
            return false;
        }

        glfwMakeContextCurrent(window);
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
        glewInit();

        glEnable(GL_DEPTH_TEST);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

        // Create shader program (basic vertex and fragment shader)
        const std::string vertexShader = R"(
            #version 330 core
            layout(location = 0) in vec3 position;
            uniform mat4 model;
            uniform mat4 view;
            uniform mat4 projection;
            void main() {
                gl_Position = projection * view * model * vec4(position, 1.0f);
            }
        )";

        const std::string fragmentShader = R"(
            #version 330 core
            out vec4 color;
            void main() {
                color = vec4(0.7f, 0.7f, 0.7f, 1.0f);
            }
        )";

        shader = new Shader(vertexShader, fragmentShader);

        // Define a basic cube mesh
        std::vector<GLfloat> vertices = {
            -0.5f, -0.5f, -0.5f, // Front-bottom-left
            0.5f, -0.5f, -0.5f,  // Front-bottom-right
            0.5f,  0.5f, -0.5f,  // Front-top-right
            -0.5f,  0.5f, -0.5f, // Front-top-left
            -0.5f, -0.5f,  0.5f, // Back-bottom-left
            0.5f, -0.5f,  0.5f,  // Back-bottom-right
            0.5f,  0.5f,  0.5f,  // Back-top-right
            -0.5f,  0.5f,  0.5f  // Back-top-left
        };

        std::vector<GLuint> indices = {
            0, 1, 2, 0, 2, 3, // Front face
            4, 5, 6, 4, 6, 7, // Back face
            0, 1, 5, 0, 5, 4, // Left face
            1, 2, 6, 1, 6, 5, // Right face
            2, 3, 7, 2, 7, 6, // Top face
            0, 3, 7, 0, 7, 4  // Bottom face
        };

        mesh = new Mesh(vertices, indices);

        // Initialize camera
        camera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));

        return true;
    }

    void run() {
        while (!glfwWindowShouldClose(window) && isRunning) {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // Use the shader program
            shader->use();

            // Set transformation matrices
            glm::mat4 model = glm::mat4(1.0f);
            glm::mat4 view = camera->getViewMatrix();
            glm::mat4 projection = camera->getProjectionMatrix();

            GLint modelLoc = glGetUniformLocation(shader->programID, "model");
            GLint viewLoc = glGetUniformLocation(shader->programID, "view");
            GLint projLoc = glGetUniformLocation(shader->programID, "projection");

            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
            glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

            // Draw the mesh (cube)
            mesh->draw();

            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }

    void terminate() {
        delete shader;
        delete mesh;
        delete camera;
        glfwTerminate();
    }

    static void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);
    }
};

// Main function
int main() {
    RenderSystem renderSystem;
    if (!renderSystem.initialize()) {
        return -1;
    }

    renderSystem.run();
    renderSystem.terminate();
    return 0;
}
