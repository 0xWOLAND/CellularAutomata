#version 330 core
out vec4 FragColor;
in vec4 pos;
in vec2 texPos;

uniform sampler2D pixels;
uniform mat4 camMatrix;
uniform float width;
uniform vec3 cameraPosition;
uniform vec2 u_resolution;

vec3 BACKGROUND = {0.82745098039f, 0.82745098039f, 0.82745098039f};
vec3 BLACK      = {0.0f, 0.0f, 0.0f};
vec3 WHITE      = {1.0f, 1.0f, 1.0f};
vec3 RED        = {1.0f, 0.0f, 0.0f};
vec3 GREEN      = {0.0f, 1.0f, 0.0f};
vec3 BLUE       = {0.0f, 0.0f, 1.0f};
vec3 YELLOW     = {1.0f, 1.0f, 0.0f};

// COLOR SCHEME
vec4 P_GREEN =   vec4(105.0f, 165.0f, 131.0f, 1.0f) / 255.0f;
vec4 P_ORANGE =  vec4(225.0f, 179.0f, 120.0f, 1.0f) / 255.0f;
vec4 P_YELLOW =  vec4(224.0f, 204.0f, 111.0f, 1.0f) / 255.0f;
vec4 P_PINK =    vec4(236.0f, 121.0f, 154.0f, 1.0f) / 255.0f;
vec4 P_RED =     vec4(159.0f, 2.0f,    81.0f, 1.0f) / 255.0f;

const float pos_infinity = uintBitsToFloat(0x7F800000);
const float neg_infinity = uintBitsToFloat(0xFF800000);

struct Ray{
	vec3 origin;
	vec3 direction;
};


bool inRange(float k, float limit){
	return -limit <= k && k <= limit;
}

bool inBounds(vec3 v, float limit){
	return (inRange(v.x, limit) && inRange(v.y, limit) && inRange(v.z, limit));
}
bool isFilled(vec4 v){
	float th = 0.005;

	return v.x + v.y + v.z > th;
}

vec4 getVoxel(int i, int j, int k){
	return texelFetch(pixels, ivec2(i, floor(width) * k + j), 0);
}

bool check(int i, int j, int k){
	return isFilled(getVoxel(i,j,k));
}

bool check(vec3 v){
	return isFilled(getVoxel(int(v.x), int(v.y), int(v.z)));
}
float boundary = 20.0f;
float d = 10.0f;
vec3 COI = vec3(width/2, width/2, width/2 + d);
vec3 eye = cameraPosition;
// [TODO] View-up
vec3 v = vec3(vec4(0.0, 1.0f, 0.0, 1.0) * camMatrix);
vec3 u = vec3(vec4(1.0, 0.0f, 0.0, 1.0) * camMatrix);
vec3 N = eye - COI;
vec3 n = N / length(N);

vec3 C = eye - n * d;
vec3 L = C - u * width / 2 - v * width / 2;

float intersects(vec3 p, vec3 d, float px, float dx, float x1, float x2){

	float t_near = neg_infinity;
	float t_far = pos_infinity;
	if(abs(dx) <= 0.0001) return neg_infinity;
	else{
		flato t1 = (
	}

	return t_near;
}

vec4 rayTrace(vec2 s){
	float W = width;
	float H = width;
	float voxel_width = 1.0f;
	vec3 e = eye;
	vec3 S = L + u * s.x * W + v * s.y * H;	
	vec3 D = normalize(S - eye);
	for(int x = 0; x < int(width); x++){
		for(int y = 0; y < int(width); y++){
			for(int z = int(d); z < int(d + width); z++){
				float x0 = x, x1 = x + 1; 
				float y0 = y, y1 = y + 1;
				float z0 = z, z1 = z + 1;
				float t_xmin = (x0 - e.x) / e.x;
				float t_xmax = (x1 - e.x) / e.x;
				float t_ymin = (y0 - e.y) / e.y;
				float t_ymax = (y1 - e.y) / e.y;
				float t_zmin = (z0 - e.z) / e.z;
				float t_zmax = (z1 - e.z) / e.z;
				if(!(t_xmin > t_ymax || t_ymin > t_xmax) || !(t_xmin > t_zmax || t_zmin > t_xmax)){
					if(check(vec3(x0,y0,z0 - d))){
						return vec4(WHITE, 1.0f);
					}
				}
				//vec3 p = eye + t * normalize(S - eye);
			}
		}
	}

	return vec4(BLACK, 1.0);
}

void main()
{
	FragColor = rayTrace(vec2(gl_FragCoord.x, gl_FragCoord.y));
}