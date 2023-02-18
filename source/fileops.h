#ifndef FILEOPS_H
#define FILEOPS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <glad/glad.h>

#define fops_buffer_size 1024
char fops_buffer[fops_buffer_size];

void fops_read(const char *file_path);

#endif FILEOPS_H
