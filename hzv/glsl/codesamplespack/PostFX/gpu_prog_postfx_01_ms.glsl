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
uniform sampler2DMS tex0;
uniform float elapsed_time;
in vec4 gl_FragCoord;
in vec4 Vertex_UV;
out vec4 Out_Color;

vec4 PostFX(sampler2DMS tex, vec2 uv, float time)
{
  vec4 c = vec4(0.0);
  int i;
  vec2 ts = textureSize(tex);
  vec2 uv_r = uv + uv * 0.002 * cos(time);
  vec2 tmp_r = floor(ts * uv_r);
  vec2 uv_g = uv + uv * 0.002 * cos(time);
  vec2 tmp_g = floor(ts * uv_g);
  vec2 uv_b = uv + uv * 0.01 * sin(50*time);
  vec2 tmp_b = floor(ts * uv_b);
  
  for (i=0; i<=4; i++)
  {
    c.r += texelFetch(tex, ivec2(tmp_r), i).r;
    c.g += texelFetch(tex, ivec2(tmp_g), i).g;
    c.b += texelFetch(tex, ivec2(tmp_b), i).b;
  }
  c.rgb /= 5.0;
  c.a = 1.0;
  return c;
}

void main()
{
  vec2 uv = Vertex_UV.xy;
  vec4 c = PostFX(tex0, uv, elapsed_time);
  Out_Color = c;

  /*
  vec4 c = vec4(0.0, 0.0, 0.0, 1.0);
  vec2 tmp = floor(textureSize(tex0) * uv);
  float weight = 1.0/5.0;
	for(int i = 0; i < 4; ++i)
		c += texelFetch(tex0, ivec2(tmp), i) * weight;
  Out_Color = c;
  */
}
