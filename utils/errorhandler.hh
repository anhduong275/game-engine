#include <iostream>
#define GLAD_GL_IMPLEMENTATION
#include <glad/glad.h>
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/gl3ext.h>
#endif

using namespace std;

class ErrorHandler {
private:
    // static ErrorHandler* instance;
    // ErrorHandler() { std::cout << "Singleton created" << std::endl; }
    // ErrorHandler(const ErrorHandler&) = delete;             // delete copy constructor
    // ErrorHandler& operator=(const ErrorHandler&) = delete;  // delete copy assignment
    // Helper function to check for and print OpenGL errors
    static GLenum _glCheckError(const char *file, int line) {
        GLenum errorCode;
        while ((errorCode = glGetError()) != GL_NO_ERROR) {
            std::string error;
            switch (errorCode) {
                case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
                case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
                case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
                case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
                case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
                case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
                case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
                default:                               error = "Unknown Error"; break; // Use gluErrorString for other errors
            }
            std::cerr << "OpenGL Error: " << error << " | " << file << " (" << line << ")" << std::endl;
        }
        return errorCode;
    }
public:
    // static ErrorHandler* getInstance() {
    //     if (instance == nullptr) {
    //         instance = new ErrorHandler();
    //     }
    //     return instance;
    // }

    static inline GLenum glCheckError() {
        return _glCheckError(__FILE__, __LINE__);
    }
};