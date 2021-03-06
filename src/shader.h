#ifndef SHADER_INC
#   define SHADER_INC

#define STRINGIFY(A) #A

const char* defaultVS = STRINGIFY(

	varying vec3 normal;\n
	varying vec3 vertex;\n

	uniform vec3 lightDir; \n

	varying float intensity; \n

	void main() {
		gl_Position = ftransform();
		vertex = vec3(gl_ModelViewMatrix * gl_Vertex); \n
		normal = normalize(gl_NormalMatrix * gl_Normal);\n
		gl_TexCoord[0] = gl_MultiTexCoord0;\n

		intensity = max(0.0, dot(normalize(lightDir), normalize(normal)));
	}

);

const char* defaultFS = STRINGIFY(

	varying vec3 normal;\n
	varying vec3 vertex;\n

	uniform vec3 lightDir; \n

	varying float intensity; \n

	uniform vec3 fogColor;
	uniform float minFogRadius;
	uniform float maxFogRadius;

	void main() {

		vec3 amb = gl_FrontMaterial.ambient.rgb;
		vec3 dif = gl_FrontMaterial.diffuse.rgb * intensity;
		float alpha = gl_FrontMaterial.diffuse.a;

		vec4 sourceColor = vec4(amb + dif, alpha);

		float dist = length(vertex);
		float fogfac = (dist - minFogRadius) / (maxFogRadius - minFogRadius);
		fogfac = clamp(fogfac, 0.0, 1.0);

		gl_FragColor = mix(sourceColor, vec4(fogColor, 1.0), fogfac);
	}

);

const char* floorVS = STRINGIFY(

	uniform sampler2D normalMap;\n
	uniform sampler2D colorMap;\n
	uniform sampler2D perlinMap;\n

	varying vec3 normal;\n
	varying vec3 vertex;\n

	uniform vec3 lightDir; \n

	varying float intensity; \n

	void main() {\n

		gl_Position = ftransform(); \n

		vertex = vec3(gl_ModelViewMatrix * gl_Vertex); \n
		normal = normalize(gl_NormalMatrix * gl_Normal);\n
		gl_TexCoord[0] = gl_MultiTexCoord0;\n

		intensity = dot(normalize(lightDir), normalize(normal));

	}

);

const char* floorFS = STRINGIFY(

	uniform sampler2D normalMap;\n
	uniform sampler2D colorMap;\n
	uniform sampler2D perlinMap;\n

	varying vec3 normal;\n
	varying vec3 vertex;\n

	uniform vec3 lightDir; \n

	uniform float reflectance;\n

	varying float intensity; \n

	uniform vec3 fogColor;
	uniform float minFogRadius;
	uniform float maxFogRadius;

	void main() {

		vec4 colorSample = texture2D(colorMap, gl_TexCoord[0].st);

		vec4 amb = gl_LightSource[0].ambient * colorSample;
		vec4 dif = gl_LightSource[0].diffuse * max(0.8, intensity) * colorSample;

		float refl = texture2D(perlinMap, gl_TexCoord[0].st * 0.3).r;

		vec4 sourceColor;

		float alpha = clamp(refl + 1.0 - reflectance, 0.5, 1.0);

		// if (refl > reflectance)
			// sourceColor = vec4((amb + dif).rgb, 1.0);
		// else
			sourceColor = vec4((amb + dif).rgb, alpha);

		float dist = length(vertex);
		float fogfac = (dist - minFogRadius) / (maxFogRadius - minFogRadius);
		fogfac = clamp(fogfac, 0.0, 1.0);

		gl_FragColor = mix(sourceColor, vec4(fogColor, 1.0), fogfac);
	}

);


const char* skyVS = STRINGIFY(

	uniform samplerCube cubeMap;\n
	varying vec3 texCoord0; \n

	void main() {\n

		gl_Position = ftransform(); \n
		texCoord0 = gl_Normal; \n
	}

);

const char* skyFS = STRINGIFY(

	uniform samplerCube cubeMap;\n
	varying vec3 texCoord0; \n

	void main() {

		vec4 colorSample = textureCube(cubeMap, texCoord0);
		gl_FragColor = colorSample;
	}

);

const char* windowVS = STRINGIFY(

uniform sampler2D normalMap;\n
uniform sampler2D colorMap;\n

varying vec3 normal;\n
varying vec3 vertex;\n

uniform vec3 lightDir; \n

varying float intensity; \n

void main() {\n

	gl_Position = ftransform(); \n

	vertex = vec3(gl_ModelViewMatrix * gl_Vertex); \n
	normal = normalize(gl_NormalMatrix * gl_Normal);\n
	gl_TexCoord[0] = gl_MultiTexCoord0;\n

	intensity = dot(normalize(lightDir), normalize(normal));

}

);

const char* windowFS = STRINGIFY(

uniform sampler2D normalMap;\n
uniform sampler2D colorMap;\n

varying vec3 normal;\n
varying vec3 vertex;\n

uniform vec3 lightDir; \n

varying float intensity; \n

uniform vec3 fogColor;
uniform float minFogRadius;
uniform float maxFogRadius;

void main() {

	vec4 colorSample = texture2D(colorMap, gl_TexCoord[0].st * 0.18);

	vec4 sourceColor;
	if (colorSample.r > 0.5)
		sourceColor = colorSample;
	else
		sourceColor = vec4((colorSample * max(0.4, intensity)).rgb, 1.0);

	float dist = length(vertex);
	float fogfac = (dist - minFogRadius) / (maxFogRadius - minFogRadius);
	fogfac = clamp(fogfac, 0.0, 1.0);

	gl_FragColor = mix(sourceColor, vec4(fogColor, 1.0), fogfac);
}

);

const char* roofVS = STRINGIFY(

	uniform sampler2D normalMap;\n
	uniform sampler2D colorMap;\n
	uniform sampler2D perlinMap;\n
	uniform samplerCube cubeMap;\n

	varying vec3 normal;\n
	varying vec3 vertex;\n
	varying vec3 refl; \n

	uniform vec3 lightDir; \n

	varying float intensity; \n

	void main() {\n

		gl_Position = ftransform(); \n

		vertex = vec3(gl_ModelViewMatrix * gl_Vertex); \n
		normal = normalize(gl_NormalMatrix * gl_Normal);\n
		refl = normalize(reflect(-vertex, normal));
		gl_TexCoord[0] = gl_MultiTexCoord0;\n

		intensity = dot(normalize(lightDir), normalize(normal));

	}

);

const char* roofFS = STRINGIFY(

	uniform sampler2D normalMap;\n
	uniform sampler2D colorMap;\n
	uniform sampler2D perlinMap;\n
	uniform samplerCube cubeMap;\n

	varying vec3 normal;\n
	varying vec3 vertex;\n
	varying vec3 refl;\n

	uniform vec3 lightDir; \n

	uniform float reflectance;\n

	varying float intensity; \n

	uniform vec3 fogColor;
	uniform float minFogRadius;
	uniform float maxFogRadius;

	void main() {

		vec4 colorSample = texture2D(colorMap, gl_TexCoord[0].st); \n

		vec4 amb = gl_LightSource[0].ambient * colorSample; \n
		vec4 dif = gl_LightSource[0].diffuse * max(1.0, intensity) * colorSample; \n

		float alpha = texture2D(perlinMap, gl_TexCoord[0].st).r; \n

		vec4 ground = vec4((amb + dif).rgb, 1.0); \n

		vec4 cubeSample = textureCube(cubeMap, refl); \n

		vec4 sourceColor;\n

		float a = clamp(alpha + 1.0 - reflectance, 0.5, 1.0);\n

		sourceColor = mix(cubeSample, ground, a);

		float dist = length(vertex);
		float fogfac = (dist - minFogRadius) / (maxFogRadius - minFogRadius);
		fogfac = clamp(fogfac, 0.0, 1.0);

		gl_FragColor = mix(sourceColor, vec4(fogColor, 1.0), fogfac);

	} \n

);

const char* generalVS = STRINGIFY(

uniform sampler2D normalMap;\n
uniform sampler2D colorMap;\n

uniform int gouraudToggle;\n
uniform int blinnPhongToggle;\n
uniform int colorTextureToggle;\n
uniform int normalTextureToggle;\n
uniform int checkerboardToggle;\n

varying vec3 normal;\n
varying vec3 vertex;\n

varying vec4 ambVarying;\n
varying vec4 diffVarying;\n
varying vec4 specVarying;\n

void main() {\n

	gl_Position = ftransform(); \n

	vertex = vec3(gl_ModelViewMatrix * gl_Vertex); \n
	normal = normalize(gl_NormalMatrix * gl_Normal);      \n
	gl_TexCoord[0] = gl_MultiTexCoord0;\n

	vec4 color = vec4(0.0);\n
	vec4 material = gl_FrontMaterial.diffuse;\n

	ambVarying = diffVarying = specVarying = vec4(0.0);\n
	
	if (gouraudToggle != 0) {\n

		for (int i=0; i<gl_MaxLights; i++) {\n

			vec3 lightDir = gl_NormalMatrix * vec3(gl_LightSource[i].position) - vertex;\n

			vec3 eye = normalize(-vertex);        \n
			vec3 refl = normalize(reflect(-lightDir, normal));      \n

			float diffIntensity = clamp(dot(normalize(lightDir), normal), 0.0, 1.0);    \n
			
			float specIntensity = clamp(pow( max(0.0, dot(refl, eye)), gl_FrontMaterial.shininess), 0.0, 1.0); \n

			ambVarying += gl_LightSource[i].ambient;       \n
			diffVarying += gl_LightSource[i].diffuse * diffIntensity;\n
			specVarying += gl_LightSource[i].specular * specIntensity; \n

		}\n

	}\n

	gl_FrontColor = color;\n
}\n

);

const char* generalFS = STRINGIFY(

uniform sampler2D normalMap; \n
uniform sampler2D colorMap; \n

uniform int gouraudToggle; \n
uniform int blinnPhongToggle; \n
uniform int colorTextureToggle; \n
uniform int normalTextureToggle; \n
uniform int checkerboardToggle; \n

varying vec3 normal; \n
varying vec3 vertex; \n

varying vec4 ambVarying; \n
varying vec4 diffVarying; \n
varying vec4 specVarying; \n

void main() { \n

	vec3 n = normalize(normal);\n

	vec4 colorSample = texture2D(colorMap, gl_TexCoord[0].st); \n
	vec4 normalSample = texture2D(normalMap, gl_TexCoord[0].st ); \n
	vec3 normalDelta = normalize(vec3(2.0 * normalSample.x - 1.0,
		                              2.0 * normalSample.y - 1.0,
		                              2.0 * normalSample.z - 1.0));  \n
	vec4 material = gl_FrontMaterial.diffuse;\n

	vec4 color = material;\n

	if (colorTextureToggle != 0) {\n
		material = color = colorSample;\n
	}\n
	else if (checkerboardToggle != 0) {\n
		material = color = (gl_TexCoord[0].s < 0.5 ^^ gl_TexCoord[0].t < 0.5)? vec4(0.0, 0.0, 0.0, 1.0) : vec4(1.0, 1.0, 1.0, 1.0);
	}\n

	if (normalTextureToggle != 0) {\n
		//not the actual normal calculation, but achieves close results
		n = normalize(normal + normalDelta);\n 
	}\n


	if (gouraudToggle != 0) { \n
		color = vec4(0.0);
		color += ambVarying * material;\n
		color += diffVarying * material;\n
		color += specVarying * gl_FrontMaterial.specular;\n		

	}\n
	else if (blinnPhongToggle != 0) {\n
		color = vec4(0.0);

		for (int i=0; i<gl_MaxLights; i++) { \n

			vec3 lightDir = gl_NormalMatrix * vec3(gl_LightSource[i].position) - vertex;

			vec3 halfVector = normalize(vec3(gl_LightSource[i].halfVector));    \n

			float ndotH = clamp(dot(n, halfVector), 0.0, 1.0);       \n

			float diffIntensity = clamp(dot(normalize(lightDir), n), 0.0, 1.0);    \n
			
			float specIntensity = pow( max(0.0, ndotH), gl_FrontMaterial.shininess); \n

			vec4 amb = gl_LightSource[i].ambient * material;       \n
			vec4 diff = gl_LightSource[i].diffuse * material * diffIntensity;\n
			vec4 spec = gl_LightSource[i].specular * gl_FrontMaterial.specular * specIntensity; \n

			color += amb + diff + spec ;\n
		}

	}\n



	gl_FragColor = color;\n
}\n
);

#endif
