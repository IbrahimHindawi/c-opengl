#include "shader.h"

void shader_compile_check(uint32_t shader) {
    int success;
    char info_log[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, info_log);
        printf("shader compile error:\n%s", info_log);
    }
}

void shader_link_check(uint32_t shader) {
    int success;
    char info_log[512];
    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shader, 512, NULL, info_log);
        printf("shader compile error:\n%s", info_log);
    }
}

uint32_t shader_compile(const char* shader_source, GLenum shader_type) {
    uint32_t shader = glCreateShader(shader_type);
    glShaderSource(shader, 1, &shader_source, NULL);
    glCompileShader(shader);
    shader_compile_check(shader);
    return shader;
}

uint32_t shader_link(uint32_t vertex_shader, uint32_t fragment_shader) {
    uint32_t shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);
    shader_link_check(shader_program);
    return shader_program;
}

