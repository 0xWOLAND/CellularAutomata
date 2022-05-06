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

bool check(int i, int j, int k){
	return isFilled(texelFetch(pixels, ivec2(i, floor(width) * j + k), 0));
}



bool getPosition(vec3 v){
	//return v.z < 0.0f;
   return check(int(v.x), int(v.y), int(v.z));
}


vec4 RayMarching(vec2 crd){
	
	int x1 = int(crd.x);
	int y1 = int(crd.y);
	int z1 = 0;
	int x0 = int(cameraPosition.x);
	int y0 = int(cameraPosition.y);
	int z0 = -int(cameraPosition.z);

	int dx = abs(x1 - x0);
	int dy = abs(y1 - y0);
	int dz = abs(z1 - z0);
	int stepX = x0 < x1 ? 1 : -1;
	int stepY = y0 < y1 ? 1 : -1;
	int stepZ = z0 < z1 ? 1 : -1;
	float hypotenuse = sqrt(pow(dx, 2) + pow(dy, 2) + pow(dz, 2));
	float tMaxX = hypotenuse * 0.5 / dx;
	float tMaxY = hypotenuse * 0.5 / dy;
	float tMaxZ = hypotenuse * 0.5 / dz;
	float tDeltaX = hypotenuse / dx;
	float tDeltaY = hypotenuse / dy;
	float tDeltaZ = hypotenuse / dz;
	// while (x0 != x1 || y0 != y1 || z0 != z1){
	while (x0 != width && y0 != width && z0 != width){
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
	}
	return WHITE;
}


void main()
{
	if(-gwidth < pos.x && pos.x < gwidth && -gwidth < pos.y && pos.y < gwidth){
		//FragColor = vec4(texture(pixels, texPos));
		vec2 tPos = vec2(pos.x, pos.y);
		tPos = tPos / (gwidth / width) + gwidth;
		FragColor = RayMarching(tPos);
	}
	else{
		// FragColor = BACKGROUND;
		FragColor = vec4(fc.x, fc.y, 0.0f, 1.0f);
	}

		

}