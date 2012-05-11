uniform sampler2D tex;
varying vec2 texcoord;

void main() {
	vec4 color = vec4(0.0,0.0,0.0,1.0);
	int i; int j;
	float pixelsize = 1.0/500.0;
	float sigma = 4.0;
	float sum = 0.0;
	float g;
	for(i=-5;i<=5;i++) {
		for(j=-5; j<=5; j++) {
			g = exp(-(float(i*i)+float(j*j))/2.0/sigma);
			sum = sum + g;
			color += texture2D(tex, vec2(texcoord.x + float(i)*pixelsize, texcoord.y + float(j)*pixelsize));
		}
	}
	
	color /= sum;
	gl_FragColor = color;
	
}