varying vec3 texcoord;

void main() {
	gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;
	texcoord = gl_Normal;
}