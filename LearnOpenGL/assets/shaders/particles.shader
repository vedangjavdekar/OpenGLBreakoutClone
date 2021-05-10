#shader vertex
#version 330 core
layout(location = 0) in vec4 vertexData; // <vec2 position, vec2 texCoords>
layout(location = 1) in vec4 vertexColor; // Color

out vec2 TexCoords;
out vec4 VertexColor;

uniform mat4 view;
uniform mat4 projection;

void main()
{
    TexCoords = vertexData.zw;
    VertexColor = vertexColor;
    gl_Position = projection * view * vec4(vertexData.xy, 0.0, 1.0);
}

#shader fragment
#version 330 core
in vec2 TexCoords;
in vec4 VertexColor;
out vec4 color;

uniform sampler2D image;

void main()
{
    color = VertexColor * texture(image, TexCoords);
}