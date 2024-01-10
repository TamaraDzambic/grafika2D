#version 330 core //Koju verziju GLSL jezika da koristim (za programabilni pipeline: GL verzija * 100) i koji profil (core - samo programabilni pipeline, compatibility - core + zastarjele stvari)

//Kanali (in, out, uniform)
layout(location = 0) in vec3 inPos; //Pozicija tjemena
layout(location = 1) in vec4 inCol; //Boja tjemena - ovo saljemo u fragment sejder
layout(location = 2) in vec2 inTex;   // Texture coordinates

out vec4 channelCol; //Izlazni kanal kroz koji saljemo boju do fragment sejdera
out vec2 texCoord;

uniform vec2 sunPos;
uniform vec2 seaPos;
uniform vec2 cloudCircleCenter;
uniform int fishPos;
uniform float firePos;

uniform mat4 uM; //Matrica transformacije
uniform mat4 uV; //Matrica kamere
uniform mat4 uP; //Matrica projekcija


uniform int mode; //0 sky, 1 sun and moon, 2 sea, 3 texture, 4 islands, 5 palmtree, 6 fire, 7 clouds, 8 fishes
uniform bool isSun; // 1 sun, 2 moon
uniform bool isLeaf;

void main() 
{
    switch (mode) {
        case 0:

            break;
        
        case 1:

            break;
        
        case 2:
             gl_Position = uP * uV * uM * vec4(inPos, 1.0);
            channelCol = vec4(0.0,0.0,0.1,1.0);
            break;
        
        case 3:
            gl_Position = vec4(inPos, 1.0);
            texCoord = inTex;
            break;
        
        case 4:
            break;
        
        case 5:
            break;
        
        case 6:
           break;
        
        case 7:
           break;
        
        case 8:
            break;
        
        default:
            gl_Position = uP * uV * uM * vec4(inPos, 1.0);
            channelCol = inCol;
    }
}
