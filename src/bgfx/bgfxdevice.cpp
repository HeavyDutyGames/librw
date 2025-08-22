#include <string.h>
#include "rwbgfx.h"
#include "rwbgfximpl.h"
#include "../rwbase.h"
#include "../rwplg.h"
#include "../rwerror.h"
#include "../rwrender.h"
#include "../rwengine.h"
#include "../rwpipeline.h"
#include "../rwobjects.h"

namespace rw {
namespace bgfx {

void beginUpdate(Camera *cam)
{
    float view[16], proj[16];
    Matrix inv;
    Matrix::invert(&inv, cam->getFrame()->getLTM());
    view[0]  = -inv.right.x;
    view[1]  =  inv.right.y;
    view[2]  =  inv.right.z;
    view[3]  =  0.0f;
    view[4]  = -inv.up.x;
    view[5]  =  inv.up.y;
    view[6]  =  inv.up.z;
    view[7]  =  0.0f;
    view[8]  =  -inv.at.x;
    view[9]  =   inv.at.y;
    view[10] =  inv.at.z;
    view[11] =  0.0f;
    view[12] = -inv.pos.x;
    view[13] =  inv.pos.y;
    view[14] =  inv.pos.z;
    view[15] =  1.0f;
    memcpy(&cam->devView, view, sizeof(view));

    float invwx = 1.0f/cam->viewWindow.x;
    float invwy = 1.0f/cam->viewWindow.y;
    float invz = 1.0f/(cam->farPlane-cam->nearPlane);
    proj[0] = invwx;
    proj[1] = 0.0f;
    proj[2] = 0.0f;
    proj[3] = 0.0f;
    proj[4] = 0.0f;
    proj[5] = invwy;
    proj[6] = 0.0f;
    proj[7] = 0.0f;
    proj[8] = cam->viewOffset.x*invwx;
    proj[9] = cam->viewOffset.y*invwy;
    proj[12] = -proj[8];
    proj[13] = -proj[9];
    if(cam->projection == Camera::PERSPECTIVE){
        proj[10] = (cam->farPlane+cam->nearPlane)*invz;
        proj[11] = 1.0f;
        proj[14] = -2.0f*cam->nearPlane*cam->farPlane*invz;
        proj[15] = 0.0f;
    }else{
        proj[10] = 2.0f*invz;
        proj[11] = 0.0f;
        proj[14] = -(cam->farPlane+cam->nearPlane)*invz;
        proj[15] = 1.0f;
    }
    memcpy(&cam->devProj, proj, sizeof(proj));

    bgfx::setViewRect(bgfxGlobals.viewId, 0, 0, cam->frameBuffer->width, cam->frameBuffer->height);
    bgfx::setViewTransform(bgfxGlobals.viewId, view, proj);
}

void endUpdate(Camera*)
{
    bgfx::touch(bgfxGlobals.viewId);
}

void initDevice(uint16 width, uint16 height)
{
    if(bgfxGlobals.initialized)
        return;
    bgfx::Init init;
    init.type = bgfx::RendererType::Count;
    init.resolution.width = width;
    init.resolution.height = height;
    init.resolution.reset = BGFX_RESET_VSYNC;
    bgfx::init(init);
    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x000000ff, 1.0f, 0);
    bgfxGlobals.initialized = true;
    bgfxGlobals.width = width;
    bgfxGlobals.height = height;
    bgfxGlobals.viewId = 0;
    bgfxGlobals.state = BGFX_STATE_DEFAULT;
}

void termDevice()
{
    if(!bgfxGlobals.initialized)
        return;
    bgfx::shutdown();
    bgfxGlobals.initialized = false;
}

void clearCamera(Camera *cam, RGBA *col, uint32 mode)
{
    uint16 flags = 0;
    if(mode & Camera::CLEARIMAGE) flags |= BGFX_CLEAR_COLOR;
    if(mode & Camera::CLEARZ) flags |= BGFX_CLEAR_DEPTH;
    if(mode & Camera::CLEARSTENCIL) flags |= BGFX_CLEAR_STENCIL;
    uint32 color = col ? (col->red | (col->green<<8) | (col->blue<<16) | (col->alpha<<24)) : 0;
    bgfx::setViewClear(bgfxGlobals.viewId, flags, color, 1.0f, 0);
    bgfx::touch(bgfxGlobals.viewId);
}

void showRaster(Raster*, uint32)
{
    bgfx::frame();
}

bool32 rasterRenderFast(Raster*, int32, int32)
{
    return 0;
}

void setViewTransform(int id, const float *view, const float *proj)
{
    bgfx::setViewTransform((uint8)id, view, proj);
}

void drawIndexed(bgfx::ProgramHandle program,
                 bgfx::VertexBufferHandle vbo,
                 bgfx::IndexBufferHandle ibo,
                 uint32 numIndices)
{
    bgfx::setVertexBuffer(0, vbo);
    bgfx::setIndexBuffer(ibo, 0, numIndices);
    bgfx::setState(bgfxGlobals.state);
    bgfx::submit(bgfxGlobals.viewId, program);
}

Device renderdevice = {
    -1.0f, 1.0f,
    beginUpdate,
    endUpdate,
    clearCamera,
    showRaster,
    rasterRenderFast,
    setRenderState,
    getRenderState,
    im2DRenderLine,
    im2DRenderTriangle,
    im2DRenderPrimitive,
    im2DRenderIndexedPrimitive,
    im3DTransform,
    im3DRenderPrimitive,
    im3DRenderIndexedPrimitive,
    im3DEnd,
    nil
};

} // namespace bgfx
} // namespace rw

