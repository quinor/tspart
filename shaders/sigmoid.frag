#version 120

uniform sampler2D tex;
uniform float alpha;
uniform float beta;

#define M_PI 3.1415926535897932384626433832795

float sigmoid(float val)
{
  return atan((val*256-beta)/alpha)/M_PI+0.5;
}

void main()
{
  vec4 pixel = texture2D(tex, gl_TexCoord[0].xy);
  gl_FragColor = vec4(sigmoid(pixel.r), sigmoid(pixel.g), sigmoid(pixel.b), pixel[3]);
}
