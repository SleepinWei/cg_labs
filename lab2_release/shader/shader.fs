#version 330 core 

out vec4 FragColor; 

// uniform sampler2D depthTexture;
// uniform float width;
// uniform float height;

// in float depth;
uniform float delta;
uniform vec4 color;

void main(){
    // vec2 uv = vec2(gl_FragCoord.x / width, gl_FragCoord.y / height);
    // float depth_map = texture2D(depthTexture,uv).r;
    // if(depth <= depth_map){
        // FragColor = vec4(1.0,1.0,1.0,1.0);
    // }
    // else{
        // FragColor = vec4(0.0f);
        // FragColor = vec4(0.0f,1.0f,0.0f,0.0f);
    // }
    FragColor = color;
    gl_FragDepth = gl_FragCoord.z * (1-delta);
}