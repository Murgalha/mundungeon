pub usingnamespace @cImport({
    @cInclude("stb_image.h");
    @cDefine("GL_GLEXT_PROTOTYPES", "");
    @cInclude("glad/glad.h");
    @cInclude("SDL2/SDL.h");
    @cInclude("SDL2/SDL_opengl.h");
    @cInclude("SDL2/SDL_opengl_glext.h");

    @cInclude("dungeon_generator.h");

    @cInclude("ft2build.h");
    @cInclude("freetype/freetype.h");
});
