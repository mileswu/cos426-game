uniform sampler2D tex;
varying vec2 texcoord;

void main() {
	vec4 color = vec4(0.0,0.0,0.0,1.0);
	float pixelsize = 3.0*1.0/500.0;
	
	color += texture2D(tex, vec2(texcoord.x, texcoord.y + -20.0*pixelsize)) * 0.00110696738396212;
	color += texture2D(tex, vec2(texcoord.x, texcoord.y + -19.0*pixelsize)) * 0.00150127041028033;
	color += texture2D(tex, vec2(texcoord.x, texcoord.y + -18.0*pixelsize)) * 0.00200445898182135;
	color += texture2D(tex, vec2(texcoord.x, texcoord.y + -17.0*pixelsize)) * 0.00263481162491919;
	color += texture2D(tex, vec2(texcoord.x, texcoord.y + -16.0*pixelsize)) * 0.00340969958728282;
	color += texture2D(tex, vec2(texcoord.x, texcoord.y + -15.0*pixelsize)) * 0.00434406997029223;
	color += texture2D(tex, vec2(texcoord.x, texcoord.y + -14.0*pixelsize)) * 0.0054486844261765;
	color += texture2D(tex, vec2(texcoord.x, texcoord.y + -13.0*pixelsize)) * 0.00672822718937529;
	color += texture2D(tex, vec2(texcoord.x, texcoord.y + -12.0*pixelsize)) * 0.00817944409978257;
	color += texture2D(tex, vec2(texcoord.x, texcoord.y + -11.0*pixelsize)) * 0.0097895127961418;
	color += texture2D(tex, vec2(texcoord.x, texcoord.y + -10.0*pixelsize)) * 0.0115348650207329;
	color += texture2D(tex, vec2(texcoord.x, texcoord.y + -9.0*pixelsize)) * 0.013380677515373;
	color += texture2D(tex, vec2(texcoord.x, texcoord.y + -8.0*pixelsize)) * 0.0152812133734616;
	color += texture2D(tex, vec2(texcoord.x, texcoord.y + -7.0*pixelsize)) * 0.0171811300222411;
	color += texture2D(tex, vec2(texcoord.x, texcoord.y + -6.0*pixelsize)) * 0.0190177773143372;
	color += texture2D(tex, vec2(texcoord.x, texcoord.y + -5.0*pixelsize)) * 0.0207243987520622;
	color += texture2D(tex, vec2(texcoord.x, texcoord.y + -4.0*pixelsize)) * 0.0222340342633355;
	color += texture2D(tex, vec2(texcoord.x, texcoord.y + -3.0*pixelsize)) * 0.0234838203866393;
	color += texture2D(tex, vec2(texcoord.x, texcoord.y + -2.0*pixelsize)) * 0.0244193094405165;
	color += texture2D(tex, vec2(texcoord.x, texcoord.y + -1.0*pixelsize)) * 0.0249983966757998;
	color += texture2D(tex, vec2(texcoord.x, texcoord.y + 0.0*pixelsize)) * 0.0251944615309335;
	color += texture2D(tex, vec2(texcoord.x, texcoord.y + 1.0*pixelsize)) * 0.0249983966757998;
	color += texture2D(tex, vec2(texcoord.x, texcoord.y + 2.0*pixelsize)) * 0.0244193094405165;
	color += texture2D(tex, vec2(texcoord.x, texcoord.y + 3.0*pixelsize)) * 0.0234838203866393;
	color += texture2D(tex, vec2(texcoord.x, texcoord.y + 4.0*pixelsize)) * 0.0222340342633355;
	color += texture2D(tex, vec2(texcoord.x, texcoord.y + 5.0*pixelsize)) * 0.0207243987520622;
	color += texture2D(tex, vec2(texcoord.x, texcoord.y + 6.0*pixelsize)) * 0.0190177773143372;
	color += texture2D(tex, vec2(texcoord.x, texcoord.y + 7.0*pixelsize)) * 0.0171811300222411;
	color += texture2D(tex, vec2(texcoord.x, texcoord.y + 8.0*pixelsize)) * 0.0152812133734616;
	color += texture2D(tex, vec2(texcoord.x, texcoord.y + 9.0*pixelsize)) * 0.013380677515373;
	color += texture2D(tex, vec2(texcoord.x, texcoord.y + 10.0*pixelsize)) * 0.0115348650207329;
	color += texture2D(tex, vec2(texcoord.x, texcoord.y + 11.0*pixelsize)) * 0.0097895127961418;
	color += texture2D(tex, vec2(texcoord.x, texcoord.y + 12.0*pixelsize)) * 0.00817944409978257;
	color += texture2D(tex, vec2(texcoord.x, texcoord.y + 13.0*pixelsize)) * 0.00672822718937529;
	color += texture2D(tex, vec2(texcoord.x, texcoord.y + 14.0*pixelsize)) * 0.0054486844261765;
	color += texture2D(tex, vec2(texcoord.x, texcoord.y + 15.0*pixelsize)) * 0.00434406997029223;
	color += texture2D(tex, vec2(texcoord.x, texcoord.y + 16.0*pixelsize)) * 0.00340969958728282;
	color += texture2D(tex, vec2(texcoord.x, texcoord.y + 17.0*pixelsize)) * 0.00263481162491919;
	color += texture2D(tex, vec2(texcoord.x, texcoord.y + 18.0*pixelsize)) * 0.00200445898182135;
	color += texture2D(tex, vec2(texcoord.x, texcoord.y + 19.0*pixelsize)) * 0.00150127041028033;
	color += texture2D(tex, vec2(texcoord.x, texcoord.y + 20.0*pixelsize)) * 0.00110696738396212;

	gl_FragColor = color;
}