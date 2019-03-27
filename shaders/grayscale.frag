#version 120

uniform sampler2D tex;

void main()
{
  vec4 pixel = texture2D(tex, gl_TexCoord[0].xy);
  float gray = pixel[0]*0.3+pixel[1]*0.59+pixel[2]*0.11;
  gl_FragColor = vec4(gray, gray, gray, pixel[3]);
}
