[Vertex_Shader]
#version 140
in vec4 gxl3d_Position;
in vec4 gxl3d_Normal;
in vec4 gxl3d_TexCoord0;
out vec4 Normal;
out vec4 Vertex_UV;
uniform mat4 gxl3d_ModelViewProjectionMatrix;
void main()
{
  gl_Position = gxl3d_ModelViewProjectionMatrix * gxl3d_Position;
  Vertex_UV = gxl3d_TexCoord0;
  Normal = gxl3d_Normal;
}

[Pixel_Shader]
#version 140
uniform sampler2D tex0;
in vec4 Normal;
in vec4 Vertex_UV;
out vec4 Out_Color;
void main()
{
  vec2 uv = Vertex_UV.xy;
  Out_Color.rgb = texture(tex0, uv).rgb;
  //Out_Color.rgb = vec3(0, 1, 0);
  //Out_Color.rgb = vec3(Normal.xy, 0);
  Out_Color.a = 1.0;
}
