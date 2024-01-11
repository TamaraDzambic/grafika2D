#version 330 core //Koju verziju GLSL jezika da koristim (za programabilni pipeline: GL verzija * 100) i koji profil (core - samo programabilni pipeline, compatibility - core + zastarjele stvari)

//Kanali (in, out, uniform)
layout(location = 0) in vec3 inPos; //Pozicija tjemena
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inUV;
layout(location = 3) in vec4 inCol; //Boja tjemena - ovo saljemo u fragment sejder
layout(location = 4) in vec2 inTex;   // Texture coordinates

out vec4 channelCol; //Izlazni kanal kroz koji saljemo boju do fragment sejdera
out vec2 texCoord;

out vec3 chFragPos;
out vec3 chNormal;
out vec2 chUV;

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


uniform mat4 rotationMatrix;
uniform mat4 translationMatrix;
uniform float scale;

void main() 
{
    switch (mode) {
        case 0:

            break;
        
        case 1:

            break;
        
        case 2:
            gl_Position = uP * uV * uM * vec4(inPos, 1.0);
            channelCol = vec4(0.0,0.0,1.0,1.0);
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
    chUV = inUV;

    // Scale the model to three sizes smaller
    mat4 scaledModel = uM * mat4(vec4(scale, 0.0, 0.0, 0.0),
                                vec4(0.0, scale, 0.0, 0.0),
                                vec4(0.0, 0.0, scale, 0.0),
                                vec4(0.0, 0.0, 0.0, 1.0));

    // Combine the translation, rotation, and scale transformations
    mat4 finalModel = translationMatrix * rotationMatrix * scaledModel;

    chFragPos = vec3(finalModel * vec4(inPos, 1.0));
    chNormal = mat3(transpose(inverse(finalModel))) * inNormal;

    // Calculate the final vertex position in clip space
    gl_Position = uP * uV * vec4(chFragPos, 1.0);

    // Set the output color
    channelCol = vec4(0.2, 0.2, 0.2, 1.0);
    break;
        default:
            chUV = inUV;
            chFragPos = vec3(uM * vec4(inPos, 1.0));
            chNormal = mat3(transpose(inverse(uM))) * inNormal;  
    

            gl_Position = uP * uV * vec4(inPos, 1.0);
            channelCol = inCol;
    }
}
