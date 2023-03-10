#version 330

in vec3 vPosition_vs; // Position du sommet transformé dans l'espace View
in vec3 vNormal_vs; // Normale du sommet transformé dans l'espace View
in vec2 vTexCoords; // Coordonnées de texture du sommet

uniform sampler2D uColorTexture;
uniform sampler2D uBonusTexture;

out vec3 fFragColor;


uniform vec3 uKd; // coefficient de reflection diffuse de l'objet
uniform vec3 uKs; // coefficient de reflection glossy de l'objet
uniform float uShininess; // exposant de brillance permettant de controller la taille de la "tache" de brillance glossy

uniform vec3 uAmbiantLight;

uniform vec3 uLightDir_vs;  // direction incidente 
uniform vec3 uLightIntensity; // intensite de la lumière incidente 

uniform vec3 uLightPos_vs1;  // position lum
uniform vec3 uLightIntensity1; // intensite de la lumière incidente 
uniform vec3 uLightPos_vs2;  // position lum
uniform vec3 uLightIntensity2; // intensite de la lumière incidente 




vec3 blinnPhlong(vec3 wi, vec3 Li, vec3 N, vec3 Kd, vec3 Ks, float shininess, vec3 halfVec){
    vec3 diffuse = Kd*dot(wi,N);
    vec3 specular = Ks*pow(dot(halfVec,N),shininess);    
    //vec3 specular = Ks*pow(max(dot(halfVec,N),0.0),shininess);
    //vec3 specular = Ks*pow(max(dot(halfVec,reflect(-wi,N)),0.0),shininess);
    return Li*(diffuse+specular) ;
}

vec3 clamp(vec3 color) {
    float cmoy = ( color.x + color.y + color.z )/3.f;
    if(cmoy < 0)
        return vec3(0);
    
    return cmoy * vec3(1);
}

void main() {	
    
	fFragColor = texture(uColorTexture,vTexCoords).xyz
		        +texture(uBonusTexture,vTexCoords).xyz;

    vec3 flight = uAmbiantLight;

    vec3 w0 = normalize(-vPosition_vs);
    vec3 wi = normalize(uLightDir_vs);

    flight += clamp(blinnPhlong(
        wi,
        uLightIntensity,
        normalize(vNormal_vs), 
        uKd, uKs,
        uShininess,
        (w0+wi)/2
    ));

    wi = normalize(uLightPos_vs1 - vPosition_vs);
    float d = distance(uLightPos_vs1, vPosition_vs);

    flight += clamp(blinnPhlong(
        wi,
        uLightIntensity1 / (d * d),
        vNormal_vs, 
        uKd, uKs,
        uShininess,
        (w0+wi)/2
    ));
  
    wi = normalize(uLightPos_vs2 - vPosition_vs);
    d = distance(uLightPos_vs2, vPosition_vs);

    flight += clamp(blinnPhlong(
        wi,
        uLightIntensity2 / (d * d),
        vNormal_vs, 
        uKd, uKs,
        uShininess,
        (w0+wi)/2
    ));

    //fFragColor *= normalize(flight);
    fFragColor *= flight;


    //if (fFragColor==vec3(0,0,0))// && vTexCoords==vec2(0,0))
       // fFragColor = vNormal_vs;
    
    // if (fFragColor==vec3(0,0,0))// && vTexCoords==vec2(0,0))
      //  fFragColor = vec3(1,1,1)*0.5;
}