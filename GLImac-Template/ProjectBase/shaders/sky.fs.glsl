#version 330

in vec3 vPosition_vs; // Position du sommet transformé dans l'espace View
in vec3 vNormal_vs; // Normale du sommet transformé dans l'espace View
in vec2 vTexCoords; // Coordonnées de texture du sommet

uniform sampler2D uColorTexture;
uniform sampler2D uBonusTexture;

out vec3 fFragColor;

void main() {	
    
	fFragColor = texture(uColorTexture,vTexCoords).xyz
		        +texture(uBonusTexture,vTexCoords).xyz;

    //if (fFragColor==vec3(0,0,0))// && vTexCoords==vec2(0,0))
       // fFragColor = vNormal_vs;
    
    // if (fFragColor==vec3(0,0,0))// && vTexCoords==vec2(0,0))
      //  fFragColor = vec3(1,1,1)*0.5;
}