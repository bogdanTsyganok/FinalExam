#include "cShaderManager.h"
#include "GLCommon.h"		// glfw.h, etc. (openGL.h)

cShaderManager::cShader::cShader(const char* vertexPath, const char* fragmentPath)
{

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
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
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
        // 2. compile shaders
        unsigned int vertex, fragment;
        // vertex shader
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        checkCompileErrors(vertex, "VERTEX");
        // fragment Shader
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        checkCompileErrors(fragment, "FRAGMENT");
        // shader Program
        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);
        checkCompileErrors(ID, "PROGRAM");
        // delete the shaders as they're linked into our program now and no longer necessary
        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }
}

cShaderManager::cShader::cShader()
{
	this->ID = 0;
	this->shaderType = cShader::UNKNOWN;
	return;
}

cShaderManager::cShader::~cShader() 
{
	return;
}

std::string cShaderManager::cShader::getShaderTypeString(void)
{
	switch ( this->shaderType )
	{
	case cShader::VERTEX_SHADER:
		return "VERTEX_SHADER";
		break;
	case cShader::FRAGMENT_SHADER:
		return "FRAGMENT_SHADER";
		break;
	case cShader::UNKNOWN:
	default:
		return "UNKNOWN_SHADER_TYPE";
		break;
	}
	// Should never reach here...
	return "UNKNOWN_SHADER_TYPE";
}


//// TODO: For the students to do, because it's FUN, FUN, FUN
//std::map< std::string /*name of uniform variable*/,
//		    unsigned int /* uniform location ID */ > 
//						mapUniformName_to_UniformLocation;

// Look up the uniform inside the shader, then save it, if it finds it
bool cShaderManager::cShaderProgram::LoadUniformLocation(std::string variableName)
{
	// 
	GLint uniLocation = glGetUniformLocation(this->ID, 
											 variableName.c_str() );
	// Did it find it (not -1)
	if ( uniLocation == -1 )
	{	// Nope.
		return false;
	}
	// Save it
	this->mapUniformName_to_UniformLocation[variableName.c_str()] = uniLocation;

	return true;	
}

// Look up the uniform location and save it.
int cShaderManager::cShaderProgram::getUniformID_From_Name(std::string name)
{
	std::map< std::string /*name of uniform variable*/,
			  int /* uniform location ID */ >::iterator 
		itUniform = this->mapUniformName_to_UniformLocation.find(name);

	if ( itUniform == this->mapUniformName_to_UniformLocation.end() )
	{
		return -1;		// OpenGL uniform not found value
	}

	return itUniform->second;		// second if the "int" value
}
