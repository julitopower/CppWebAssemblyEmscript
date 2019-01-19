attribute vec4 position;
varying vec3 f_color;

void main() {
     gl_Position = vec4(position.xyz, 1.0);
     f_color = position.xyz;
}
