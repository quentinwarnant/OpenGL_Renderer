#ifndef OBJLOADER_HPP
#define OBJLOADER_HPP

bool LoadOBJ(const char* path,
            std::vector<glm::vec3> & out_verts,
            std::vector<glm::vec2> & out_uv,
            std::vector<glm::vec3> & out_normal );


#endif