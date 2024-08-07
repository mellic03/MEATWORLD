
vec4 triplanar( sampler2D tex, vec3 pos, vec3 N )
{
    vec4 result = vec4(0.0);

    vec3 W = abs(N);
         W /= (W.x + W.y + W.z);

    result += W.z * texture(tex, pos.xy);
    result += W.x * texture(tex, pos.yz);
    result += W.y * texture(tex, pos.xz);

    return result;
}

