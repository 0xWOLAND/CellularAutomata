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
vec4 RED = {1.0f, 0.0f, 0.0f, 1.0f};
vec4 GREEN = {0.0f, 1.0f, 0.0f, 1.0f};
vec4 BLUE = {0.0f, 0.0f, 1.0f, 1.0f};
vec4 YELLOW = {1.0f, 1.0f, 0.0f, 1.0f};

// COLOR SCHEME
vec4 P_GREEN =   vec4(105.0f, 165.0f, 131.0f, 1.0f) / 255.0f;
vec4 P_ORANGE =  vec4(225.0f, 179.0f, 120.0f, 1.0f) / 255.0f;
vec4 P_YELLOW =  vec4(224.0f, 204.0f, 111.0f, 1.0f) / 255.0f;
vec4 P_PINK =    vec4(236.0f, 121.0f, 154.0f, 1.0f) / 255.0f;
vec4 P_RED =     vec4(159.0f, 2.0f,    81.0f, 1.0f) / 255.0f;

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

Ray normalize(Ray r){
	float val = mag(vec3(r.x, r.y, r.z));
	r.x /= val;
	r.y /= val;
	r.z /= val;
	return r;
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

vec3 invert(vec3 v){
	v *= -1;
	return v;
}
Ray invert(Ray r){
	r.x *= -1;
	r.y *= -1;
	r.z *= -1;
	return r;
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
bool inRange(vec3 v, float f, float s){
	return ((f <= v.x && v.x <= s) && 
			(f <= v.y && v.y <= s) && 
			(f <= v.z && v.z <= s));	
}
bool inRange(vec3 v, float f){
	return ((-f <= v.x && v.x <= f) && 
			(-f <= v.y && v.y <= f) && 
			(-f <= v.z && v.z <= f));	
}
bool withinDist(vec3 v, float f){
	return ((-f <= v.x && v.x <= f) && 
			(-f <= v.y && v.y <= f) && 
			(v.z <= f));	
}

bool getPosition(vec3 v){
	if(!((0 <= v.x && v.x <= width) && (0 <= v.y && v.y <= width) && (0 <= v.z && v.z <= width))){
		return false;
	}
   return check(int(v.x), int(v.y), int(v.z));
}


vec4 RayMarching(vec2 crd){
	// crd -> [0, 4]
	float x1 = float(crd.x);
	float y1 = float(crd.y);
	float z1 = 0;
	float x0 =  float(cameraPosition.x);
	float y0 =  float(cameraPosition.y);
	float z0 = -float(cameraPosition.z);

	float dx = abs(x1 - x0);
	float dy = abs(y1 - y0);
	float dz = abs(z1 - z0);
	float stepX = x0 < x1 ? 1 : -1;
	float stepY = y0 < y1 ? 1 : -1;
	float stepZ = z0 < z1 ? 1 : -1;
	float hypotenuse = sqrt(pow(dx, 2) + pow(dy, 2) + pow(dz, 2));
	float tMaxX = hypotenuse * 0.5 / float(dx);
	float tMaxY = hypotenuse * 0.5 / float(dy);
	float tMaxZ = hypotenuse * 0.5 / float(dz);
	float tDeltaX = hypotenuse / float(dx);
	float tDeltaY = hypotenuse / float(dy);
	float tDeltaZ = hypotenuse / float(dz);
	while (x0 != x1 || y0 != y1 || z0 != z1){
	//while (withinDist(vec3(x0,y0,z0), width)){
		if (tMaxX < tMaxY) {
			if (tMaxX < tMaxZ) {
				x0 = x0 + stepX;
				tMaxX = tMaxX + tDeltaX;
			}
			else if (tMaxX > tMaxZ){
				z0 = z0 + stepZ;
				tMaxZ = tMaxZ + tDeltaZ;
			}
			else{
				x0 = x0 + stepX;
				tMaxX = tMaxX + tDeltaX;
				z0 = z0 + stepZ;
				tMaxZ = tMaxZ + tDeltaZ;
			}
		}
		else if (tMaxX > tMaxY){
			if (tMaxY < tMaxZ) {
				y0 = y0 + stepY;
				tMaxY = tMaxY + tDeltaY;
			}
			else if (tMaxY > tMaxZ){
				z0 = z0 + stepZ;
				tMaxZ = tMaxZ + tDeltaZ;
			}
			else{
				y0 = y0 + stepY;
				tMaxY = tMaxY + tDeltaY;
				z0 = z0 + stepZ;
				tMaxZ = tMaxZ + tDeltaZ;

			}
		}
		else{
			if (tMaxY < tMaxZ) {
				y0 = y0 + stepY;
				tMaxY = tMaxY + tDeltaY;
				x0 = x0 + stepX;
				tMaxX = tMaxX + tDeltaX;
			}
			else if (tMaxY > tMaxZ){
				z0 = z0 + stepZ;
				tMaxZ = tMaxZ + tDeltaZ;
			}
			else{
				x0 = x0 + stepX;
				tMaxX = tMaxX + tDeltaX;
				y0 = y0 + stepY;
				tMaxY = tMaxY + tDeltaY;
			
			z0 = z0 + stepZ;
				tMaxZ = tMaxZ + tDeltaZ;
			}
			if(getPosition(vec3(round(x0),round(y0),round(z0)))){
				return BLACK;
			}
		}
		if(!inRange(vec3(x0,y0,z0), width)) break;
	}
	if(x0 >= width){
		return P_GREEN;
	}
	else if(y0 >= width){
		return P_ORANGE;
	}
	else if(z0 >= width){
		return P_PINK;
	}
	
	return WHITE;
}

bool getPosition2(vec3 v){
//	return v.x < 1.0f && v.y < 1.0f;
	return getPosition(v);
}

vec3 mClamp(vec3 v){
	float high = max(v.x, max(v.y, v.z));
	v /= high;
	return v;
}


vec3 getOffset(vec3 v){
	return vec3(
		max(abs(v.x), abs(v.x - width)), 
		max(abs(v.y), abs(v.y - width)), 
		max(abs(v.z), abs(v.z - width))
	);
}

bool withinDist(vec3 v, vec3 cp, float f){
		return (
			((-f <= v.x || cp.x < -f) && (v.x <= f || cp.x > f)) && 
			((-f <= v.y || cp.y < -f) && (v.y <= f || cp.y > f)) && 
			((-f <= v.z || cp.z < -f) && (v.z <= f || cp.z > f))
			);

}
vec4 RayMarching2(vec2 crd){
	vec3 directedRay = vec3(
		(crd.x - cameraPosition.x),
		(crd.y - cameraPosition.y),
		cameraPosition.z);
	vec3 currentPosition = vec3(cameraPosition.x, cameraPosition.y, -cameraPosition.z);
	directedRay = normalize(directedRay);
	bool flag = false;
	while(withinDist(currentPosition, width * cameraPosition.z)){
		if(getPosition2(currentPosition)){
			return WHITE;
		}
		currentPosition += (directedRay / 100.0f);
	}
	vec3 temp = getOffset(currentPosition);
	float ans = temp.z;
	if(!(0 <= currentPosition.x && currentPosition.x < width)) ans = max(ans, temp.x);
	if(!(0 <= currentPosition.y && currentPosition.y < width)) ans = max(ans, temp.y);
	if(!(0 <= currentPosition.z && currentPosition.z < width)) ans = max(ans, temp.z);
	
	
	if(ans == temp.x){
		return RED;
	}
	else if(ans == temp.y){
		return GREEN;
	}
	else if(ans == temp.z){
		return BLUE;
	}
	
	return BACKGROUND;

}

vec4 RayMarching3(vec2 crd){
	//crd *= cameraPosition.z;
	crd *= cameraPosition.z;
	crd -= crd / 2;
	vec3 directedRay = vec3(
		(cameraPosition.x - crd.x),
		(cameraPosition.y - crd.y),
		cameraPosition.z);
	//vec3 currentPosition = vec3(cameraPosition.x, cameraPosition.y, -cameraPosition.z);
	vec3 currentPosition = vec3(crd.x, crd.y, 0.0f);
	directedRay = normalize(directedRay);
	bool flag = false;
	while(inRange(currentPosition, 0.0f, 5.0f)){
//	for(int i = 0; i < 50000; i++){
		if(getPosition2(currentPosition * mag(cameraPosition))){
			return WHITE;
		}
		currentPosition += (directedRay / 100.0f);
	}
	vec3 temp = getOffset(currentPosition);
	float ans = temp.z;
	if(!(0 <= currentPosition.x && currentPosition.x < width)) ans = max(ans, temp.x);
	if(!(0 <= currentPosition.y && currentPosition.y < width)) ans = max(ans, temp.y);
	if(!(0 <= currentPosition.z && currentPosition.z < width)) ans = max(ans, temp.z);
	
	
	if(ans == temp.x){
		return RED;
	}
	else if(ans == temp.y){
		return GREEN;
	}
	else if(ans == temp.z){
		return BLUE;
	}
	
	return BACKGROUND;

}
void main()
{
	
	if(-gwidth < pos.x && pos.x < gwidth && -gwidth < pos.y && pos.y < gwidth){
		//FragColor = vec4(texture(pixels, texPos));
		// vec2 tPos = vec2(pos.x, pos.y);
		// tPos = tPos / (gwidth / width) + gwidth;
		// tPos.x = max(tPos.x, width);
		// tPos.y = max(tPos.y, width);
		vec2 tPos = vec2(fc.x, fc.y);
		// if(tPos.x < 4) FragColor = GREEN;
		// else FragColor = BLUE;
		FragColor = RayMarching3(tPos);
	}
	else{
		FragColor = BACKGROUND;
		FragColor = vec4(fc.x, fc.y, 0.0f, 1.0f);
		//if(fc.x < 0) FragColor = GREEN;
		// FragColor = getVoxel(0,0,1);
	}

		

}