#version 130

#define M_PI 3.1415926535897932384626433832795

uniform sampler2D tex1;
uniform sampler2D tex2;
uniform float ratio;

void main()
{
  vec3 a = texture(tex1, gl_TexCoord[0].xy).rgb;
  vec3 b = texture(tex2, gl_TexCoord[0].xy).rgb;
  a = max(vec3(0.001), a);
  b = max(vec3(0.001), b);
  vec3 c = atan(log(vec3(a.r/b.r, a.g/b.g, a.b/b.b)))/M_PI;
  gl_FragColor = vec4(c+0.5, 1.0);
}
