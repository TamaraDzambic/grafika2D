#version 330 core //Koju verziju GLSL jezika da koristim (za programabilni pipeline: GL verzija * 100) i koji profil (core - samo programabilni pipeline, compatibility - core + zastarjele stvari)

out vec4 outCol; //Izlazni kanal koji ce biti zavrsna boja tjemena (ukoliko se ispisuju podaci u memoriju, koristiti layout kao za ulaze verteks sejdera)

in vec4 channelCol; //Kanal iz Verteks sejdera - mora biti ISTOG IMENA I TIPA kao u vertex sejderu
in vec2 texCoord; //koordinate teksture

in vec3 chNormal;  
in vec3 chFragPos;  
in vec2 chUV;

uniform vec3 uLightPos; 
uniform vec3 uViewPos; 
uniform vec3 uLightColor;


uniform sampler2D uTex; //teksturna jedinica
uniform bool useTexture;
uniform bool model;

void main() //Glavna funkcija sejdera
{
        float ambientStrength = 0.5;
        vec3 ambient = ambientStrength * uLightColor;
  	
        // diffuse 
        vec3 norm = normalize(chNormal);
        vec3 lightDir = normalize(uLightPos - chFragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * uLightColor;
    
        // specular
        float specularStrength = 0.5;
        vec3 viewDir = normalize(uViewPos - chFragPos);
        vec3 reflectDir = reflect(-lightDir, norm);  
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
        vec3 specular = specularStrength * spec * uLightColor;  

        outCol =  channelCol * vec4(ambient + diffuse + specular, 1.0);

    if(model){
            outCol =  texture(uTex, chUV) * vec4(ambient + diffuse + specular, 1.0);
    }
    if (useTexture) {
        outCol = texture(uTex, texCoord); //boja na koordinatama chTex teksture vezane na teksturnoj jedinici uTex
    } 
    
}