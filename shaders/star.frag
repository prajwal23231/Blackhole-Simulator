#version 450 core

out vec4 FragColor;

void main() {
    vec2 coord = gl_PointCoord - vec2(0.5);
    float dist = length(coord);

    if(dist>0.5){
        discard;
    }

    float glow = 1.0 - (dist * 2.0);
    glow = pow(glow, 1.5);

    vec3 starColor = vec3(0.9, 0.95, 1.0);
    
    FragColor = vec4(starColor * glow, glow);
}