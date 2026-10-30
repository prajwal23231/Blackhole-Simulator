#version 450 core

out vec4 FragColor;

void main() {
    vec2 coord = gl_PointCoord - vec2(0.5);
    float dist = length(coord);

    if(dist>0.5){
        discard;
    }

    float core = smoothstep(0.08, 0.0, dist);
    float glow = 0.06/(dist * dist + 0.04);
    float brightness = core * 0.9 + glow * 0.5;

    vec3 starColor = mix(vec3(0.6, 0.8, 1.0), vec3(1.0, 1.0, 1.0), core);
    
    FragColor = vec4(starColor * brightness, brightness);
}