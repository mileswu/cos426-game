uniform sampler2D tex;
varying vec2 texcoord;

void main() {
	vec4 color = vec4(0.0,0.0,0.0,1.0);
	int i; int j;
	float pixelsize = 5.0*1.0/500.0;
	float sigma = 5.0;
	float sum = 0.0;
	float g;
	for(i=-10;i<=10;i++) {
		for(j=-10; j<=10; j++) {
			g = exp(-(float(i*i)+float(j*j))/2.0/sigma);
			sum += g;
			color += g*texture2D(tex, vec2(texcoord.x + float(i)*pixelsize, texcoord.y + float(j)*pixelsize));
		}
	}
	
	color /= sum;
	gl_FragColor = color;
	
}