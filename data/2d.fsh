in vec4 v_color;
in vec2 v_uv1;

out vec4 FragColor;

uniform sampler2D u_texture;

void main()
{
    // pixel shader Documenttation
    vec4 pixel = texture(u_texture, v_uv1);
    vec4 v_chnl = vec4(1,0,0,0);

    if (dot(vec4(1.0), v_chnl) > 0.0) {
        float val = dot(pixel, v_chnl);

        pixel.rgb = (val > 0.5) ? vec3(2.0 * val - 1.0) : vec3(0.0);
        pixel.a = (val > 0.5) ? 1.0 : 2.0 * val;
    }

    // Apply the vertex color to the final result
    FragColor = pixel * v_color;

    //FragColor = v_color;
    
    //FragColor = vec4(1.0, 0.0, 0.0, 1.0); // Red color

    //FragColor = texture(u_texture, v_uv1) * vec4(0.0, 1.0, 1.0, 1.0);//v_color;

    //FragColor = vec4(v_uv1, 0.0, 1.0); // Map UV to red (U) and green (V)
}
