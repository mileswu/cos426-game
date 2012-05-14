uniform samplerCube tex;
varying vec3 texcoord;
void main()
{
   gl_FragColor = textureCube(tex, texcoord);
}