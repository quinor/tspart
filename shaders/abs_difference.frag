#version 120

uniform sampler2D tex1;
uniform sampler2D tex2;
uniform float ratio;

void main()
{
  vec4 c1 = texture2D(tex1, gl_TexCoord[0].xy);
  vec4 c2 = texture2D(tex2, gl_TexCoord[0].xy);
  gl_FragColor = vec4(
    abs(c1.r-c2.r),
    abs(c1.g-c2.g),
    abs(c1.b-c2.b),
    1.0
  );
}
