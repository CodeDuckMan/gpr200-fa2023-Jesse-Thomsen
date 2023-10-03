#version 450
out vec4 FragColor;
in vec2 UV;
uniform float _iTime;
uniform vec2 _aspectRatio;
uniform vec3 _sunColor;
uniform vec3 _buildingColor1;
uniform vec3 _buildingColor2;
uniform vec3 _buildingColor3;

void main(){
vec2 uv = UV;
	
    
    uv = (2.0*gl_FragCoord.xy-_aspectRatio.xy)/_aspectRatio.y;

    float d = distance(uv, vec2(0.0,(sin(_iTime) * 0.6) - 0.5));
    
    d = smoothstep(0.4, 0.5, d);
    
    
    // Sun
    vec3 bgColor = mix(vec3(sin(_iTime)*0.7 + 0.2, sin(_iTime)*0.2 + 0.2, sin(_iTime)*0.1 + 0.4), vec3(0.4,0.1,0.4), uv.y);
    vec3 color = mix(_sunColor, bgColor, d);
    
    // Buildings
    float t = abs(mod(floor(uv.x * 5.0 + _iTime/2.0),2.0) -1.2 ) * 0.8;
    t = step(t,uv.y + 0.9);
    vec3 buildingColor = _buildingColor1;
    color = mix(buildingColor, color, t);
    
    t = abs(mod(floor(uv.x * 5.0 + _iTime/0.8),2.0) -1.0 ) * 0.8;
    t = step(t,uv.y + 0.8);
    buildingColor = _buildingColor2;
    color = mix(buildingColor, color, t);
    
    t = abs(mod(floor(uv.x * 5.0 + _iTime/0.4),2.0) -0.8 ) * 0.8;
    t = step(t,uv.y + 1.0);
    buildingColor = _buildingColor3;
    color = mix(buildingColor, color, t);

	FragColor = vec4(color, 1.0);
}