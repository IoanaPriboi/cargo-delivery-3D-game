#version 330

in vec3 vert_color;
in vec3 vert_normal;

layout(location = 0) out vec4 out_color;

uniform vec3 object_color;
uniform int use_object_color;   // 1 -> foloseste uniform, 0 -> foloseste vert_color

uniform int is_time_sensitive; // 1 -> culoarea obiectului depinde de timp, 0 -> nu
uniform float time_mix_factor; // 0 -> culoare originala, 1 -> rosu complet

void main()
{
    vec3 c;

    if (use_object_color == 1) {
        c = object_color;
    } else {
        c = vert_color;
    }

    if (is_time_sensitive == 1) {
        c = mix(c, vec3(1, 0, 0), time_mix_factor);
    }
    
    out_color = vec4(c, 1.0);
}
