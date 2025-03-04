#version 130

// Attributes passed from the vertex shader
in vec4 color_interp;
in vec2 uv_interp;

// Texture sampler
uniform sampler2D onetex;

void main()
{
    // Sample texture
    vec4 color = texture2D(onetex, uv_interp);

    // Convert color to grayscale
    float gray = dot(color.rgb, vec3(0.299, 0.587, 0.114));
    vec4 grayscaleColor = vec4(vec3(gray), color.a);

    // Assign grayscale color to fragment
    gl_FragColor = grayscaleColor;

    // Check for transparency
    if(color.a < 1.0)
    {
         discard;
    }
}
// Majority of this code was taken from provided sprite_fragment_shader.glsl