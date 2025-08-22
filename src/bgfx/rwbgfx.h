#pragma once

#include <bgfx/bgfx.h>
#include "../rwbase.h"
#include "../rwrender.h"
#include "../rwobjects.h"

namespace rw {
namespace bgfx {

struct IndexBuffer {
    bgfx::IndexBufferHandle handle;
    explicit IndexBuffer(const bgfx::Memory* mem);
    ~IndexBuffer();
};

struct VertexBuffer {
    bgfx::VertexBufferHandle handle;
    bgfx::VertexLayout layout;
    VertexBuffer(const bgfx::Memory* mem, const bgfx::VertexLayout& layout);
    ~VertexBuffer();
};

struct Texture {
    bgfx::TextureHandle handle;
    Texture(uint16 width, uint16 height, const bgfx::Memory* mem);
    ~Texture();
};

struct Shader {
    bgfx::ProgramHandle program;
    Shader(bgfx::ShaderHandle vs, bgfx::ShaderHandle fs);
    ~Shader();
};

struct EngineOpenParams {
    uint16 width;
    uint16 height;
};

struct Im3DVertex
{
    V3d position;
    V3d normal;
    uint32 color;
    float32 u, v;

    void setX(float32 x) { this->position.x = x; }
    void setY(float32 y) { this->position.y = y; }
    void setZ(float32 z) { this->position.z = z; }
    void setNormalX(float32 x) { this->normal.x = x; }
    void setNormalY(float32 y) { this->normal.y = y; }
    void setNormalZ(float32 z) { this->normal.z = z; }
    void setColor(uint8 r, uint8 g, uint8 b, uint8 a) { this->color = (a<<24)|(r<<16)|(g<<8)|b; }
    void setU(float32 u) { this->u = u; }
    void setV(float32 v) { this->v = v; }
};

struct Im2DVertex
{
    float32 x, y, z;
    float32 w;
    uint32 color;
    float32 u, v;

    void setScreenX(float32 x) { this->x = x; }
    void setScreenY(float32 y) { this->y = y; }
    void setScreenZ(float32 z) { this->z = z; }
    void setCameraZ(float32 z) { this->w = z; }
    void setColor(uint8 r, uint8 g, uint8 b, uint8 a) { this->color = (a<<24)|(r<<16)|(g<<8)|b; }
    void setU(float32 u, float recipZ) { this->u = u; }
    void setV(float32 v, float recipZ) { this->v = v; }
};

extern RGBA im3dMaterialColor;
extern SurfaceProperties im3dSurfaceProps;

void initDevice(uint16 width, uint16 height);
void termDevice();

void* createIndexBuffer(uint32 length, bool dynamic = false);
void destroyIndexBuffer(void *indexBuffer);

void* createVertexBuffer(uint32 length, bool dynamic = false);
void destroyVertexBuffer(void *vertexBuffer);

void setViewTransform(int id, const float *view, const float *proj);

void drawIndexed(bgfx::ProgramHandle program,
                 bgfx::VertexBufferHandle vbo,
                 bgfx::IndexBufferHandle ibo,
                 uint32 numIndices);

void setRenderState(int32 state, void *value);
void *getRenderState(int32 state);

void im2DRenderLine(void *vertices, int32 numVertices, int32 vert1, int32 vert2);
void im2DRenderTriangle(void *vertices, int32 numVertices, int32 vert1, int32 vert2, int32 vert3);
void im2DRenderPrimitive(PrimitiveType primType, void *vertices, int32 numVertices);
void im2DRenderIndexedPrimitive(PrimitiveType primType, void *vertices, int32 numVertices, void *indices, int32 numIndices);

void im3DTransform(void *vertices, int32 numVertices, Matrix *world, uint32 flags);
void im3DRenderPrimitive(PrimitiveType primType);
void im3DRenderIndexedPrimitive(PrimitiveType primType, void *indices, int32 numIndices);
void im3DEnd(void);

void beginUpdate(Camera *cam);
void endUpdate(Camera *cam);
void clearCamera(Camera *cam, RGBA *col, uint32 mode);
void showRaster(Raster *raster, uint32 flags);
bool32 rasterRenderFast(Raster *raster, int32 x, int32 y);

extern Device renderdevice;

} // namespace bgfx
} // namespace rw

