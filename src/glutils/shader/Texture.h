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
class Texture
{
private:
    typedef void (*CreateTexture)(GLuint*, GLuint, GLuint, unsigned char[]);
    typedef void (*BindTexture)(GLuint, GLuint);
    static void createTexture_default(GLuint*, GLuint, GLuint, unsigned char[]);
    // Default bind func without specular map (second param isn't used but is present for templating. value doesn't matter)
    static void bind_default(GLuint, GLuint);
    // Default bind func with specular map
    static void bind_specDefault(GLuint, GLuint);

    BindTexture m_BindFunc;
    GLuint m_TexID;
    GLuint m_SpecTexID;
    GLuint m_Width;
    GLuint m_Height;

public:

    /// Create texture function is for custom texture setup parameters
        //! Complete override of default create. EVERYTHING about creating texture must be done

    // Sets normal texture without specular map
    Texture(GLuint width, GLuint height, unsigned char data[], CreateTexture func = createTexture_default);
    // Sets data and specular respectively
    Texture(GLuint width, GLuint height, unsigned char data[], unsigned char specularData[],
            CreateTexture dataFunc = createTexture_default,
            CreateTexture specDataFunc = createTexture_default
            );

    /// Custom bind function, for if the texture should have blend funcs, depth funcs, etc
        //! Complete override of default bind. EVERYTHING about binding texture(s) (regular and spec map) must be done
    void setBindFunc(BindTexture func);
    void bind();

    GLuint getTexID();
    GLuint getSpecTexID();
    GLuint getWidth();
    GLuint getHeight();

};


#endif //GLUTILS_TEXTURE_H
