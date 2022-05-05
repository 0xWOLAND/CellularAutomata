#version 330 core
out vec4 FragColor;
in vec4 pos;
in vec2 texPos;

uniform sampler2D pixels;
uniform mat4 camMatrix;
uniform float width;
uniform vec3 cameraPosition;
uniform vec2 u_resolution;

vec4 BACKGROUND = {0.82745098039f, 0.82745098039f, 0.82745098039f, 1.0f};
vec4 BLACK = {0.0f, 0.0f, 0.0f, 1.0f};
vec4 WHITE = {1.0f, 1.0f, 1.0f, 1.0f};
vec2 fc = gl_FragCoord.xy/u_resolution;
float gwidth = 0.5f;
float step = 0.01f;

struct Ray{
	float x;
	float y;
	float z;
	bool isHit;
};

vec3 toVec3(Ray r){
	return vec3(r.x, r.y, r.z);
}

float mag(Ray r){
	return sqrt(
				pow(r.x, 2)
			  + pow(r.y, 2)
			  + pow(r.z, 2)
			  );
}
float mag(vec3 r){
	return sqrt(
				pow(r.x, 2)
			  + pow(r.y, 2)
			  + pow(r.z, 2)
			  );
}

float mag(vec2 r){
	return sqrt(
				pow(r.x, 2)
				+ pow(r.y, 2)
				);
}

vec3 normalize(vec3 v){
	return v / mag(v);
}

vec2 normalize(vec2 v){
	return v / mag(v);
}



bool inCube(vec3 v, float cube){
	return  -cube <= v.x && v.x <= cube && 
			-cube <= v.y && v.y <= cube && 
			-cube <= v.z && v.z <= cube;
}

Ray divide(Ray r, float d){
	Ray ans = Ray(r.x / d, r.y / d, r.z / d, r.isHit);
	return ans;
}

Ray invert(Ray r){
	r.x *= -1;
	r.y *= -1;
	r.z *= -1;
	return r;
}

bool isFilled(vec4 v){
	float th = 0.005;

	return v.x > th && v.y > th && v.z > th;
}

bool check(int i, int j, int k){
	return isFilled(texelFetch(pixels, ivec2(i, floor(width) * j + k), 0));
}

int clamp(float val, int limit){
	
	float threshold = 0.01f;
	if(val < threshold) return 0;
	else if(val > float(limit) - threshold) return limit - 1;
	int ans = int(ceil(val)) * limit;
	if(val > 0.5) return 0;
	return min(limit, ans);
}

bool getPosition(vec3 v){
	int i = clamp(v.x, int(width));
	int j = clamp(v.y, int(width));
	int k = clamp(v.z, int(width));

	if(i == 0) return true;

	return check(i,j,k);
	
}


vec4 RayMarching(vec2 crd, float st){
	vec2 coord = normalize(crd);
	vec3 cp = normalize(vec3(cameraPosition.x, cameraPosition.y, cameraPosition.z));
	Ray r = Ray(cp.x - coord.x,
		  cp.y - coord.y,
		  cp.z,
		  false);
	r = invert(r);
	float current_step = 0.0f;
	bool hit = false;
	// CameraPosition: (0,0,1)
	vec3 projectedRay = cameraPosition + toVec3(r) * current_step; 
	while(inCube(projectedRay, width) && current_step < 100.0f){
		if(getPosition(projectedRay)){
			r.isHit = true;
		}
		current_step += 0.01f;
		projectedRay = cameraPosition + toVec3(r) * st; 
	}
	if(r.isHit){
		return vec4(1.0f, 1.0f, 1.0f, 1.0f);
	}
	return vec4(r.x, r.y, r.z, 1.0f);
}


void main()
{
	if(-gwidth < pos.x && pos.x < gwidth && -gwidth < pos.y && pos.y < gwidth){
		//FragColor = vec4(texture(pixels, texPos));
		vec2 coord = vec2(gl_FragCoord.x, gl_FragCoord.y) / u_resolution;
		FragColor = RayMarching(coord, 0.001f);
	}
	else{
		FragColor = BACKGROUND;
	}

		

}