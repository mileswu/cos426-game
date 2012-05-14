varying vec3 vtx_normal;
varying vec2 texcoord;
varying vec3 light_dir[2];
varying vec3 eye_dir;

void main() {
	vec3 normal = normalize(vtx_normal);
	vec3 toeye = normalize(eye_dir);

	vec4 ambient = gl_LightModel.ambient * gl_FrontMaterial.ambient;
	vec4 emission = gl_FrontMaterial.emission;
	gl_FragColor = emission + ambient;
	
	for(int i=0; i<2; i++) {
		vec3 tolight = normalize(light_dir[i]);
		vec3 reflection = reflect(-tolight, normal);
	
		vec4 diffuse = gl_LightSource[i].diffuse * max(dot(tolight, normal), 0.0) * gl_FrontMaterial.diffuse;
		diffuse = clamp(diffuse, 0.0, 1.0);
		
		vec4 specular = gl_LightSource[i].specular * pow(max(dot(reflection, toeye), 0.0),gl_FrontMaterial.shininess) * gl_FrontMaterial.specular;
		specular = clamp(specular, 0.0, 1.0);
		
		gl_FragColor += diffuse + specular;
	}
	
	gl_FragColor = clamp(gl_FragColor, 0.0, 1.0);
}