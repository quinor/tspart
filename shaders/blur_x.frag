#version 120

uniform sampler2D tex;
uniform float sigma;
uniform vec2 size;

#define gauss_constant 0.3989422804014327

float gauss_weight(int d)
{
  return (gauss_constant/sigma)*exp(-d*d/(2*sigma*sigma));
}

void main()
{
  vec3 sum = vec3(0.0);
  float delta = 0.001*max(size.x, size.y)*1.0/size.x;
  float div = 0;
  sum += gauss_weight(0)*texture2D(tex, gl_TexCoord[0].xy).rgb;
  div += gauss_weight(0);
  for (int i=1; i<=2*sigma+1; i++)
  {
    sum += gauss_weight(i)*texture2D(tex, gl_TexCoord[0].xy+vec2(delta*i, 0)).rgb;
    sum += gauss_weight(i)*texture2D(tex, gl_TexCoord[0].xy+vec2(-delta*i, 0)).rgb;
    div += 2*gauss_weight(i);
  }
  gl_FragColor = vec4(sum, 1.0)/div;
}
