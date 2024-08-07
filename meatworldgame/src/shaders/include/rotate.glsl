
vec2 vec2_rotate( vec2 v, float theta )
{
    float mid = 0.5;

    return vec2(
        cos(theta) * (v.x - mid) + sin(theta) * (v.y - mid) + mid,
        cos(theta) * (v.y - mid) - sin(theta) * (v.x - mid) + mid
    );
}

