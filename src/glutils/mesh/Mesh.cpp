/**
  * Created by Earl Kennedy
  * https://github.com/Mnenmenth
  */

#include "Mesh.h"
#include <tiny_obj_loader.h>


Mesh::Mesh(std::string& objFile)
{
    // Model loading modified from https://vulkan-tutorial.com/Loading_models
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;

    for(const auto& shape : shapes)
    {
        for(const auto& index : shape.mesh.indices)
        {
            
        }
    }

}