#version 330 core

// uniform data
uniform mat4 uProjectionMatrix;
uniform mat4 uModelViewMatrix;
uniform vec3 uColor;
uniform vec3 uLightColour;
uniform vec3 uLightDirection;
uniform vec3 uSpecularColour;
uniform vec3 uExtraAttributes;
uniform bool uTextured;
uniform sampler2D uTexture;


// viewspace data (this must match the output of the fragment shader)
in VertexData {
	vec3 position;
	vec3 normal;
	vec2 textureCoord;
	vec3 colours;
} f_in;

// framebuffer output
out vec4 fb_color;

void main() {

	vec3 objectColour = vec3(1,1,1);
	bool textured = uTextured;
	objectColour = f_in.colours;
	if (textured) {
	objectColour = vec3(texture(uTexture, f_in.textureCoord));
	}
	
	vec3 lightDirection = uLightDirection;
	vec3 lightColour = uLightColour;

	float ambientStrength = uExtraAttributes.z;
	vec3 ambient = ambientStrength * objectColour; // * lightColour;

	vec3 norm = normalize(f_in.normal);
	vec3 lightDir = normalize(-lightDirection);

	float diff = max(dot(norm,lightDir), 0.0);
	vec3 diffuse = diff * lightColour * objectColour;

	float specularStrength = uExtraAttributes.x;
	float toonStrength = uExtraAttributes.y;
	vec3 reflectDir = reflect(-lightDir,norm);
	vec3 viewDir = normalize(-f_in.position);

	float spec = pow(max(dot(viewDir,reflectDir), uExtraAttributes.y), 32);
	vec3 specular = specularStrength * spec * uSpecularColour * vec3(1,1,1);

	vec3 result = ambient + diffuse + specular;


	// output to the frambuffer
	fb_color = vec4(result, 1.0);
}