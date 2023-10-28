#version 440 core

layout (location = 0) out vec4 fsout_frag_color;
layout (location = 1) out vec4 fsout_frag_depth;
layout (location = 2) out vec4 fsout_frag_pos;

in vec2 fsin_texcoords;


layout (std140, binding = 2) uniform UBO_camera_data
{
    mat4 un_view;
    mat4 un_projection;
    vec3 un_viewpos;
};

uniform sampler2D un_last_depth;
uniform sampler2D un_last_position;
uniform sampler2D un_current_position;
uniform sampler3D un_whitenoise;
uniform sampler2D un_bluenoise;
uniform sampler2D un_last_frame;
uniform float un_span;
uniform float un_increment;
uniform float un_w;
uniform float un_h;


struct OctNode
{
    int   blocktype, childrenid, mode, pad2;
};


struct OctNode_group
{
    OctNode nodes[8];
};


struct OctNode_data
{
    int   blocktype;
    float span;
    vec3  center;
    int   groupid, octant;
};


layout (std430, binding = 3) buffer SVO_nodes
{
    OctNode svo_nodegroups[][8];
};

#define GOLDEN_RATIO 1.618033988749
#define EULER 2.718281828459

#define SPAN_DIST_RATIO 0.001
#define ROLLING_AVG_FRAMES 32
#define MOVEMENT_BIAS 0.01
#define BLOCK_BIAS 0.01

#define BLOCK_GRASS 1
#define BLOCK_DIRT 2
#define BLOCK_STONE 3
#define BLOCK_RED 4
#define BLOCK_WHITE 7


vec3 svo_blocktype_color( int blocktype )
{
    switch (blocktype)
    {
        case BLOCK_GRASS: return vec3(0.2, 1.0, 0.2);
        case BLOCK_DIRT:  return vec3(0.69, 0.50, 0.35);
        case BLOCK_STONE: return vec3(0.67, 0.69, 0.71);
        case BLOCK_WHITE: return vec3(1.0, 1.0, 1.0);
        case 4:   return vec3(1.0, 0.2, 0.2);
        case 5:   return vec3(0.2, 1.0, 0.2);
        case 6:   return vec3(0.2, 0.2, 1.0);
    }
}


float svo_blocktype_reflectiveness( int blocktype )
{
    switch (blocktype)
    {
        case BLOCK_GRASS: return 0.9;
        case BLOCK_DIRT:  return 0.7;
        case BLOCK_STONE: return 0.8;
        case BLOCK_WHITE: return 0.8;
        case 4:   return 0.0;
        case 5:   return 0.0;
        case 6:   return 0.0;
    }
}

float svo_blocktype_emission( int blocktype )
{
    switch (blocktype)
    {
        case BLOCK_GRASS: return 0.0;
        case BLOCK_DIRT:  return 0.0;
        case BLOCK_STONE: return 0.0;
        case BLOCK_WHITE: return 5.0;
        case 4:   return 4.0;
        case 5:   return 4.0;
        case 6:   return 4.0;
    }
}


int svo_get_octant( vec3 pos, vec3 center )
{
    int octant = 0;

    if (pos.x < center.x) octant |= 1;
    if (pos.y < center.y) octant |= 2;
    if (pos.z < center.z) octant |= 4;

    return octant;
}


vec3 svo_shift_center( int octant, vec3 center, float span )
{
    vec3 offset;

    offset.x = (octant & 1) == 0 ? span/4.0 : -span/4.0;
    offset.y = (octant & 2) == 0 ? span/4.0 : -span/4.0;
    offset.z = (octant & 4) == 0 ? span/4.0 : -span/4.0;

    return center + offset;
}


OctNode_data svo_get_node( vec3 pos )
{
    OctNode_data node_data;
    int groupid = 0;
    int octant  = 0;

    OctNode node = svo_nodegroups[groupid][octant];
    vec3 center  = vec3(0.0);
    float span   = un_span;

    while (node.childrenid != -1)
    {
        octant = svo_get_octant(pos, center);

        groupid = node.childrenid;
        node = svo_nodegroups[groupid][octant];

        center = svo_shift_center(octant, center, span);
        span /= 2.0;
    }

    node_data.span      = span;
    node_data.blocktype = node.blocktype;
    node_data.center    = center;

    node_data.groupid = groupid;
    node_data.octant  = octant;

    return node_data;
}


OctNode_data svo_get_node2( vec3 pos, float min_span )
{
    OctNode_data node_data;
    int groupid = 0;
    int octant  = 0;

    OctNode node = svo_nodegroups[groupid][octant];
    vec3 center  = vec3(0.0);
    float span   = un_span;

    while (node.childrenid != -1 && span > min_span)
    {
        octant = svo_get_octant(pos, center);

        groupid = node.childrenid;
        node = svo_nodegroups[groupid][octant];

        center = svo_shift_center(octant, center, span);
        span /= 2.0;
    }

    node_data.span      = span;
    node_data.blocktype = node.blocktype;
    node_data.center    = center;

    node_data.groupid = groupid;
    node_data.octant  = octant;

    return node_data;
}


struct AABB_Plane
{
    vec3 pos;
    vec3 dir;
};

AABB_Plane svo_get_x_plane( vec3 node_center, float half_span, vec3 ray_dir )
{
    float f = sign(ray_dir.x);

    vec3 plane_dir = vec3(f, 0.0, 0.0);
    vec3 plane_pos = node_center + half_span*vec3(f, 0.0, 0.0);

    return AABB_Plane(plane_pos, plane_dir);
}

AABB_Plane svo_get_y_plane( vec3 node_center, float half_span, vec3 ray_dir )
{
    float f = sign(ray_dir.y);

    vec3 plane_dir = vec3(0.0, f, 0.0);
    vec3 plane_pos = node_center + half_span*vec3(0.0, f, 0.0);

    return AABB_Plane(plane_pos, plane_dir);
}

AABB_Plane svo_get_z_plane( vec3 node_center, float half_span, vec3 ray_dir )
{
    float f = sign(ray_dir.z);

    vec3 plane_dir = vec3(0.0, 0.0, f);
    vec3 plane_pos = node_center + half_span*vec3(0.0, 0.0, f);

    return AABB_Plane(plane_pos, plane_dir);
}


float ray_plane_intersection( vec3 ray_pos, vec3 ray_dir, AABB_Plane plane )
{
    const float EPSILON = 1e-6;

    float denom = dot(ray_dir, plane.dir);

    if (denom < EPSILON)
    {
        return 0.0;
    }

    float t = dot(plane.pos - ray_pos, plane.dir) / denom;

    return t;
}


vec4 minw(vec4 a, vec4 b)
{
    return mix( a, b, step( b.w, a.w ) );
}

vec4 svo_next_step( OctNode_data node_data, vec3 ray_pos, vec3 ray_dir )
{
    vec3 ray_step = vec3(0.0);

    vec3  center = node_data.center;
    float span   = node_data.span;

    const AABB_Plane X_PLANE = svo_get_x_plane(center, span/2.0, ray_dir);
    const AABB_Plane Y_PLANE = svo_get_y_plane(center, span/2.0, ray_dir);
    const AABB_Plane Z_PLANE = svo_get_z_plane(center, span/2.0, ray_dir);

    const float tx = ray_plane_intersection(ray_pos, ray_dir, X_PLANE);
    const float ty = ray_plane_intersection(ray_pos, ray_dir, Y_PLANE);
    const float tz = ray_plane_intersection(ray_pos, ray_dir, Z_PLANE);

    vec4 result = minw(vec4(X_PLANE.dir, tx), minw(vec4(Y_PLANE.dir, ty), vec4(Z_PLANE.dir, tz)));

    return result;
}



vec3 randomDirection(vec2 seed)
{
    float rand1 = fract(sin(dot(seed, vec2(12.9898, 78.233)) * 43758.5453));
    float rand2 = fract(sin(dot(seed, vec2(63.7264, 10.873)) * 27963.7311));
    float theta = rand1 * 2.0 * 3.14159265359;
    float phi = rand2 * 3.14159265359;
    
    float x = cos(theta) * sin(phi);
    float y = sin(theta) * sin(phi);
    float z = cos(phi);
    
    return normalize(vec3(x, y, z));
}


#define PI 3.14159265359
vec3 randomSpherePoint(vec3 rand)
{
    float ang1 = (rand.x + 1.0) * PI; // [-1..1) -> [0..2*PI)
    float u = rand.y; // [-1..1), cos and acos(2v-1) cancel each other out, so we arrive at [-1..1)
    float u2 = u * u;
    float sqrt1MinusU2 = sqrt(1.0 - u2);
    float x = sqrt1MinusU2 * cos(ang1);
    float y = sqrt1MinusU2 * sin(ang1);
    float z = u;

    return vec3(x, y, z);
}


vec3 randomHemispherePoint(vec3 rand, vec3 n)
{
    // vec3 v = randomSpherePoint(rand);
    return normalize(rand * sign(dot(rand, n)));
}


vec3 trace_diffuse( vec3 ray_pos, vec3 normal, OctNode_data og_data, float depth )
{
    float min_span = SPAN_DIST_RATIO*depth;

    OctNode_data node_data = svo_get_node2(ray_pos, min_span);

    vec3 uva = vec3(fsin_texcoords, un_increment);
    uva.y /= (un_w / un_h);
    // uva.x = float(int(uva.x) / 128);
    // uva.y = float(int(uva.x) / 128);

    vec3 seed = texture(un_whitenoise, uva).rgb - vec3(0.5);
    // seed.x += sign(seed.x) * EULER * un_increment;
    // seed.y += sign(seed.y) * GOLDEN_RATIO * un_increment;
    // seed.z += sign(seed.z) * PI * un_increment;
    vec3 ray_dir = randomHemispherePoint(seed, normal);
    // vec3 ray_dir = normalize(seed);

    for (int i=0; i<32; i++)
    {
        vec4 next_step = svo_next_step(node_data, ray_pos, ray_dir);
        float step_size = next_step.w;
        ray_pos += step_size * ray_dir;

        depth += step_size;
        min_span = SPAN_DIST_RATIO*depth;
        node_data = svo_get_node2(ray_pos + BLOCK_BIAS*next_step.xyz, min_span);

        if (node_data.blocktype > 0)
        {
            float dist = distance(og_data.center, node_data.center);

            float emission = svo_blocktype_emission(node_data.blocktype);
            float strength = svo_blocktype_reflectiveness(og_data.blocktype);
            vec3 irradiance = emission * strength * svo_blocktype_color(node_data.blocktype);
            irradiance /= (1.0 + 0.2*dist + 0.0*dist*dist);

            return irradiance;
        }
    }

    return vec3(0.0);
}


struct DirLight
{
    vec4 direction;
    vec4 ambient;
    vec4 diffuse;
};

layout (std140, binding = 5) uniform UBO_dirlights
{
    int         ubo_num_dirlights;
    DirLight    ubo_dirlights[1];
    mat4        ubo_dirlight_lightspacematrices[1];
};


float trace_shadow( vec3 ray_pos )
{
    vec3 ray_dir = -normalize(ubo_dirlights[0].direction.xyz);
    OctNode_data node_data = svo_get_node(ray_pos);

    // Cast ray to light source
    for (int i=0; i<64; i++)
    {
        vec4 next_step = svo_next_step(node_data, ray_pos, ray_dir);
        float step_size = next_step.w;
        ray_pos += step_size * ray_dir;

        node_data = svo_get_node(ray_pos + 0.001*next_step.xyz);

        if (node_data.blocktype > 0)
        {
            return 0.0;
        }
    }

    return 1.0;
}


vec4 trace( vec3 ray_pos, vec3 ray_dir )
{
    vec3 inv_light_dir = -normalize(ubo_dirlights[0].direction.xyz);

    vec3 result = vec3(0.0, 0.0, 0.0);
    float depth = 0.0;
    float min_span = SPAN_DIST_RATIO*depth;
    
    OctNode_data node_data = svo_get_node2(ray_pos, min_span);

    for (int i=0; i<64; i++)
    {
        vec4 next_step = svo_next_step(node_data, ray_pos, ray_dir);
        float step_size = next_step.w;
        ray_pos += step_size * ray_dir;
        depth   += step_size;


        min_span = 0.0; //SPAN_DIST_RATIO*depth;
        node_data = svo_get_node2(ray_pos + BLOCK_BIAS*next_step.xyz, min_span);

        const vec3 color = svo_blocktype_color(node_data.blocktype);
        const float span = node_data.span;
        const int blocktype = node_data.blocktype;
        const int groupid = node_data.groupid;
        const int octant = node_data.octant;


        if (blocktype > 0)
        {
            // vec3 normal = normalize(ray_pos - node_data.center);
            // vec3 normal_sign = sign(normal);
            // normal = abs(normal);

            // float max_element = max(normal.x, max(normal.y, normal.z));
            // normal = step(vec3(max_element), normal);
            // normal = normalize(normal * normal_sign);

            // float diff = dot(normalize(normal), normalize(inv_light_dir));
            // diff = (diff + 1.0) / 2.0;

            float shadow   = trace_shadow(ray_pos - BLOCK_BIAS*next_step.xyz);
            vec3  indirect = trace_diffuse(ray_pos - BLOCK_BIAS*next_step.xyz, -next_step.xyz, node_data, depth);
            float emission = svo_blocktype_emission(blocktype);

            result = shadow*ubo_dirlights[0].diffuse.xyz*color + emission*color + indirect;

            return vec4(result, depth);
        }
    }

    return vec4(result, 1000000.0);
}

uniform vec3 un_last_viewpos;
uniform mat4 un_last_view;

vec2 stereographicSphereToPlane(vec3 cartPointOnSphere) 
{
    return vec2(
        cartPointOnSphere.x / (1.0-cartPointOnSphere.y), 
        cartPointOnSphere.z / (1.0-cartPointOnSphere.y));
}


uniform int un_moving;

void main()
{
    vec3 frag_pos = texture(un_current_position, fsin_texcoords).xyz;

    vec3 ray_dir = normalize(frag_pos.xyz - un_viewpos);
    vec3 ray_pos = un_viewpos;

    vec4 color_depth = trace(ray_pos, ray_dir);
    vec3 color = color_depth.rgb;
    float depth = color_depth.w;

    ray_pos += depth*ray_dir;

    // ---------------------------------------------------------------------------
    vec4 p_last_pos = un_projection * un_view * vec4(texture(un_last_position, fsin_texcoords).rgb, 1.0);
    p_last_pos.xy = (p_last_pos.xy / p_last_pos.w) * 0.5 + 0.5;
    p_last_pos.xy *= vec2(un_w, un_h);
    p_last_pos.xy = floor(p_last_pos.xy);
    p_last_pos.xy /= vec2(un_w, un_h);

    vec4 p_now_pos = un_projection * un_view * vec4(ray_pos, 1.0);
    p_now_pos.xy = (p_now_pos.xy / p_now_pos.w) * 0.5 + 0.5;
    p_now_pos.xy *= vec2(un_w, un_h);
    p_now_pos.xy = floor(p_now_pos.xy);
    p_now_pos.xy /= vec2(un_w, un_h);

    vec2 movement = (p_last_pos.xy - p_now_pos.xy);
    vec2 last_uv = fsin_texcoords - movement;

    vec3 last_color = texture(un_last_frame, last_uv).rgb;
    // ---------------------------------------------------------------------------

    float last_depth = texture(un_last_depth, fsin_texcoords).r;

    if (abs(depth - last_depth) > 0.5)
    {
        last_color = vec3(0.0);
    }

    if (fsin_texcoords.x < 0.001 || fsin_texcoords.x > 0.999)
    {
        last_color = vec3(0.0);
    }
    if (fsin_texcoords.y < 0.001 || fsin_texcoords.y > 0.999)
    {
        last_color = vec3(0.0);
    }

    const float a = (ROLLING_AVG_FRAMES-1.0) / ROLLING_AVG_FRAMES;
    const float b = 1.0 - a;
    color = a*last_color + b*color;


    fsout_frag_color = vec4(color, 1.0);
    fsout_frag_depth = vec4(vec3(depth), 1.0);
    fsout_frag_pos   = vec4(ray_pos, 1.0);
}