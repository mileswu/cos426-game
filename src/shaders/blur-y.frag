uniform sampler2D tex;
varying vec2 texcoord;

void main() {
	vec4 color = vec4(0.0,0.0,0.0,1.0);
	float pixelsize = 4.0*1.0/500.0;
	
	color += texture2D(tex, vec2(texcoord.x, texcoord.y + -10.0*pixelsize)) * 0.002209803590374;
	color += texture2D(tex, vec2(texcoord.x, texcoord.y + -9.0*pixelsize)) * 0.00400143736749684;
	color += texture2D(tex, vec2(texcoord.x, texcoord.y + -8.0*pixelsize)) * 0.00680667425186954;
	color += texture2D(tex, vec2(texcoord.x, texcoord.y + -7.0*pixelsize)) * 0.0108770344837837;
	color += texture2D(tex, vec2(texcoord.x, texcoord.y + -6.0*pixelsize)) * 0.0163283626968918;
	color += texture2D(tex, vec2(texcoord.x, texcoord.y + -5.0*pixelsize)) * 0.0230266821828666;
	color += texture2D(tex, vec2(texcoord.x, texcoord.y + -4.0*pixelsize)) * 0.0305053975999552;
	color += texture2D(tex, vec2(texcoord.x, texcoord.y + -3.0*pixelsize)) * 0.0379645807085439;
	color += texture2D(tex, vec2(texcoord.x, texcoord.y + -2.0*pixelsize)) * 0.0443850916074496;
	color += texture2D(tex, vec2(texcoord.x, texcoord.y + -1.0*pixelsize)) * 0.0487474865636634;
	color += texture2D(tex, vec2(texcoord.x, texcoord.y + 0.0*pixelsize)) * 0.0502948978942108;
	color += texture2D(tex, vec2(texcoord.x, texcoord.y + 1.0*pixelsize)) * 0.0487474865636634;
	color += texture2D(tex, vec2(texcoord.x, texcoord.y + 2.0*pixelsize)) * 0.0443850916074496;
	color += texture2D(tex, vec2(texcoord.x, texcoord.y + 3.0*pixelsize)) * 0.0379645807085439;
	color += texture2D(tex, vec2(texcoord.x, texcoord.y + 4.0*pixelsize)) * 0.0305053975999552;
	color += texture2D(tex, vec2(texcoord.x, texcoord.y + 5.0*pixelsize)) * 0.0230266821828666;
	color += texture2D(tex, vec2(texcoord.x, texcoord.y + 6.0*pixelsize)) * 0.0163283626968918;
	color += texture2D(tex, vec2(texcoord.x, texcoord.y + 7.0*pixelsize)) * 0.0108770344837837;
	color += texture2D(tex, vec2(texcoord.x, texcoord.y + 8.0*pixelsize)) * 0.00680667425186954;
	color += texture2D(tex, vec2(texcoord.x, texcoord.y + 9.0*pixelsize)) * 0.00400143736749684;
	color += texture2D(tex, vec2(texcoord.x, texcoord.y + 10.0*pixelsize)) * 0.002209803590374;

	gl_FragColor = color;
}