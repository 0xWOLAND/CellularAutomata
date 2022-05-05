#version 330 core
out vec4 FragColor;
in vec4 pos;
in vec2 texPos;

uniform sampler2D pixels;
uniform mat4 camMatrix;
uniform float width;
uniform vec3 cameraPosition;
uniform vec2 u_resolution;

vec2 fc = gl_FragCoord.xy/u_resolution;

bool isFilled(vec4 v){
	float th = 0.005;

	return v.x > th && v.y > th && v.z > th;
}

bool check(int i, int j, int k){
	return isFilled(texelFetch(pixels, ivec(i, (int)(width) * j + k)));
}



void main()
{
	if(-0.5f < pos.x && pos.x < 0.5f && -0.5f < pos.y && pos.y < 0.5f){
		//FragColor = vec4(texture(pixels, texPos));
		
		vec4 val = texelFetch(pixels, ivec2(2, 1), 0);
		FragColor = val;
	}
	else{
		FragColor = vec4(0.82745098039f, 0.82745098039f, 0.82745098039f, 1.0f);
	}

		

}