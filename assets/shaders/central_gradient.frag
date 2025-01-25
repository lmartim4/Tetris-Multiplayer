// central_gradient.frag
uniform vec2 u_resolution;   // Tamanho da célula
uniform vec4 u_centerColor;  // Cor do centro (mais escura)
uniform vec4 u_edgeColor;    // Cor das bordas (mais clara)

void main()
{
    // Em vez de gl_FragCoord
    vec2 fragCoord = gl_TexCoord[0].xy;  // SFML envia vTexCoords -> gl_TexCoord[0]

    vec2 center = 0.5 * u_resolution; // (size.x/2, size.y/2)
    float dist = distance(fragCoord, center);
    float maxDist = length(u_resolution * 0.5);
    float t = clamp(dist / maxDist, 0.0, 1.0);

    gl_FragColor = mix(u_centerColor, u_edgeColor, t);
}
