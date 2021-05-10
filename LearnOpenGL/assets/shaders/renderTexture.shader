#shader vertex
#version 330 core
layout(location = 0) in vec4 vertex; // <vec2 position, vec2 texCoords>

out vec2 TexCoords;

void main()
{
    TexCoords = vertex.zw;
    gl_Position = vec4(vertex.xy, 0.0, 1.0);
}

#shader fragment
#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D image;

void main()
{
    //Inverted Color 
    color = vec4(vec3(1.0 - texture(image, TexCoords)), 1.0);
    // Normal Color
    //color = vec4(vec3(texture(image, TexCoords)),1.0);
}