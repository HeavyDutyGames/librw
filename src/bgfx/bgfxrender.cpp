#include "rwbgfx.h"
#include "rwbgfximpl.h"
#include "../rwrender.h"
#include "../rwengine.h"
#include "../rwobjects.h"

namespace rw {
namespace bgfx {

RGBA im3dMaterialColor = { 255, 255, 255, 255 };
SurfaceProperties im3dSurfaceProps = { 1.0f, 1.0f, 1.0f, 1.0f };

void setRenderState(int32 state, void *pvalue)
{
    uint32 value = (uint32)(uintptr)pvalue;
    switch(state){
    case VERTEXALPHA:
        if(value)
            bgfxGlobals.state |= BGFX_STATE_BLEND_ALPHA;
        else
            bgfxGlobals.state &= ~BGFX_STATE_BLEND_MASK;
        break;
    case ZWRITEENABLE:
        if(value)
            bgfxGlobals.state |= BGFX_STATE_WRITE_Z;
        else
            bgfxGlobals.state &= ~BGFX_STATE_WRITE_Z;
        break;
    case CULLMODE:
        if(value == CULLBACK){
            bgfxGlobals.state |= BGFX_STATE_CULL_CCW;
            bgfxGlobals.state &= ~BGFX_STATE_CULL_CW;
        }else if(value == CULLFRONT){
            bgfxGlobals.state |= BGFX_STATE_CULL_CW;
            bgfxGlobals.state &= ~BGFX_STATE_CULL_CCW;
        }else{
            bgfxGlobals.state &= ~(BGFX_STATE_CULL_CW|BGFX_STATE_CULL_CCW);
        }
        break;
    default:
        break;
    }
}

void* getRenderState(int32)
{
    return nil;
}

static bgfx::VertexLayout im2dLayout;
static bgfx::VertexLayout im3dLayout;

static void ensureLayouts()
{
    if(im2dLayout.getStride() == 0){
        im2dLayout.begin()
            .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
            .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
            .end();
    }
    if(im3dLayout.getStride() == 0){
        im3dLayout.begin()
            .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::Normal, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
            .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
            .end();
    }
}

float32 im2DGetNearZ(void) { return 0.0f; }
float32 im2DGetFarZ(void) { return 1.0f; }

void im2DRenderLine(void *verts, int32 numVerts, int32 vert1, int32 vert2)
{
    Im2DVertex *v = (Im2DVertex*)verts;
    Im2DVertex tmp[2];
    tmp[0] = v[vert1];
    tmp[1] = v[vert2];
    im2DRenderPrimitive(PRIMTYPELINELIST, tmp, 2);
}

void im2DRenderTriangle(void *verts, int32 numVerts, int32 v1, int32 v2, int32 v3)
{
    Im2DVertex *v = (Im2DVertex*)verts;
    Im2DVertex tmp[3];
    tmp[0] = v[v1];
    tmp[1] = v[v2];
    tmp[2] = v[v3];
    im2DRenderPrimitive(PRIMTYPETRILIST, tmp, 3);
}

void im2DRenderPrimitive(PrimitiveType primType, void *verts, int32 numVerts)
{
    ensureLayouts();
    const bgfx::Memory* vmem = bgfx::copy(verts, numVerts*sizeof(Im2DVertex));
    bgfx::VertexBufferHandle vbh = bgfx::createVertexBuffer(vmem, im2dLayout);
    bgfx::setVertexBuffer(0, vbh);
    bgfx::setState(bgfxGlobals.state);
    bgfx::submit(bgfxGlobals.viewId, BGFX_INVALID_HANDLE);
    bgfx::destroy(vbh);
}

void im2DRenderIndexedPrimitive(PrimitiveType primType, void *verts, int32 numVerts, void *indices, int32 numIndices)
{
    ensureLayouts();
    const bgfx::Memory* vmem = bgfx::copy(verts, numVerts*sizeof(Im2DVertex));
    const bgfx::Memory* imem = bgfx::copy(indices, numIndices*sizeof(uint16));
    bgfx::VertexBufferHandle vbh = bgfx::createVertexBuffer(vmem, im2dLayout);
    bgfx::IndexBufferHandle ibh = bgfx::createIndexBuffer(imem);
    bgfx::setVertexBuffer(0, vbh);
    bgfx::setIndexBuffer(ibh);
    bgfx::setState(bgfxGlobals.state);
    bgfx::submit(bgfxGlobals.viewId, BGFX_INVALID_HANDLE);
    bgfx::destroy(vbh);
    bgfx::destroy(ibh);
}

void im3DTransform(void*, int32, Matrix*, uint32)
{
}

void im3DRenderPrimitive(PrimitiveType)
{
}

void im3DRenderIndexedPrimitive(PrimitiveType, void*, int32)
{
}

void im3DEnd(void)
{
}

} // namespace bgfx
} // namespace rw

