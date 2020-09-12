#version 330 core


const int MAX_POINT_LIGHTS = 4;
const int MAX_SPOT_LIGHTS = 2;

struct DirectionalLight
{
	vec3 ambientColor;
	float ambientIntensity;

	vec3 directionalColor;
	float directionalIntensity;
	vec3 direction;
};


struct PointLight
{
	vec3 position;
	vec3 color;
	float constant;
	float linear;
	float exponential;
};


struct SpotLight
{
	PointLight point;
	vec3 direction;
	float angle;
};


struct Material
{
	float specularIntensity;
	float shininess;
};

in vec4 vCol;
in vec2 vUV;
in vec3 vNormal;
in vec3 FragPos; // worldspace pos
in vec4 DirectionalLightSpacePos;

// Ouput data
out vec4 color;

// reason why this is automatically identified as the bound texture is because there's only ONE atm.
// If there were more than one texture we'd have to bind the uniform
uniform sampler2D mainTexSampler; 


uniform DirectionalLight dirLight;
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform int pointLightCount;
uniform int spotLightCount;
uniform SpotLight spotLights[MAX_SPOT_LIGHTS];

uniform Material material;
uniform vec3 cameraWorldPos;

uniform sampler2D directionalShadowMap;

float CalcDirectionalShadowFactor(DirectionalLight light)
{
	//First need to transform these coordinates to device coordinates (projection coords) by dividing coords by W (-1 to 1)
	vec3 projCoords = DirectionalLightSpacePos.xyz / DirectionalLightSpacePos.w;
	// Convert to 0-1 scale (like depth map should)
	projCoords = (projCoords * 0.5) + 0.5;

	//Figure out the closest point based on the light's depthmap
	float closestToLight = texture(directionalShadowMap, projCoords.xy).r; //orthogonal view allows us to just use XY coordinates (perspective wouldn't)
	float current = projCoords.z; // how far this fragment actually is

	//if the current fragment's depth is larger than the closest object in the light's depth map, it's "in shadow"
	float shadow = (current > closestToLight) ? 1.0 : 0.0f;
	return shadow;
}

vec4 CalcDirectionalLight()
{
	vec4 diffuseColor = vec4( dirLight.directionalColor, 1.0f) * dirLight.directionalIntensity;

	float diffuseFactor =  max( dot( normalize(vNormal), dirLight.direction) , 0.0f);
	diffuseColor *= diffuseFactor;

	vec4 specularColor = vec4(0);
	if( diffuseFactor > 0)
	{
		vec3 viewDir = normalize(cameraWorldPos - FragPos);
		vec3 lightRefl = normalize(reflect( dirLight.direction, normalize(vNormal)));
		float specularFactor = dot(viewDir, lightRefl);
		
		if( specularFactor > 0) // needed?
		{
			specularFactor = pow(specularFactor, material.shininess);
			specularColor = vec4(dirLight.directionalColor * material.specularIntensity * specularFactor, 1);
		}
	}

	float shadow = CalcDirectionalShadowFactor(dirLight);

	vec4 finalDirLight = (1.0-shadow) * ( diffuseColor + specularColor);

	return finalDirLight;
}


vec4 CalcPointLight(PointLight light )
{
	vec4 diffuseColor = vec4( light.color, 1.0f);
	
	vec3 lightDir = (FragPos - light.position);
	float lightDistance = length(lightDir);
	lightDir = normalize(lightDir);

	float diffuseFactor =  max( dot( normalize(vNormal), lightDir) , 0.0f);
	diffuseColor *= diffuseFactor;
	
	vec4 specularColor = vec4(0);
	if( diffuseFactor > 0)
	{
		//Specular

		vec3 viewDir = normalize(cameraWorldPos - FragPos);
		vec3 lightRefl = normalize(reflect( lightDir, normalize(vNormal)));
		float specularFactor = dot(viewDir, lightRefl);
		
		if( specularFactor > 0) // needed?
		{
			specularFactor = pow(specularFactor, material.shininess);
			specularColor = vec4(light.color * material.specularIntensity * specularFactor, 1);
		}
	}
	
	float attenuation = light.exponential * lightDistance * lightDistance + 
						light.linear * lightDistance +
						light.constant;


	return ( (diffuseColor + specularColor) / attenuation);
}

vec4 CalcPointLights()
{
	vec4 accumulatedPointLightsColor = vec4(0,0,0,0);
	for(int i = 0; i < pointLightCount; i++)
	{
		accumulatedPointLightsColor += CalcPointLight(pointLights[i]);
	}

	return accumulatedPointLightsColor;
}


vec4 CalcSpotLight(SpotLight light)
{
	vec3 lightToFragDirection = FragPos - light.point.position;
	float fragDirDotLightDir = dot( normalize(lightToFragDirection), normalize(light.direction));

	if( fragDirDotLightDir > light.angle)
	{
		vec4 lightCol =  CalcPointLight(light.point); 

		//Map the value to a 0-1 scale
		float fallOff =  1.0 - (( 1.0 - fragDirDotLightDir) * (1.0/ (1.0-light.angle)));
		return lightCol * fallOff;
	}

	return vec4(0,0,0,0);
}

vec4 CalcSpotLights()
{
	vec4 accumulatedSpotLightsColor = vec4(0,0,0,0);
	for(int i = 0; i < spotLightCount; i++)
	{
		accumulatedSpotLightsColor += CalcSpotLight(spotLights[i]);
	}
	return accumulatedSpotLightsColor;
}

void main()
{
	vec4 ambientLight = vec4( dirLight.ambientColor, 1.0f) * dirLight.ambientIntensity;
	vec4 directionalLightColor = CalcDirectionalLight();
	vec4 pointLightsColor = CalcPointLights();
	vec4 spotLightsColor = CalcSpotLights();

	color = texture(mainTexSampler, vUV)*  (ambientLight + directionalLightColor + pointLightsColor + spotLightsColor );
}