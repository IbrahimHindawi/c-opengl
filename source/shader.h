#ifndef SHADER_H
#define SHADER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <glad/glad.h>

void shader_compile_check(uint32_t shader); 
void shader_link_check(uint32_t shader); 

uint32_t shader_compile(const char* shader_source, GLenum shader_type);
uint32_t shader_link(uint32_t vertex_shader, uint32_t fragment_shader);

#endif
