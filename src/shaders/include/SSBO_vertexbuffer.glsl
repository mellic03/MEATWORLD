
struct IDK_Vertex
{
    vec3 position;
    vec3 normal;
    vec3 tangent;
    vec2 texcoord;
};


layout (std430, binding = 10) buffer SSBO_VertexBuffer
{
    IDK_Vertex IDK_SSBO_VertexBuffer[];
};


layout (std430, binding = 11) buffer SSBO_IndexBuffer
{
    uint IDK_SSBO_IndexBuffer[];
};




