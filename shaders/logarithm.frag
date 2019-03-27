#version 120

uniform sampler2D tex;
uniform float scale;

float logarithm(float val)
{
  return log(1+val*scale)/log(1+scale);
}

void main()
{
  vec4 pixel = texture2D(tex, gl_TexCoord[0].xy);
  gl_FragColor = vec4(logarithm(pixel.r), logarithm(pixel.g), logarithm(pixel.b), pixel[3]);
}
