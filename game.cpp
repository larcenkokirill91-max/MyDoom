#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <math.h>
#include <stdint.h>

#define GL_TEXTURE0                       0x84C0
#define GL_TEXTURE1                       0x84C1
#define GL_TEXTURE2                       0x84C2
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// ==================== ТИПЫ И КОНСТАНТЫ OPENGL ====================
typedef uint32_t GLuint;
typedef int32_t GLint;
typedef uint32_t GLenum;
typedef char GLchar;
typedef int32_t GLsizei;
typedef uint8_t GLboolean;
typedef float GLfloat;
typedef void GLvoid;
typedef intptr_t GLsizeiptr;
typedef uint32_t GLbitfield;

#define GL_COMPILE_STATUS                 0x8B81
#define GL_LINK_STATUS                    0x8B82
#define GL_VERTEX_SHADER                  0x8B31
#define GL_FRAGMENT_SHADER                0x8B30
#define GL_ARRAY_BUFFER                   0x8892
#define GL_ELEMENT_ARRAY_BUFFER           0x8893
#define GL_STATIC_DRAW                    0x88E4
#define GL_TRIANGLES                      0x0004
#define GL_UNSIGNED_INT                   0x1405
#define GL_FLOAT                          0x1406
#define GL_COLOR_BUFFER_BIT               0x4000
#define GL_DEPTH_BUFFER_BIT               0x0100
#define GL_FALSE                          0
#define GL_TRUE                           1
#define GL_TEXTURE0                       0x84C0
#define GL_TEXTURE_2D                     0x0DE1
#define GL_TEXTURE_WRAP_S                 0x2802
#define GL_TEXTURE_WRAP_T                 0x2803
#define GL_TEXTURE_MIN_FILTER             0x2801
#define GL_TEXTURE_MAG_FILTER             0x2800
#define GL_LINEAR                         0x2601
#define GL_REPEAT                         0x2901
#define GL_RGBA                           0x1908
#define GL_UNSIGNED_BYTE                  0x1401

// ==================== УКАЗАТЕЛИ НА ФУНКЦИИ ====================
typedef GLuint (*PFNGLCREATESHADERPROC)(GLenum type);
typedef void (*PFNGLSHADERSOURCEPROC)(GLuint shader, GLsizei count, const GLchar* const* string, const GLint* length);
typedef void (*PFNGLCOMPILESHADERPROC)(GLuint shader);
typedef void (*PFNGLGETSHADERIVPROC)(GLuint shader, GLenum pname, GLint* params);
typedef void (*PFNGLGETSHADERINFOLOGPROC)(GLuint shader, GLsizei bufSize, GLsizei* length, GLchar* infoLog);
typedef GLuint (*PFNGLCREATEPROGRAMPROC)(void);
typedef void (*PFNGLATTACHSHADERPROC)(GLuint program, GLuint shader);
typedef void (*PFNGLBINDATTRIBLOCATIONPROC)(GLuint program, GLuint index, const GLchar* name);
typedef void (*PFNGLLINKPROGRAMPROC)(GLuint program);
typedef void (*PFNGLGETPROGRAMIVPROC)(GLuint program, GLenum pname, GLint* params);
typedef void (*PFNGLGETPROGRAMINFOLOGPROC)(GLuint program, GLsizei bufSize, GLsizei* length, GLchar* infoLog);
typedef void (*PFNGLDELETESHADERPROC)(GLuint shader);
typedef void (*PFNGLUSEPROGRAMPROC)(GLuint program);
typedef GLint (*PFNGLGETUNIFORMLOCATIONPROC)(GLuint program, const GLchar* name);
typedef void (*PFNGLUNIFORM3FPROC)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
typedef void (*PFNGLUNIFORM1FPROC)(GLint location, GLfloat v0);
typedef void (*PFNGLUNIFORM1IPROC)(GLint location, GLint v0);
typedef void (*PFNGLGENVERTEXARRAYSPROC)(GLsizei n, GLuint* arrays);
typedef void (*PFNGLGENBUFFERSPROC)(GLsizei n, GLuint* buffers);
typedef void (*PFNGLBINDVERTEXARRAYPROC)(GLuint array);
typedef void (*PFNGLBINDBUFFERPROC)(GLenum target, GLuint buffer);
typedef void (*PFNGLBUFFERDATAPROC)(GLenum target, GLsizeiptr size, const void* data, GLenum usage);
typedef void (*PFNGLVERTEXATTRIBPOINTERPROC)(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer);
typedef void (*PFNGLENABLEVERTEXATTRIBARRAYPROC)(GLuint index);
typedef void (*PFNGLDRAWELEMENTSPROC)(GLenum mode, GLsizei count, GLenum type, const void* indices);
typedef void (*PFNGLDELETEVERTEXARRAYSPROC)(GLsizei n, const GLuint* arrays);
typedef void (*PFNGLDELETEBUFFERSPROC)(GLsizei n, const GLuint* buffers);
typedef void (*PFNGLDELETEPROGRAMPROC)(GLuint program);
typedef void (*PFNGLCLEARPROC)(GLbitfield mask);
typedef void (*PFNGLGENTEXTURESPROC)(GLsizei n, GLuint* textures);
typedef void (*PFNGLBINDTEXTUREPROC)(GLenum target, GLuint texture);
typedef void (*PFNGLTEXIMAGE2DPROC)(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void* pixels);
typedef void (*PFNGLTEXPARAMETERIPROC)(GLenum target, GLenum pname, GLint param);
typedef void (*PFNGLACTIVETEXTUREPROC)(GLenum texture);

static PFNGLCREATESHADERPROC pglCreateShader = nullptr;
static PFNGLSHADERSOURCEPROC pglShaderSource = nullptr;
static PFNGLCOMPILESHADERPROC pglCompileShader = nullptr;
static PFNGLGETSHADERIVPROC pglGetShaderiv = nullptr;
static PFNGLGETSHADERINFOLOGPROC pglGetShaderInfoLog = nullptr;
static PFNGLCREATEPROGRAMPROC pglCreateProgram = nullptr;
static PFNGLATTACHSHADERPROC pglAttachShader = nullptr;
static PFNGLBINDATTRIBLOCATIONPROC pglBindAttribLocation = nullptr;
static PFNGLLINKPROGRAMPROC pglLinkProgram = nullptr;
static PFNGLGETPROGRAMIVPROC pglGetProgramiv = nullptr;
static PFNGLGETPROGRAMINFOLOGPROC pglGetProgramInfoLog = nullptr;
static PFNGLDELETESHADERPROC pglDeleteShader = nullptr;
static PFNGLUSEPROGRAMPROC pglUseProgram = nullptr;
static PFNGLGETUNIFORMLOCATIONPROC pglGetUniformLocation = nullptr;
static PFNGLUNIFORM3FPROC pglUniform3f = nullptr;
static PFNGLUNIFORM1FPROC pglUniform1f = nullptr;
static PFNGLUNIFORM1IPROC pglUniform1i = nullptr;
static PFNGLGENVERTEXARRAYSPROC pglGenVertexArrays = nullptr;
static PFNGLGENBUFFERSPROC pglGenBuffers = nullptr;
static PFNGLBINDVERTEXARRAYPROC pglBindVertexArray = nullptr;
static PFNGLBINDBUFFERPROC pglBindBuffer = nullptr;
static PFNGLBUFFERDATAPROC pglBufferData = nullptr;
static PFNGLVERTEXATTRIBPOINTERPROC pglVertexAttribPointer = nullptr;
static PFNGLENABLEVERTEXATTRIBARRAYPROC pglEnableVertexAttribArray = nullptr;
static PFNGLDRAWELEMENTSPROC pglDrawElements = nullptr;
static PFNGLDELETEVERTEXARRAYSPROC pglDeleteVertexArrays = nullptr;
static PFNGLDELETEBUFFERSPROC pglDeleteBuffers = nullptr;
static PFNGLDELETEPROGRAMPROC pglDeleteProgram = nullptr;
static PFNGLCLEARPROC pglClear = nullptr;
static PFNGLGENTEXTURESPROC pglGenTextures = nullptr;
static PFNGLBINDTEXTUREPROC pglBindTexture = nullptr;
static PFNGLTEXIMAGE2DPROC pglTexImage2D = nullptr;
static PFNGLTEXPARAMETERIPROC pglTexParameteri = nullptr;
static PFNGLACTIVETEXTUREPROC pglActiveTexture = nullptr;

bool loadOpenGLFunctions() {
    pglCreateShader = (PFNGLCREATESHADERPROC)glfwGetProcAddress("glCreateShader");
    pglShaderSource = (PFNGLSHADERSOURCEPROC)glfwGetProcAddress("glShaderSource");
    pglCompileShader = (PFNGLCOMPILESHADERPROC)glfwGetProcAddress("glCompileShader");
    pglGetShaderiv = (PFNGLGETSHADERIVPROC)glfwGetProcAddress("glGetShaderiv");
    pglGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)glfwGetProcAddress("glGetShaderInfoLog");
    pglCreateProgram = (PFNGLCREATEPROGRAMPROC)glfwGetProcAddress("glCreateProgram");
    pglAttachShader = (PFNGLATTACHSHADERPROC)glfwGetProcAddress("glAttachShader");
    pglBindAttribLocation = (PFNGLBINDATTRIBLOCATIONPROC)glfwGetProcAddress("glBindAttribLocation");
    pglLinkProgram = (PFNGLLINKPROGRAMPROC)glfwGetProcAddress("glLinkProgram");
    pglGetProgramiv = (PFNGLGETPROGRAMIVPROC)glfwGetProcAddress("glGetProgramiv");
    pglGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)glfwGetProcAddress("glGetProgramInfoLog");
    pglDeleteShader = (PFNGLDELETESHADERPROC)glfwGetProcAddress("glDeleteShader");
    pglUseProgram = (PFNGLUSEPROGRAMPROC)glfwGetProcAddress("glUseProgram");
    pglGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)glfwGetProcAddress("glGetUniformLocation");
    pglUniform3f = (PFNGLUNIFORM3FPROC)glfwGetProcAddress("glUniform3f");
    pglUniform1f = (PFNGLUNIFORM1FPROC)glfwGetProcAddress("glUniform1f");
    pglUniform1i = (PFNGLUNIFORM1IPROC)glfwGetProcAddress("glUniform1i");
    pglGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)glfwGetProcAddress("glGenVertexArrays");
    pglGenBuffers = (PFNGLGENBUFFERSPROC)glfwGetProcAddress("glGenBuffers");
    pglBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)glfwGetProcAddress("glBindVertexArray");
    pglBindBuffer = (PFNGLBINDBUFFERPROC)glfwGetProcAddress("glBindBuffer");
    pglBufferData = (PFNGLBUFFERDATAPROC)glfwGetProcAddress("glBufferData");
    pglVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)glfwGetProcAddress("glVertexAttribPointer");
    pglEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)glfwGetProcAddress("glEnableVertexAttribArray");
    pglDrawElements = (PFNGLDRAWELEMENTSPROC)glfwGetProcAddress("glDrawElements");
    pglDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC)glfwGetProcAddress("glDeleteVertexArrays");
    pglDeleteBuffers = (PFNGLDELETEBUFFERSPROC)glfwGetProcAddress("glDeleteBuffers");
    pglDeleteProgram = (PFNGLDELETEPROGRAMPROC)glfwGetProcAddress("glDeleteProgram");
    pglClear = (PFNGLCLEARPROC)glfwGetProcAddress("glClear");
    pglGenTextures = (PFNGLGENTEXTURESPROC)glfwGetProcAddress("glGenTextures");
    pglBindTexture = (PFNGLBINDTEXTUREPROC)glfwGetProcAddress("glBindTexture");
    pglTexImage2D = (PFNGLTEXIMAGE2DPROC)glfwGetProcAddress("glTexImage2D");
    pglTexParameteri = (PFNGLTEXPARAMETERIPROC)glfwGetProcAddress("glTexParameteri");
    pglActiveTexture = (PFNGLACTIVETEXTUREPROC)glfwGetProcAddress("glActiveTexture");

    if (!pglCreateShader || !pglCreateProgram || !pglUseProgram || !pglGenVertexArrays) {
        fprintf(stderr, "Failed to load OpenGL functions via GLFW\n");
        return false;
    }
    return true;
}

// ==================== ШЕЙДЕРЫ ====================
const char* vertexShaderSource = R"glsl(
#version 330 core
layout (location = 0) in vec2 aPos;
out vec2 TexCoords;
void main() {
    TexCoords = aPos * 0.5 + 0.5;
    gl_Position = vec4(aPos, 0.0, 1.0);
}
)glsl";

const char* fragmentShaderSource = R"glsl(
#version 330 core
out vec4 FragColor;
in vec2 TexCoords;

uniform vec3 camPos;
uniform vec3 camDir;
uniform vec3 camRight;
uniform vec3 camUp;
uniform float fov;
uniform sampler2D wallTexture;
uniform sampler2D floorTexture;
uniform sampler2D skullTexture;

int getVoxel(vec3 p) {
    int x = int(floor(p.x));
    int y = int(floor(p.y));
    int z = int(floor(p.z));
    
    if (x < 0 || x >= 16 || z < 0 || z >= 16) return 1;
    if (y < 0 || y >= 5) return 2;
    
    return 0;
}

vec2 getUV(int voxel, vec3 hitPos, vec3 normal) {
    vec2 uv;
    if (voxel == 1) {
        // Стены - проекция на XZ
        if (abs(normal.x) > 0.5) {
            uv = vec2(hitPos.z, hitPos.y);
        } else {
            uv = vec2(hitPos.x, hitPos.y);
        }
    } else if (voxel == 2) {
        // Пол/потолок
        uv = vec2(hitPos.x, hitPos.z);
    } else {
        uv = vec2(0.0);
    }
    return uv;
}

void main() {
    vec2 retroRes = vec2(320.0, 180.0);
    vec2 pixelUV = floor(TexCoords * retroRes) / retroRes;
    vec2 uv = pixelUV * 2.0 - 1.0;
    uv.y = -uv.y;
    
    float aspect = 1280.0 / 720.0;
    vec3 rayDir = normalize(camDir + fov * uv.x * camRight * aspect + fov * uv.y * camUp);
    vec3 mapPos = floor(camPos);
    vec3 deltaDist = abs(vec3(1.0 / rayDir.x, 1.0 / rayDir.y, 1.0 / rayDir.z));
    if (rayDir.x == 0.0) deltaDist.x = 1e30;
    if (rayDir.y == 0.0) deltaDist.y = 1e30;
    if (rayDir.z == 0.0) deltaDist.z = 1e30;
    
    vec3 step;
    vec3 sideDist;
    
    if (rayDir.x < 0.0) { step.x = -1.0; sideDist.x = (camPos.x - mapPos.x) * deltaDist.x; }
    else { step.x = 1.0; sideDist.x = (mapPos.x + 1.0 - camPos.x) * deltaDist.x; }
    
    if (rayDir.y < 0.0) { step.y = -1.0; sideDist.y = (camPos.y - mapPos.y) * deltaDist.y; }
    else { step.y = 1.0; sideDist.y = (mapPos.y + 1.0 - camPos.y) * deltaDist.y; }
    
    if (rayDir.z < 0.0) { step.z = -1.0; sideDist.z = (camPos.z - mapPos.z) * deltaDist.z; }
    else { step.z = 1.0; sideDist.z = (mapPos.z + 1.0 - camPos.z) * deltaDist.z; }
    
    int voxel = 0;
    vec3 normal = vec3(0.0);
    float dist = 0.0;
    
    for (int i = 0; i < 64; ++i) {
        if (sideDist.x < sideDist.y && sideDist.x < sideDist.z) {
            mapPos.x += step.x; dist = sideDist.x; sideDist.x += deltaDist.x;
            normal = vec3(-step.x, 0.0, 0.0);
        } else if (sideDist.y < sideDist.z) {
            mapPos.y += step.y; dist = sideDist.y; sideDist.y += deltaDist.y;
            normal = vec3(0.0, -step.y, 0.0);
        } else {
            mapPos.z += step.z; dist = sideDist.z; sideDist.z += deltaDist.z;
            normal = vec3(0.0, 0.0, -step.z);
        }
        
        voxel = getVoxel(mapPos);
        if (voxel != 0) break;
    }
    
    if (voxel != 0) {
        vec3 hitPos = camPos + rayDir * dist;
        vec2 texUV = getUV(voxel, hitPos, normal);
        
        vec3 color;
        if (voxel == 1) {
            color = texture(wallTexture, texUV).rgb;
        } else if (voxel == 2) {
            if (normal.y > 0.5) {
                // Потолок - тёмный
                color = vec3(0.1, 0.1, 0.15);
            } else {
                // Пол
                color = texture(floorTexture, texUV).rgb;
            }
        } else {
            color = vec3(1.0, 0.0, 1.0);
        }
        
        // Освещение
        vec3 lightDir = normalize(vec3(0.5, 0.8, 0.3));
        float diff = max(dot(normal, lightDir), 0.0);
        float ambient = 0.3;
        color = color * (ambient + diff * 0.7);
        
        // Туман
        float fog = 1.0 - min(dist / 14.0, 1.0);
        color *= fog;
        
        // Прицел
        if (abs(uv.x) < 0.015 && abs(uv.y) < 0.015) color = vec3(1.0, 1.0, 1.0);
        
        FragColor = vec4(color, 1.0);
    } else {
        FragColor = vec4(0.05, 0.05, 0.08, 1.0);
    }
}
)glsl";

// ==================== ЗАГРУЗКА ТЕКСТУР ====================
GLuint loadTexture(const char* path) {
    GLuint texture;
    pglGenTextures(1, &texture);
    
    int width, height, channels;
    stbi_set_flip_vertically_on_load(1);
    unsigned char* data = stbi_load(path, &width, &height, &channels, 4);
    
    if (data) {
        pglBindTexture(GL_TEXTURE_2D, texture);
        pglTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        pglTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        pglTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        pglTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        pglTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        stbi_image_free(data);
        printf("Loaded texture: %s (%dx%d)\n", path, width, height);
    } else {
        printf("Failed to load texture: %s\n", path);
        stbi_image_free(data);
    }
    
    return texture;
}

// ==================== ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ ====================
unsigned int compileShader(unsigned int type, const char* source) {
    unsigned int id = pglCreateShader(type);
    pglShaderSource(id, 1, &source, NULL);
    pglCompileShader(id);
    int success;
    pglGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        pglGetShaderInfoLog(id, 512, NULL, infoLog);
        printf("Shader compilation error: %s\n", infoLog);
    }
    return id;
}

unsigned int createShaderProgram(const char* vertexSource, const char* fragmentSource) {
    unsigned int program = pglCreateProgram();
    unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexSource);
    unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentSource);
    pglAttachShader(program, vs);
    pglAttachShader(program, fs);
    pglBindAttribLocation(program, 0, "aPos");
    pglLinkProgram(program);
    int success;
    pglGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        pglGetProgramInfoLog(program, 512, NULL, infoLog);
        printf("Shader linking error: %s\n", infoLog);
    }
    pglDeleteShader(vs);
    pglDeleteShader(fs);
    return program;
}

// ==================== ГЛОБАЛЬНЫЕ ПЕРЕМЕННЫЕ ====================
float camX = 8.0f, camY = 2.5f, camZ = 2.0f;
float yaw = -90.0f, pitch = 0.0f;
bool firstMouse = true;
float lastX = 640.0f, lastY = 360.0f;
bool captureCursor = true;

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.15f;
    yaw += xoffset * sensitivity;
    pitch -= yoffset * sensitivity; // Инвертировано

    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;
}

// ==================== MAIN ====================
int main() {
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return 1;
    }

    const char* glsl_version = "#version 330 core";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(1280, 720, "DOOM 3D Voxel Raycaster", NULL, NULL);
    if (window == NULL) {
        fprintf(stderr, "Failed to create GLFW window\n");
        glfwTerminate();
        return 1;
    }
    
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (!loadOpenGLFunctions()) {
        fprintf(stderr, "Failed to load OpenGL functions\n");
        return 1;
    }

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    unsigned int shaderProgram = createShaderProgram(vertexShaderSource, fragmentShaderSource);

    // Загрузка текстур
    GLuint wallTex = loadTexture("textures/wall.png");
    GLuint floorTex = loadTexture("textures/floor.png");
    GLuint skullTex = loadTexture("textures/skull.png");

    float quadVertices[] = {
        -1.0f,  1.0f,
        -1.0f, -1.0f,
         1.0f, -1.0f,
         1.0f,  1.0f,
    };
    unsigned int quadIndices[] = { 0, 1, 2, 2, 3, 0 };
    unsigned int VAO, VBO, EBO;
    pglGenVertexArrays(1, &VAO);
    pglGenBuffers(1, &VBO);
    pglGenBuffers(1, &EBO);
    pglBindVertexArray(VAO);
    pglBindBuffer(GL_ARRAY_BUFFER, VBO);
    pglBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
    pglBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    pglBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quadIndices), quadIndices, GL_STATIC_DRAW);
    pglVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    pglEnableVertexAttribArray(0);

    bool show_demo_window = false;
    double lastFrame = 0.0;

    while (!glfwWindowShouldClose(window)) {
        double currentFrame = glfwGetTime();
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glfwPollEvents();

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            captureCursor = !captureCursor;
            glfwSetInputMode(window, GLFW_CURSOR, captureCursor ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
            firstMouse = true;
        }

        float velocity = 6.0f * deltaTime;
        float radYaw = yaw * 3.14159265f / 180.0f;
        float radPitch = pitch * 3.14159265f / 180.0f;

        float fx = cos(radPitch) * sin(radYaw);
	float fy = sin(radPitch);
	float fz = cos(radPitch) * cos(radYaw);

	float rx = cos(radYaw);
	float ry = 0.0f;
        float rz = -sin(radYaw);

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) { camX += fx * velocity; camY += fy * velocity; camZ += fz * velocity; }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) { camX -= fx * velocity; camY -= fy * velocity; camZ -= fz * velocity; }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) { camX -= rx * velocity; camZ -= rz * velocity; }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) { camX += rx * velocity; camZ += rz * velocity; }

        pglClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        pglUseProgram(shaderProgram);
        pglUniform3f(pglGetUniformLocation(shaderProgram, "camPos"), camX, camY, camZ);
        pglUniform3f(pglGetUniformLocation(shaderProgram, "camDir"), fx, fy, fz);
        pglUniform3f(pglGetUniformLocation(shaderProgram, "camRight"), rx, ry, rz);
        
        float ux = -sin(radPitch) * sin(radYaw);
	float uy = cos(radPitch);
	float uz = -sin(radPitch) * cos(radYaw);
        pglUniform3f(pglGetUniformLocation(shaderProgram, "camUp"), ux, uy, uz);
        pglUniform1f(pglGetUniformLocation(shaderProgram, "fov"), 1.0f);
        
        // Привязка текстур
        pglActiveTexture(GL_TEXTURE0);
        pglBindTexture(GL_TEXTURE_2D, wallTex);
        pglUniform1i(pglGetUniformLocation(shaderProgram, "wallTexture"), 0);
        
        pglActiveTexture(GL_TEXTURE1);
        pglBindTexture(GL_TEXTURE_2D, floorTex);
        pglUniform1i(pglGetUniformLocation(shaderProgram, "floorTexture"), 1);
        
        pglActiveTexture(GL_TEXTURE2);
        pglBindTexture(GL_TEXTURE_2D, skullTex);
        pglUniform1i(pglGetUniformLocation(shaderProgram, "skullTexture"), 2);

        pglBindVertexArray(VAO);
        pglDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("DOOM 3D Raycaster");
        ImGui::TextColored(ImVec4(1.0f, 0.2f, 0.2f, 1.0f), "3D VOXEL RAYCASTER ENGINE");
        ImGui::Separator();
        ImGui::Text("WASD : Движение");
        ImGui::Text("Мышь : Обзор");
        ImGui::Text("ESC  : Захват/освобождение курсора");
        ImGui::Separator();
        ImGui::Text("Camera Pos: X=%.2f Y=%.2f Z=%.2f", camX, camY, camZ);
        ImGui::Text("Yaw: %.1f | Pitch: %.1f", yaw, pitch);
        ImGui::Checkbox("Show ImGui Demo", &show_demo_window);
        ImGui::End();

        if (show_demo_window) ImGui::ShowDemoWindow(&show_demo_window);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    pglDeleteVertexArrays(1, &VAO);
    pglDeleteBuffers(1, &VBO);
    pglDeleteBuffers(1, &EBO);
    pglDeleteProgram(shaderProgram);

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
