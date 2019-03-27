#version 120

uniform sampler2D tex;
uniform float power;

float gamma(float val)
{
  return pow(val, power);
}

void main()
{
  vec4 pixel = texture2D(tex, gl_TexCoord[0].xy);
  gl_FragColor = vec4(gamma(pixel.r), gamma(pixel.g), gamma(pixel.b), pixel[3]);
}
