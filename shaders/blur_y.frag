#version 130

uniform sampler2D tex;
uniform int radius;

void main()
{
  vec3 sum = vec3(0.0);
  float scale = 1.0/float(radius*2+1);
  float delta = 1.0/textureSize(tex, 0).y;
  for (int i=-radius; i<=radius; i++)
    sum += scale*texture(tex, gl_TexCoord[0].xy+vec2(0, delta*i)).rgb;
  gl_FragColor = vec4(sum, 1.0);
}
