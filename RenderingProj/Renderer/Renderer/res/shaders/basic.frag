// fragment shader - runs once per fragment to color in
#version 430 core

in vec4 vPos;
in vec4 vCol;
in vec2 vUV;
in vec3 vNormal;

// the fragment shader has one output(by default) - the color of the fragment
out vec4 fragColor;

// uniforms (shader program globals)
layout (location = 3) uniform sampler2D mainTex;
layout (location = 4) uniform vec3 lightColor;
layout (location = 5) uniform vec3 lightDir;
layout (location = 6) uniform vec3 lightPos;

layout (location = 7) uniform vec3 camera;

//void main()
//{
//	vec3 n = normalize(vNormal);
//	vec3 L = normalize(lightDir);
//
//	float lambertTerm = max(0, min(1, dot( n, -L)));
//
//	vec4 lum = vec4( lambertTerm, lambertTerm, lambertTerm, 1);
//
//	fragColor = texture(mainTex, vUV) * vCol * lum ;
//}

void main()
{
	vec3 n = normalize(vNormal);
	vec3 L = normalize(lightDir);
	
	float lambertTerm = max(0, min(1, dot( n, -L)));
	vec4 diffuse = vec4( lambertTerm, lambertTerm, lambertTerm, 1 );
	
	vec3 V = normalize(camera - vPos.xyz);
	vec3 R = reflect(L, n);

	// calculate specular term, could adjust the specular power as a uniform
	float specularPower = 128.0;
	float specularTerm = pow(max(0, dot(R,V) ), specularPower);
	vec4 specular = vec4(lightColor * specularTerm, 0);

	fragColor = texture(mainTex, vUV) * vCol * diffuse + specular;
}