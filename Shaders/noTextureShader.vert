#version 330

in vec3 vertex_position, vertex_normal;

uniform mat4 view, proj, model;

uniform vec3 light;

out vec3 position_eye, normal_eye, light_position_eye;

void main(){
  position_eye = vec3(view*model*vec4(vertex_position, 1.0));
  normal_eye = mat3(transpose(inverse(view*model))) * vertex_normal;  
  light_position_eye = light;
  gl_Position = proj* vec4(position_eye, 1.0);

  //light_position_eye = vec3(view*vec4(light, 1.0));
}


  