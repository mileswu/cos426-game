uniform sampler2D tex;
varying vec2 texcoord;

void main() {
	//gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
	gl_FragColor = texture2D(tex, texcoord);
}