#version 330 core //Koju verziju GLSL jezika da koristim (za programabilni pipeline: GL verzija * 100) i koji profil (core - samo programabilni pipeline, compatibility - core + zastarjele stvari)

//Kanali (in, out, uniform)
layout(location = 0) in vec2 inPos; //Pozicija tjemena
layout(location = 2) in vec4 inCol; //Boja tjemena - ovo saljemo u fragment sejder
layout(location = 1) in vec2 inTex;   // Texture coordinates

out vec4 channelCol; //Izlazni kanal kroz koji saljemo boju do fragment sejdera
out vec2 texCoord;

uniform vec2 sunPos;
uniform vec2 seaPos;
uniform vec2 cloudCircleCenter;
uniform int fishPos;
uniform float firePos;

uniform int mode; //0 sky, 1 sun and moon, 2 sea, 3 texture, 4 islands, 5 palmtree, 6 fire, 7 clouds, 8 fishes
uniform bool isSun; // 1 sun, 2 moon
uniform bool isLeaf;

void main() 
{
    switch (mode) {
        case 0:
            vec4 lightBlue = vec4(0.53, 0.81, 0.98, 1.0);
            vec4 darkBlueColor = vec4(0.0, 0.0, 0.2, 1.0);
            float t = (sunPos.y + 1.0) / 2.0;
            vec4 finalBackgroundColor = mix(lightBlue, darkBlueColor, t);
            channelCol = finalBackgroundColor;
            gl_Position = vec4(inPos, 0.0, 1.0);
            break;
        
        case 1:
            gl_Position = vec4(inPos.x - sunPos.x, inPos.y + sunPos.y, 0.0, 1.0);
            channelCol = vec4(0.5, 0.5, 0.5, 1.0);
            if (isSun) {
                channelCol = vec4(1.0, 1.0, 0.0, 1.0);
            }
            break;
        
        case 2:
            channelCol = vec4(0.0, 0.0, 0.0, 1.0);  //efekat senke na moru
            if (inPos.y > -1.0) {
                gl_Position = vec4(inPos.x, inPos.y + seaPos.y, 0.0, 1.0);
                channelCol = vec4(0.0, 0.0, 1.0, 1.0);
            }
            break;
        
        case 3:
            gl_Position = vec4(inPos, 0.0, 1.0);
            texCoord = inTex;
            break;
        
        case 4:
            gl_Position = vec4(inPos.x, inPos.y, 0.0, 1.0);
            channelCol = vec4(0.9, 0.8, 0.4, 1.0);
            break;
        
        case 5:
            gl_Position = vec4(inPos.x, inPos.y, 0.0, 1.0);
            channelCol = vec4(0.5, 0.3, 0.08, 1.0);
            if (isLeaf) {
                vec2 center = vec2(-0.15, 0.4);
                float distanceX = abs(inPos.x - center.x);
                float distanceY = abs(inPos.y - center.y);
                float distance = sqrt(distanceX * distanceX + distanceY * distanceY) * 3;
                channelCol = mix(vec4(0.0, 1.0, 0.3, 1.0), vec4(0.0, 0.0, 0.0, 1.0), distance);
            }
            break;
        
        case 6:
            vec2 targetPos = vec2(0.21, -0.2);
            gl_Position = vec4((inPos.x - targetPos.x) * firePos + targetPos.x, (inPos.y - targetPos.y) * firePos + targetPos.y, 0.0, 1.0);
            float distance = length(inPos - targetPos);
            vec4 redColor = vec4(1.0, 0.0, 0.0, 1.0);
            vec4 yellowColor = vec4(1.0, 1.0, 0.0, 1.0);
            channelCol = mix(redColor, yellowColor, smoothstep(firePos / 6.0, 0, distance));
            break;
        
        case 7:
            gl_Position = vec4(inPos.x + cloudCircleCenter.x, inPos.y + cloudCircleCenter.y, 0.0, 1.0);
            channelCol = vec4(1.0, 1.0, 1.0, 1.0);
            break;
        
        case 8:
            gl_Position = vec4(inPos.x - seaPos.x, inPos.y + seaPos.y, 0.0, 1.0);
            channelCol = vec4(0.5, 0.5, 0.5, 1.0);
            break;
        
        default:
            gl_Position = vec4(inPos.x, inPos.y, 0.0, 1.0);
            channelCol = inCol;
    }
}
