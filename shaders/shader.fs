#version 330 core
in vec3 position;
in vec3 nw;
in float col;
in vec3 pw;
uniform bool is_wire;
uniform float side_len;
uniform vec3 eye;
uniform float time;

vec3 lighting()
{
    float x = position.x;
    float y = position.y;
    float z = position.z;
    float r = sqrt((x - side_len)*(x - side_len) + (y - side_len)*(y - side_len) + (z - side_len)*(z - side_len));
    float red =    r / (side_len * (2.3 - 0.36 * time * cos(0.09  * time))) ;
    float green =  r / (side_len * (2.9 - 0.82 * time * sin(0.18  * time))) ;
    float blue =   r / (side_len * (3.2 - 0.18 * time * cos(0.22 * time))) ;
    
    //float green = 0.5* abs(side_len / 2 - position.y)/side_len;
    //float blue =  0.5* abs(side_len / 2 - position.z)/side_len;
    // [0, sidelength]
    vec3 amb = vec3(red,green,blue);
    vec3 diff = vec3(red,green,blue);
    vec3 spec = vec3(1,1,0);
    vec3 lpp[3] = vec3[](vec3(0,0,0),vec3(side_len/2, side_len/2, side_len/2),eye);
    float kl = 0;
    float kq = .01;
    //d s ns
    vec3 md = vec3(red,green,blue);
    vec3 ms = vec3(red,green,blue);
    float ns = 100;
    vec3 n = normalize(nw);
    vec3 v = normalize(eye - pw);
    vec3 c = vec3(0.0f);
    for(int i = 0; i < 1; i++){
        vec3 lpos = lpp[i];
        c += amb*md;
        vec3 ll = lpos - pw;
        if (dot(ll, n) > 0.0f) {
            float d = length(ll), k = 1.0f/(1.0f + kl*d + kq*d*d);
            vec3 kv = vec3(k, k, k), l = vec3(ll.x/d, ll.y/d, ll.z/d);
            c += diff*md*dot(n, l)*kv;
            vec3 h = normalize(l + v);
            float nh = dot(n, h);
            c += spec*ms*pow(nh, ns)*kv;
        }
    }
    return vec3(c);
}

void main() 
{
    vec3 c = lighting();
    
    
    gl_FragColor = vec4(c.x,c.y,c.z,1.0); // white color
}