uniform sampler2D tex;
varying vec2 texcoord;

void main() {
	gl_FragColor = texture2D(tex, texcoord.xy);
	
	if(gl_FragColor.r < 0.65) {
		gl_FragColor.r = 0.0;
	}
	else {
		gl_FragColor.r = (gl_FragColor.r-0.65)/(1.0-0.65);
	}
	if(gl_FragColor.g < 0.65) {
		gl_FragColor.g = 0.0;
	}
	else {
		gl_FragColor.g = (gl_FragColor.g-0.65)/(1.0-0.65);
	}
	if(gl_FragColor.b < 0.65) {
		gl_FragColor.b = 0.0;
	}
	else {
		gl_FragColor.b = (gl_FragColor.b-0.65)/(1.0-0.65);
	}
}