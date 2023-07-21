#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cglm/affine.h>
#include <cglm/cam.h>
#include <cglm/mat4.h>
#include <cglm/vec3.h>

#include "fileops.h"
#include "shader.h"

#define true 1
#define false 0

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

char fops_buffer[1024];

float delta_time = 0.0f; // time between the current frame and the last frame
float last_frame = 0.0f; // time to render the last frame
float angle = 0.0f;

vec3 camera_position = {0};
vec3 camera_forward = {0};
vec3 camera_up = {0};
vec3 camera_right = {0};
vec3 camera_target = {0};
vec3 camera_direction = {0};

mat4 model= {0};
mat4 view = {0};
mat4 proj = {0};

// triangle
float vertices[] = {
  -0.5f, -0.5f, 0.0f,
   0.5f, -0.5f, 0.0f,
   0.0f,  0.5f, 0.0f
};


float svertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

vec3 cubePositions[] = {
    { 0.0f,  0.0f,  0.0f }, 
    { 2.0f,  5.0f, -15.0f}, 
    {-1.5f, -2.2f, -2.5f },  
    {-3.8f, -2.0f, -12.3f},  
    { 2.4f, -0.4f, -3.5f },  
    {-1.7f,  3.0f, -7.5f },  
    { 1.3f, -2.0f, -2.5f },  
    { 1.5f,  2.0f, -2.5f }, 
    { 1.5f,  0.2f, -1.5f }, 
    {-1.3f,  1.0f, -1.5f }  
};
// uint32_t sindices[] = {
//     0, 3, 1,
//     4, 7, 5,
//     6, 2, 7,
//     5, 1, 4,
//     5, 2, 0,
//     1, 6, 4,
//     6, 1, 3,
//     2, 5, 7,
//     1, 5, 0,
//     2, 6, 3,
//     7, 4, 6,
//     3, 0, 2
// };


void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

void input(GLFWwindow *window) {
    const float camera_speed = 3.0f * delta_time;
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        glm_vec3_muladds(camera_forward, camera_speed, camera_position);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        glm_vec3_muladds(camera_forward, -camera_speed, camera_position);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        glm_vec3_muladds(camera_right, camera_speed, camera_position);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        glm_vec3_muladds(camera_right, -camera_speed, camera_position);
    }
}

int main() {
    
    //  INITIALIZATION
    //-------------------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        printf("Failed to create GLFW window.\n");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("Failed to initialize GLAD.\n");
        glfwTerminate();
        return -1;
    }

    int num_attribs;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &num_attribs);
    // printf("Maximum number of attributes: %d.\n", num_attribs);

    //  SHADER
    //-------------------------------------------
    fops_read("resource/simple.vert");
    uint32_t vertex_shader = shader_compile(fops_buffer, GL_VERTEX_SHADER);

    fops_read("resource/simple.frag");
    uint32_t fragment_shader = shader_compile(fops_buffer, GL_FRAGMENT_SHADER);

    uint32_t shader_program = shader_link(vertex_shader, fragment_shader);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    //  TEXTURE
    //-------------------------------------------
    uint32_t texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                      GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int32_t width, height, n_channels;
    uint8_t *data =
        stbi_load("resource/container.jpg", &width, &height, &n_channels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
        GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        printf("failed to load texture.");
    }
    STBI_FREE(data);

    //  GLOBJECTS
    //-------------------------------------------
    uint32_t svao, svbo;
    glGenVertexArrays(1, &svao);
    glGenBuffers(1, &svbo);

    glBindVertexArray(svao);

    glBindBuffer(GL_ARRAY_BUFFER, svbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(svertices), svertices, GL_STATIC_DRAW);

    // pos
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // tex
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(sizeof(float) * 3));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    //  TRANSFORMS
    //-------------------------------------------
    // camera
    camera_position[2] = 3.0f;
    glm_vec3_sub(camera_direction, camera_position, camera_direction);
    glm_vec3_normalize(camera_direction);

    glm_vec3_cross((vec3){0.0f, 1.0f, 0.0f}, camera_direction, camera_right);
    glm_vec3_normalize(camera_right);

    glm_vec3_cross(camera_direction, camera_right, camera_up);

    camera_forward[2] = -1.0f;

    // mesh
    glm_mat4_identity(model);
    glm_mat4_identity(view);
    glm_mat4_identity(proj );
    glm_perspective(glm_rad(45.0f), 800.0f / 600.0f, 0.1f, 100.0f, proj );

    //  LOOPS
    //-------------------------------------------
    while (!glfwWindowShouldClose(window)) {
        float current_frame = (float)glfwGetTime();
        delta_time = current_frame - last_frame;
        last_frame = current_frame;

        // input
        input(window);

        // update
        glm_vec3_dup(camera_position, camera_target);
        glm_vec3_sub(camera_target, (vec3){0.0f, 0.0f, 10.0f}, camera_target);
        glm_lookat(camera_position, camera_target, camera_up, view);

        // render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        uint32_t view_location = glGetUniformLocation(shader_program, "view");
        glUniformMatrix4fv(view_location, 1, GL_FALSE, view[0]);

        uint32_t proj_location = glGetUniformLocation(shader_program, "proj");
        glUniformMatrix4fv(proj_location, 1, GL_FALSE, proj[0]);

        glBindTexture(GL_TEXTURE_2D, texture);

        glUseProgram(shader_program);

        glBindVertexArray(svao);

        for (int i = 0; i < 10; i++) {
            glm_mat4_identity(model);

            glm_translate(model, cubePositions[i]);

            float angle = 20.0f * i;
            glm_rotate(model, angle, (vec3){0.5f, 0.3f, 0.2f});

            uint32_t model_location = glGetUniformLocation(shader_program, "model");
            glUniformMatrix4fv(model_location, 1, GL_FALSE, model[0]);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glBindVertexArray(0);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &svao);
    glDeleteBuffers(1, &svbo);

    glfwTerminate();
    return 0;
}

