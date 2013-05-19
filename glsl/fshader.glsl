#ifdef GL_ES
precision mediump int;
precision mediump float;
#endif

uniform sampler2D texture;
uniform vec3 color;
uniform bool useTexture;
varying vec2 v_texcoord;

void main()
{
  if (useTexture)
    gl_FragColor = texture2D(texture, v_texcoord);
  else
    gl_FragColor.rgb = color.rgb;
}
