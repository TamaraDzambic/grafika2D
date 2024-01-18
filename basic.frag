#version 330 core //Koju verziju GLSL jezika da koristim (za programabilni pipeline: GL verzija * 100) i koji profil (core - samo programabilni pipeline, compatibility - core + zastarjele stvari)

out vec4 outCol; //Izlazni kanal koji ce biti zavrsna boja tjemena (ukoliko se ispisuju podaci u memoriju, koristiti layout kao za ulaze verteks sejdera)

in vec4 channelCol; //Kanal iz Verteks sejdera - mora biti ISTOG IMENA I TIPA kao u vertex sejderu
in vec2 TexCoords; //koordinate teksture

in vec3 chNormal;  
in vec3 chFragPos;  

uniform vec3 uLightPos; 
uniform vec3 uViewPos; 
uniform vec3 uLightColor;


uniform sampler2D uTex; //teksturna jedinica
uniform int mode;




struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
}; 

struct DirLight {
    vec3 direction;
    vec3 color;
    vec3 intensity;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;
    vec3 color;
    vec3 intensity;
    
    float constant;
    float linear;
    float quadratic;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
    vec3 color;
  
    float constant;
    float linear;
    float quadratic;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;       
};

uniform Material material;
uniform DirLight dirLight;
uniform PointLight pointLight;
uniform SpotLight spotLight;


vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);


void main() //Glavna funkcija sejdera
{
  	
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
        
        float ambientStrength = 0.5;
        vec3 ambient = ambientStrength * uLightColor;
        
        vec3 result = CalcDirLight(dirLight, norm, viewDir) 
                + CalcPointLight(pointLight, norm, chFragPos, viewDir)
                + CalcSpotLight(spotLight, norm, chFragPos, viewDir);


      switch (mode) {
           case 0:
               outCol = texture(uTex, TexCoords); //boja na koordinatama chTex teksture vezane na teksturnoj jedinici uTex
               break;
           
           case 1:  //model
               outCol =  texture(uTex, TexCoords) * vec4(result, 1.0);
               break;
            
           case 2:  //island
               outCol =  texture(uTex, TexCoords) * vec4(result, 1.0);
               break;
            
           case 3:  //clouds
               outCol =  channelCol * vec4(ambient + diffuse + specular, 1.0);
               break;
           
           case 4: //fishes
               outCol =  channelCol * vec4(result, 1.0);
               break;

             case 5:  //fire
               outCol = channelCol * vec4(ambient + diffuse + specular, 1.0) * vec4(pointLight.intensity, 1.0);
               break;
            
            default:
               outCol = channelCol;
    }
    
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    // combine results
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords)) * light.color * light.intensity;
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords)) * light.color * light.intensity;
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords)) * light.color * light.intensity;
    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // combine results
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords)) * light.color * light.intensity;
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords)) * light.color * light.intensity;
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords)) * light.color * light.intensity;
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // combine results
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords)) * light.color * 2;
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords)) * light.color * 2;
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords)) * light.color * 2;
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}