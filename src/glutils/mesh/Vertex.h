/**
  * Created by Earl Kennedy
  * https://github.com/Mnenmenth
  */

#ifndef GLUTILS_VERTEX_H
#define GLUTILS_VERTEX_H

#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

struct Vertex
{
    // Vertex position
    glm::vec3 v;
    // Normal vector
    glm::vec3 n;
    // UV Coordinate
    glm::vec2 t;

    bool operator==(const Vertex& v2) const
    {
        return v == v2.v && n == v2.n && t == v2.t;
    }
};

namespace std
{
    template<> struct hash<Vertex>
    {
        size_t operator()(Vertex const& vertex) const
        {
            return ((hash<glm::vec3>()(vertex.v) ^
                    (hash<glm::vec3>()(vertex.n) << 1)) >> 1) ^
                    (hash<glm::vec2>()(vertex.t) << 1);
        }
    };
}

#endif //GLUTILS_VERTEX_H
