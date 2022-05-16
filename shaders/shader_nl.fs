#version 330 core
in vec4 color;
in vec3 position;
uniform bool is_wire;
uniform float side_len;
void main() 
{
    float x = position.x;
    float y = position.y;
    float z = position.z;
    // float r = sqrt((x-10.5)*(x-10.5) + (y-10.5)*(y-10.5) + (z-10.5)*(z-10.5));
    float red = position.x/side_len;
    float green = position.y/side_len;
    float blue = position.z/side_len;
    float range2 = 0.5f;
    red = (red*range2);
    green = (green*range2);
    blue = (blue*range2);
    // r /= 25.0f;
    gl_FragColor = vec4(0.0); // white color
}