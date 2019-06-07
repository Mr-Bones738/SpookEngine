#version 330 core

out vec4 fragColour;
in vec2 texc;
uniform sampler2D tex;

void main() {
	fragColour = vec4(texture(tex, texc).rgb, 1.0);
}