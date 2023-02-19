#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cglm/vec3.h>
#include <cglm/mat4.h>
#include <cglm/affine.h>
#include <cglm/cam.h>

#include "shader.h"
#include "fileops.h"

#define true 1
#define false 0

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


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

void input(GLFWwindow *window) {
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

    /***********************************************************************************
     *                              SHADER                                             *
     ***********************************************************************************/
    fops_read("source/simple.vert");
    uint32_t vertex_shader = shader_compile(fops_buffer, GL_VERTEX_SHADER);

    fops_read("source/simple.frag");
    uint32_t fragment_shader = shader_compile(fops_buffer, GL_FRAGMENT_SHADER);

    uint32_t shader_program = shader_link(vertex_shader, fragment_shader);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);


    /***********************************************************************************
     *                              TEXTURE                                            *
     ***********************************************************************************/
    uint32_t texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    uint32_t width, height, n_channels;
    uint8_t *data = stbi_load("source/container.jpg", &width, &height, &n_channels, 0);
    if(data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        printf("failed to load texture.");
    }
    STBI_FREE(data);

    /***********************************************************************************
     *                              GLOBJECTS                                          *
     ***********************************************************************************/
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
    
    /***********************************************************************************
     *                              TRANSFORMS                                         *
     ***********************************************************************************/
    float angle = 0.0f;

    // mat4 trans;
    // glm_mat4_identity(trans);
    // trans[0][0] *= 1.5f;
    // trans[1][1] *= 0.5f;
    // trans[2][2] *= 0.5f;

    mat4 model;
    glm_mat4_identity(model);
    vec3 axis = {1.0f, 0.0f, 0.0f};
    glm_rotate(model, glm_rad(-55.0f), axis);

    mat4 view;
    glm_mat4_identity(view);
    vec3 pos = {0.0f, 0.0f, -3.0f};
    glm_translate(view, pos);

    mat4 projection;
    glm_mat4_identity(projection);
    glm_perspective(glm_rad(45.0f), 800.0f / 600.0f, 0.1f, 100.0f, projection);

    while(!glfwWindowShouldClose(window)) {
        angle += 0.0001f;
        input(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
         
        // glUseProgram(shader_program);
        // glBindVertexArray(vao);
        // glDrawArrays(GL_TRIANGLES, 0, 3);
        // glBindVertexArray(0);

        // float time_value = glfwGetTime();
        // float green_value = (sin(time_value) / 2.0f) + 0.5f;
        // int vertex_color_location = glGetUniformLocation(shader_program, "ucolor");

        // uint32_t transform_location = glGetUniformLocation(shader_program, "Transform");
        // glUniformMatrix4fv(transform_location, 1, GL_FALSE, trans[0]);

        uint32_t model_location = glGetUniformLocation(shader_program, "model");
        glUniformMatrix4fv(model_location, 1, GL_FALSE, model[0]);

        uint32_t view_location = glGetUniformLocation(shader_program, "view");
        glUniformMatrix4fv(view_location, 1, GL_FALSE, view[0]);

        uint32_t projection_location = glGetUniformLocation(shader_program, "projection");
        glUniformMatrix4fv(projection_location, 1, GL_FALSE, projection[0]);
        
        glUseProgram(shader_program);

        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(svao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);

    glDeleteVertexArrays(1, &svao);
    glDeleteBuffers(1, &svbo);
    glDeleteBuffers(1, &sebo);

    glfwTerminate();
    return 0;
}
