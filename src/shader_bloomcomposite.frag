uniform sampler2D tex_orig;
uniform sampler2D tex_blur;
varying vec2 texcoord;

void main() {
	gl_FragColor = texture2D(tex_orig, texcoord.xy) + texture2D(tex_blur, texcoord.xy);
}