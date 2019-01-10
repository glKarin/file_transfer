#version 100

// vertex
attribute vec4 a_Vertex;
attribute vec3 a_Normal;
attribute vec4 a_Color;
attribute vec4 a_Texcoord;
//attribute vec4 a_Texcoord2;

// matrix
uniform mat4 u_ModelviewMatrix;
uniform mat4 u_ProjectionMatrix;
uniform mat3 u_NormalMatrix;
uniform mat4 u_ModelviewProjectionMatrix;

uniform vec4 u_Color;
uniform float u_PointSize;

// out
varying vec4 v_Color;
varying vec2 v_Texcoord;
//varying vec2 v_Texcoord2;

void main()
{
	// return
	gl_Position = u_ModelviewProjectionMatrix * a_Vertex;
	v_Color = a_Color * u_Color;
	v_Texcoord = a_Texcoord.st;
	//v_Texcoord2 = a_Texcoord2.st;
	gl_PointSize = u_PointSize;
}
