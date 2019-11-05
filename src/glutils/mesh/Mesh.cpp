/**
  * Created by Earl Kennedy
  * https://github.com/Mnenmenth
  */

#include "Mesh.h"
#include <tiny_obj_loader.h>
#include <unordered_map>
#include "Vertex.h"

Mesh::Mesh(std::string& objFile)
{
    // Model loading modified from https://vulkan-tutorial.com/Loading_models
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;

    std::vector<Vertex> tempVerts;
    std::vector<GLuint> tempIndices;

    // Load vertex information, removing duplicate vertices
    std::unordered_map<Vertex, GLuint> uniqueVertices = {};

    for(const auto& shape : shapes)
    {
        for(const auto& index : shape.mesh.indices)
        {
            Vertex vertex = {
                    { // Vertex pos
                            attrib.vertices[3 * index.vertex_index + 0],
                            attrib.vertices[3 * index.vertex_index + 1],
                            attrib.vertices[3 * index.vertex_index + 2]
                    },
                    { // Normal
                            attrib.normals[3 * index.normal_index + 0],
                            attrib.normals[3 * index.normal_index + 1],
                            attrib.normals[3 * index.normal_index + 2]
                    },
                    { // UV coords with flipped Y
                            attrib.texcoords[2 * index.texcoord_index + 0],
                            1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
                    }
            };

            if(uniqueVertices.count(vertex) == 0)
            {
                uniqueVertices[vertex] = static_cast<GLuint>(vertices.size());
                tempVerts.push_back(vertex);
            }

            indices.push_back(uniqueVertices[vertex]);
        }
    }

}