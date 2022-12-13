#include "Shader.h"

#include <fstream>
#include <iostream>
#include <sstream>

Shader::Shader()
{
}

Shader::Shader(const std::string& vertexSrc, const std::string& fragmentSrc)
{
    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // open files
        vShaderFile.open(vertexSrc);
        fShaderFile.open(fragmentSrc);
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // close file handlers
        vShaderFile.close();
        fShaderFile.close();
        // convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure& e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " << e.what() << std::endl;
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    // vertex shader
    VertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(VertexShader, 1, &vShaderCode, NULL);
    glCompileShader(VertexShader);
    CheckCompileErrors(VertexShader, "VERTEX");
    // fragment Shader
    FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(FragmentShader, 1, &fShaderCode, NULL);
    glCompileShader(FragmentShader);
    CheckCompileErrors(FragmentShader, "FRAGMENT");
    // shader Program
    ShaderProgram = glCreateProgram();
    glAttachShader(ShaderProgram, VertexShader);
    glAttachShader(ShaderProgram, FragmentShader);
    glLinkProgram(ShaderProgram);
    CheckCompileErrors(ShaderProgram, "PROGRAM");
    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(VertexShader);
    glDeleteShader(FragmentShader);
}

Shader::~Shader()
{
}

void Shader::Bind() const
{
    glUseProgram(ShaderProgram);
}

void Shader::Unbind() const
{
    glUseProgram(0);
}

void Shader::UploadUniformFloat2(const std::string& name, const glm::vec2& vector)
{
}

void Shader::UploadUniformBool(const std::string& name, bool value) const
{
    glUniform1i(glGetUniformLocation(ShaderProgram, name.c_str()), (int)value);
}

void Shader::UploadUniformInt(const std::string& name, int value) const
{
    glUniform1i(glGetUniformLocation(ShaderProgram, name.c_str()), value);
}

void Shader::UploadUniformFloat(const std::string& name, float value) const
{
    glUniform1f(glGetUniformLocation(ShaderProgram, name.c_str()), value);
}

void Shader::UploadUniformVec2(const std::string& name, const glm::vec2& value) const
{
    glUniform2fv(glGetUniformLocation(ShaderProgram, name.c_str()), 1, &value[0]);
}

void Shader::UploadUniformVec2(const std::string& name, float x, float y) const
{
    glUniform2f(glGetUniformLocation(ShaderProgram, name.c_str()), x, y);
}

void Shader::UploadUniformVec3(const std::string& name, const glm::vec3& value) const
{
    glUniform3fv(glGetUniformLocation(ShaderProgram, name.c_str()), 1, &value[0]);
}

void Shader::UploadUniformVec3(const std::string& name, float x, float y, float z) const
{
    glUniform3f(glGetUniformLocation(ShaderProgram, name.c_str()), x, y, z);
}

void Shader::UploadUniformVec4(const std::string& name, const glm::vec4& value) const
{
    glUniform4fv(glGetUniformLocation(ShaderProgram, name.c_str()), 1, &value[0]);
}

void Shader::UploadUniformVec4(const std::string& name, float x, float y, float z, float w) const
{
    glUniform4f(glGetUniformLocation(ShaderProgram, name.c_str()), x, y, z, w);
}

void Shader::UploadUniformMat2(const std::string& name, const glm::mat2& mat) const
{
    glUniformMatrix2fv(glGetUniformLocation(ShaderProgram, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::UploadUniformMat3(const std::string& name, const glm::mat3& mat) const
{
    glUniformMatrix3fv(glGetUniformLocation(ShaderProgram, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::UploadUniformMat4(const std::string& name, const glm::mat4& mat) const
{
    glUniformMatrix4fv(glGetUniformLocation(ShaderProgram, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::CompileShader(GLenum shaderType, const std::string& shaderSrc)
{
}

void Shader::CheckCompileErrors(unsigned shader, std::string type)
{
    int success;
    char infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog <<
                "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog <<
                "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}
