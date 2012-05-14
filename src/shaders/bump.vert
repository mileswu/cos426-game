varying vec3 vtx_normal;
varying vec3 texcoord;
varying vec3 light_dir[2];
varying vec3 eye_dir;

void main() {
	gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;
	vtx_normal = normalize(gl_NormalMatrix * gl_Normal);
	
	vec3 vtx_pos = vec3(gl_ModelViewMatrix * gl_Vertex);
	light_dir[0] = normalize(gl_LightSource[0].position.xyz - vtx_pos);
	light_dir[1] = normalize(gl_LightSource[1].position.xyz - vtx_pos);
	eye_dir = -vtx_pos;
	
	texcoord = gl_Normal;
}
