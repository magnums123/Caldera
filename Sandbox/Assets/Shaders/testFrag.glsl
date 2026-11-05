#version 450

layout(location = 0) out vec4 outColor;

void main()
{
  vec2 uv = gl_FragCoord.xy / vec2(800.0, 600.0);
  outColor = vec4(uv.x, uv.y, 0.2, 1.0);
}
