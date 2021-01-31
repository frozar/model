[Vertex_Shader]
#version 150
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
#version 150
uniform sampler2D tex0;
uniform float elapsed_time;
in vec4 Vertex_UV;
out vec4 Out_Color;
vec4 PostFX(sampler2D tex, vec2 uv, float time)
{
  vec4 c;
  c.r = texture(tex0, uv + uv * 0.002 * cos(time)).r;
  c.g = texture(tex0, uv + uv * 0.01 * cos(100.0*time)*sin(250.0*time)).g;
  c.b = texture(tex0, uv + uv * 0.01 * sin(50*time)).b;
  c.a = 1.0;
  return c;
}
void main()
{
  vec2 uv = Vertex_UV.xy;
  Out_Color = PostFX(tex0, uv, elapsed_time);
}
