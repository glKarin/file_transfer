#version 100

precision mediump float;

// texture
uniform sampler2D u_Texture2D;
//uniform texture2D u_Texture2D2;

// in
varying vec4 v_Color;
varying vec2 v_Texcoord;
//varying vec2 v_Texcoord2;

void main()
{
	// return
	//gl_FragColor = texture2D(u_Texture2D, v_Texcoord) * v_Color;
	gl_FragColor = v_Color; //vec4(1.0,0.0,0.0,1.0);
}
