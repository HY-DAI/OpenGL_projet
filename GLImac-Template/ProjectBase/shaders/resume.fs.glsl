#version 440


in vec3 vs_position;
in vec3 vs_color;
in vec2 vs_texcoord;
in vec3 vs_normal;

out vec4 fs_color;

//Uniforms
uniform sampler2D texture1;
uniform sampler2D texture2;

uniform PointLight pointLight;
uniform DirectionalLight directionalLight;

struct Material
{
    vec3 uKd; // coefficient de reflection diffuse de l'objet
    vec3 uKs; // coefficient de reflection glossy de l'objet
    float uShininess; // exposant de brillance permettant de controller la taille de la "tache" de brillance glossy

	sampler2D diffuseTex;
	sampler2D specularTex;
};

struct PointLight
{
	vec3 position;
	float intensity;
	vec3 color;
};

struct DirectionalLight
{
	vec3 direction;
	float intensity;
	vec3 color;
};


// Functions
vec3 blinnPhlong(vec3 wi, vec3 Li, vec3 N, vec3 Kd, vec3 Ks, float shininess, vec3 halfVec){
    vec3 diffuse = Kd*dot(wi,N);  
    vec3 specular = Ks*pow(max(dot(halfVec,N),0.0),shininess);
    return Li*(diffuse+specular) ;
}


void main()
{
    // Point Lights
    vec3 w0 = normalize(-vs_position);
    vec3 wi = normalize(-pointLight.position);

	fs_color = texture(Material.diffuseTex,vs_texcoord).xyz
		        +texture(Material.specularTex,vs_texcoord).xyz;
/*
    fs_color *= blinnPhlong(
        wi,
        uLightIntensity,
        normalize(vNormal_vs), 
        uKd, uKs,
        uShininess,
        (w0+wi)/2
    );
*/

    //if (fFragColor==vec3(0,0,0))// && vTexCoords==vec2(0,0))
       // fFragColor = vNormal_vs;
    
    // if (fFragColor==vec3(0,0,0))// && vTexCoords==vec2(0,0))
      //  fFragColor = vec3(1,1,1)*0.5;
}
