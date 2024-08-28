#version 330 core
//in vec3 ourColor;
in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D texture1;

void main()
{
    //FragColor = vec4(ourColor, 1.0);
    FragColor = texture(texture1, TexCoord);
}
