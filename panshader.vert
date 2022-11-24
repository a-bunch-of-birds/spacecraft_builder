#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 viewPos;

//out vec3 fragcolor;
out vec4 fragcolor;
out vec3 FragPos;
out vec2 TexCoord;

vec4 ambient = vec4(1.0, 1.0, 1.0, 1.0);
vec3 light_dir = vec3(0.0, 0.0, 10.0);

void main()
{
vec4 diffuse_color = vec4(0.5, 0.5, 0.5, 1.0);
vec4 homo_pos = vec4(aPos, 1.0); //<-- convert the aPos coords into homogeneous coords 

//<><><><><><><><><><><><><><><>
//MOVED FRAGPOS UP HERE OTHERWISE CALCULATING THE MV MATRIX CAUSES NOTHING TO RENDER
FragPos = vec3(model * vec4(aPos, 1.0)); //<--you NEED this for anything to render
//<--- AMBIENT --->
ambient = diffuse_color * 0.2;

//<--- DIFFUSE --->
mat4 model_view_matrix = view * model;

mat3 normal_matrix = transpose(inverse(mat3(model_view_matrix)));
vec3 normal_vector = mat3(normal_matrix) * aNormal;
normal_vector = normalize(normal_vector);
light_dir = normalize(light_dir);
vec3 diffuse_lighting = max(dot(normal_vector, light_dir), 0.0) * diffuse_color.xyz; 

// <-- EVERYTHING UNDERNEATH HERE IS UNCONDITIONALLY NECESSARY -->
//<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>

fragcolor = vec4(diffuse_lighting, 1.0) + ambient;
gl_Position = projection * view * model * homo_pos; 
TexCoord = aTexCoord;

}