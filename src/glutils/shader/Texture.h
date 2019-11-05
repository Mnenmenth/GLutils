/**
  * Created by Earl Kennedy
  * https://github.com/Mnenmenth
  */

#ifndef GLUTILS_TEXTURE_H
#define GLUTILS_TEXTURE_H

#include <glad/glad.h>
#include <vector>
#include <functional>

/// Generalized texture class for easily creating and binding textures
    //! Assumes two sampler2D uniforms in shader for regular tex and specular map
    //! This can be changed by setting a new bind function to bind textures to fit
    //! different shader setups, such as a shader with only one sampler2D
class Texture
{
private:
    static void createTexture_default(GLuint*, unsigned char[]);
    static void bind_default(GLuint, GLuint);

    void (*m_BindFunc)(GLuint, GLuint) = bind_default;
    GLuint m_TexID;
    GLuint m_SpecTexID;

public:

    /// Create texture function is for custom texture setup parameters
        //! Complete override of default create. EVERYTHING about creating texture must be done

    // Sets specular value to the same as the data
        // Not very efficient, but its okay for now since there's not tons and tons of stuff being rendered
    explicit Texture(unsigned char data[], void (*createTexture)(GLuint* texID, unsigned char data[]) = createTexture_default);
    // Sets data and specular respectively
    Texture(unsigned char data[], unsigned char specularData[],
            void(*createTexture)(GLuint* texID, unsigned char data[]) = createTexture_default,
            void(*createSpecTexture)(GLuint* specTexID, unsigned char data[]) = createTexture_default
            );

    /// Custom bind function, for if the texture should have blend funcs, depth funcs, etc
        //! Complete override of default bind. EVERYTHING about binding texture must be done
        //! Texture and specular texture are both bound in bind func
    void setBindFunc(void(*bindFunc)(GLuint texID, GLuint specTexID));

    void bind();

};


#endif //GLUTILS_TEXTURE_H
