void main() {
	gl_FragColor = gl_Color;
	if(gl_Color.r > 0.75) {
		gl_FragColor.r = 1.0;
	}
	else if(gl_Color.r > 0.25) {
		gl_FragColor.r = 0.5;
	}
	else {
		gl_FragColor.r = 0.0;
	}
	
	if(gl_Color.g > 0.75) {
		gl_FragColor.g = 1.0;
	}
	else if(gl_Color.g > 0.25) {
		gl_FragColor.g = 0.5;
	}
	else {
		gl_FragColor.g = 0.0;
	}
	
	if(gl_Color.b > 0.75) {
		gl_FragColor.b = 1.0;
	}
	else if(gl_Color.b > 0.25) {
		gl_FragColor.b = 0.5;
	}
	else {
		gl_FragColor.b = 0.0;
	}
}