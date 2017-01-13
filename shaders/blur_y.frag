#version 130

uniform sampler2D tex;
uniform float sigma;

#define gauss_constant 0.3989422804014327

float gauss_weight(int d)
{
  return (gauss_constant/sigma)*exp(-d*d/(2*sigma*sigma));
}

void main()
{
  vec3 sum = vec3(0.0);
  float delta = 1.0/textureSize(tex, 0).y;
  float div = 0;
  sum += gauss_weight(0)*texture(tex, gl_TexCoord[0].xy).rgb;
  div += gauss_weight(0);
  for (int i=1; i<=2*sigma; i++)
  {
    sum += gauss_weight(i)*texture(tex, gl_TexCoord[0].xy+vec2(0, delta*i)).rgb;
    sum += gauss_weight(i)*texture(tex, gl_TexCoord[0].xy+vec2(0, -delta*i)).rgb;
    div += 2*gauss_weight(i);
  }
  gl_FragColor = vec4(sum, 1.0)/div;
}
