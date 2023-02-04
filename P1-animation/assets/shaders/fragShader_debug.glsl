// Fragment shader
#version 420

out vec4 pixelColour;

in vec4 fVertexColor; // the input variable from the vertex shader (same name and same type)  

void main()
{
    pixelColour = fVertexColor;
} 