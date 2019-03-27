#version 120

uniform sampler2D tex;

void main()
{
  vec4 pixel = texture2D(tex, gl_TexCoord[0].xy);
  gl_FragColor = vec4(1.0-pixel.r, 1.0-pixel.g, 1.0-pixel.b, pixel[3]);
}
