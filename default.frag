#version 330 core
out vec4 FragColor;
in vec4 pos;
in vec2 texPos;

uniform sampler2D pixels;

void main()
{
   // FragColor = vec4(pos.x, pos.y, pos.z, 1.0f);
	if(-0.5f < pos.x && pos.x < 0.5f && -0.5f < pos.y && pos.y < 0.5f){
		FragColor = vec4(texPos.x, texPos.y, 0.0f, 1.0f);
	}
	else{
		//FragColor = vec4(texture(pixels, texPos));
		
		vec4 val = texelFetch(pixels, ivec2(1, 1), 0);
		FragColor = vec4(val.x, 0.0f, 0.0f, 1.0f);
	}

		

}