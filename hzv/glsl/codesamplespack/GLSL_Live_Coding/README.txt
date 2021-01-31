
Start GLSL Hacker and load the live-coding-demo.xml.
Open the GPU program live coding tool and display
the pixel shader source code (see here: http://www.geeks3d.com/glslhacker/doc/live_coding.php and
http://geeks3d.com/glslhacker/doc/hack_your_first_glsl_program.php).

Now, copy and paste this chunk of code in the pixel shader main() function:

  vec2 uv = 2.0 * (gl_FragCoord.xy / resolution.xy) - 1.0;
  float col=0.0;
  float i=1.0;
  vec2 spec = vec2(0.1, 0.4);
  uv.x += sin(i*20.0 + spec.x*5.0*time*6.0 + uv.y*1.5) * spec.y;
  col += abs(0.066/uv.x) * spec.y;
  gl_FragColor = vec4(col, col, col, 1.0);
  
  

Source of the pixel shader: http://vimeo.com/51993089


