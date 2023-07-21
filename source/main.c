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
     0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right
     0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right
    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
    -0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f  // top left
};

uint32_t sindices[] = {
    0, 1, 3, // first triangle
    1, 2, 3  // second triangle
};

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

void input(GLFWwindow *window) {
    const float camera_speed = 10.0f * delta_time;
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

<<<<<<< HEAD
    int num_attribs;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &num_attribs);
    // printf("Maximum number of attributes: %d.\n", num_attribs);
    /***********************************************************************************
    *                              SHADER                                             *
    ***********************************************************************************/
    fops_read("resource/simple.vert");
    uint32_t vertex_shader = shader_compile(fops_buffer, GL_VERTEX_SHADER);
=======
  // bla bla bla
  glDeleteVertexArrays(1, &svao);
  glDeleteBuffers(1, &svbo);
  glDeleteBuffers(1, &sebo);
>>>>>>> 21540d806ed0b740428b5c4295cfa1511e7e3865

    fops_read("resource/simple.frag");
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
    /***********************************************************************************
    *                              GLOBJECTS                                          *
    ***********************************************************************************/

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
    camera_position[2] = 3.0f;
    glm_vec3_sub(camera_direction, camera_position, camera_direction);
    glm_vec3_normalize(camera_direction);

    glm_vec3_cross((vec3){0.0f, 1.0f, 0.0f}, camera_direction, camera_right);
    glm_vec3_normalize(camera_right);

    glm_vec3_cross(camera_direction, camera_right, camera_up);

    camera_forward[2] = -1.0f;

    glm_mat4_identity(model);
    glm_mat4_identity(view);
    glm_mat4_identity(proj );
    glm_perspective(glm_rad(45.0f), 800.0f / 600.0f, 0.1f, 100.0f, proj );

    /***********************************************************************************
     *                              LOOP                                               *
     ***********************************************************************************/
    while (!glfwWindowShouldClose(window)) {
        float current_frame = (float)glfwGetTime();
        delta_time = current_frame - last_frame;
        last_frame = current_frame;

        // input
        // angle += 0.0001f;
        input(window);

        // update
        glm_vec3_dup(camera_position, camera_target);
        glm_vec3_sub(camera_target, (vec3){0.0f, 0.0f, 10.0f}, camera_target);
        glm_lookat(camera_position, camera_target, camera_up, view);

        // render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        uint32_t view_location = glGetUniformLocation(shader_program, "view");
        glUniformMatrix4fv(view_location, 1, GL_FALSE, view[0]);

        uint32_t proj_location = glGetUniformLocation(shader_program, "proj");
        glUniformMatrix4fv(proj_location, 1, GL_FALSE, proj[0]);

        glUseProgram(shader_program);

        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(svao);
        glm_mat4_identity(model);
        vec3 axis = {1.0f, 0.0f, 0.0f};
        vec3 trans = {0.0f, 0.0f, -3.0f};
        glm_translate(model, trans);
        uint32_t model_location = glGetUniformLocation(shader_program, "model");
        glUniformMatrix4fv(model_location, 1, GL_FALSE, model[0]);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &svao);
    glDeleteBuffers(1, &svbo);
    glDeleteBuffers(1, &sebo);

    glfwTerminate();
    return 0;
}

