// Vertex shader
#version 420

//uniform mat4 MVP;
uniform mat4 matView;
uniform mat4 matProjection;
uniform vec4 vVertexColor;

out vec4 fVertexColor; // specify a color output to the fragment shader

layout (location = 0) in vec4 vVertexPosition;

void main()
{
	mat4 MVP = matProjection * matView;

	vec4 vertPosition = vVertexPosition;
	vertPosition.w = 1.f;

	fVertexColor = vVertexColor;

	gl_Position = MVP * vertPosition;
}