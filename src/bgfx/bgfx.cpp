#include "rwbgfx.h"
#include "rwbgfximpl.h"

namespace rw {
namespace bgfx {

Globals bgfxGlobals;

IndexBuffer::IndexBuffer(const bgfx::Memory* mem)
{
    handle = bgfx::createIndexBuffer(mem);
}

IndexBuffer::~IndexBuffer()
{
    if(bgfx::isValid(handle))
        bgfx::destroy(handle);
}

VertexBuffer::VertexBuffer(const bgfx::Memory* mem, const bgfx::VertexLayout& layout)
    : layout(layout)
{
    handle = bgfx::createVertexBuffer(mem, layout);
}

VertexBuffer::~VertexBuffer()
{
    if(bgfx::isValid(handle))
        bgfx::destroy(handle);
}

Texture::Texture(uint16 width, uint16 height, const bgfx::Memory* mem)
{
    handle = bgfx::createTexture2D(width, height, false, 1, bgfx::TextureFormat::RGBA8, 0, mem);
}

Texture::~Texture()
{
    if(bgfx::isValid(handle))
        bgfx::destroy(handle);
}

Shader::Shader(bgfx::ShaderHandle vs, bgfx::ShaderHandle fs)
{
    program = bgfx::createProgram(vs, fs, true);
}

Shader::~Shader()
{
    if(bgfx::isValid(program))
        bgfx::destroy(program);
}

void* createIndexBuffer(uint32 length, bool dynamic)
{
    (void)dynamic;
    const bgfx::Memory* mem = bgfx::alloc(length);
    return new IndexBuffer(mem);
}

void destroyIndexBuffer(void* indexBuffer)
{
    delete static_cast<IndexBuffer*>(indexBuffer);
}

void* createVertexBuffer(uint32 length, bool dynamic)
{
    (void)dynamic;
    bgfx::VertexLayout layout;
    layout.begin().add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float).end();
    const bgfx::Memory* mem = bgfx::alloc(length);
    return new VertexBuffer(mem, layout);
}

void destroyVertexBuffer(void* vertexBuffer)
{
    delete static_cast<VertexBuffer*>(vertexBuffer);
}

} // namespace bgfx
} // namespace rw

