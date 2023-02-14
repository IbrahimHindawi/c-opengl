#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader.h"

#define true 1
#define false 0
#define shader_source_buffer_size 512

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

char shader_source_buffer[shader_source_buffer_size];

// triangle
float vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
};

float svertices[] = {
     0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,  // top right
     0.5f, -0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,  // bottom right
    -0.5f, -0.5f,  0.0f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,  // bottom left
    -0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f   // top left 
};

uint32_t sindices[] = {
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
};  

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

void process_input(GLFWwindow *window) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);   
    }
}    

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        printf("Failed to create GLFW window.\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("Failed to initialize GLAD.\n");
        glfwTerminate();
        return -1;
    }

    int num_attribs;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &num_attribs);
    // printf("Maximum number of attributes: %d.\n", num_attribs);

    FILE *fileptr;
    fileptr = fopen("C:/devel/c-opengl/source/simple.vert", "r");
    if (fileptr != NULL) {
        size_t newlen = fread(shader_source_buffer, sizeof(char), shader_source_buffer_size, fileptr);
        if (ferror(fileptr) != 0){
            fputs("error reading file!", stderr);
        } else {
            shader_source_buffer[newlen++] = '\0';
        }
    }
    fclose(fileptr);
    uint32_t vertex_shader = shader_compile(shader_source_buffer, GL_VERTEX_SHADER);

    fileptr = fopen("C:/devel/c-opengl/source/simple.frag", "r");
    if (fileptr != NULL) {
        size_t newlen = fread(shader_source_buffer, sizeof(char), shader_source_buffer_size, fileptr);
        if (ferror(fileptr) != 0){
            fputs("error reading file!", stderr);
        } else {
            shader_source_buffer[newlen++] = '\0';
        }
    }
    fclose(fileptr);
    uint32_t fragment_shader = shader_compile(shader_source_buffer, GL_FRAGMENT_SHADER);

    uint32_t shader_program = shader_link(vertex_shader, fragment_shader);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    uint32_t vao, vbo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    uint32_t texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // set texture parms
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    uint32_t width, height, n_channels;
    uint8_t *data = stbi_load("C:/devel/c-opengl/source/wall.jpg", &width, &height, &n_channels, 0);
    if(data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        printf("failed to load texture.");
    }
    //stbi_free(data);
    free(data);

    uint32_t svao, svbo, sebo;
    glGenVertexArrays(1, &svao);
    glGenBuffers(1, &svbo);
    glGenBuffers(1, &sebo);

    glBindVertexArray(svao);

    glBindBuffer(GL_ARRAY_BUFFER, svbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(svertices), svertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(sindices), sindices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    while(!glfwWindowShouldClose(window)) {
        process_input(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
         
        // glUseProgram(shader_program);
        // glBindVertexArray(vao);
        // glDrawArrays(GL_TRIANGLES, 0, 3);
        // glBindVertexArray(0);

        // float time_value = glfwGetTime();
        // float green_value = (sin(time_value) / 2.0f) + 0.5f;
        // int vertex_color_location = glGetUniformLocation(shader_program, "ucolor");

        glUseProgram(shader_program);
        // glUniform4f(vertex_color_location, 0.0f, green_value, 0.0f, 1.0f);

        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(svao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }
    glViewport(0, 0, 800, 600);
    glfwTerminate();
    return 0;
}
