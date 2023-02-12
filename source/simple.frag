#version 330 core

out vec4 FragColor;
in vec3 color;

// uniform vec4 ucolor;

void main()
{
   // FragColor = vec4(vertex_color);
   // FragColor = vec4(ucolor);
   FragColor = vec4(color, 1.0f);
}
