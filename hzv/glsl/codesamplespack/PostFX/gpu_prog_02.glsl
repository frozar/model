[Vertex_Shader]
#version 140
in vec4 gxl3d_Position;
in vec4 gxl3d_TexCoord0;
out vec4 Vertex_UV;
uniform mat4 gxl3d_ModelViewProjectionMatrix;
void main()
{
  gl_Position = gxl3d_ModelViewProjectionMatrix * gxl3d_Position;
  Vertex_UV = gxl3d_TexCoord0;
}

[Pixel_Shader]
#version 140
uniform sampler2D tex0;
in vec4 Vertex_UV;
out vec4 Out_Color;
void main()
{
  vec2 uv = Vertex_UV.xy;
  uv.y *= -1.0;
  //uv.y += 0.5;
  Out_Color.rgb = texture(tex0, uv).rgb;
  Out_Color.a = 1.0;
}
