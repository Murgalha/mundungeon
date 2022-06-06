const log = @import("std").log;
const c = @import("c.zig");
const SDL_Window = c.SDL_Window;
const SDL_GLContext = c.SDL_GLContext;
const Game = @import("game.zig").Game;

pub const App = struct {
    const Self = @This();
    window: ?*SDL_Window,
    openglContext: SDL_GLContext,
    shouldQuit: bool,

    pub fn init(width: i32, height: i32) Self {
        if (c.SDL_InitSubSystem(c.SDL_INIT_EVERYTHING ^ c.SDL_INIT_HAPTIC) != 0) {
            log.err("Could not initialize SDL: {s}", .{c.SDL_GetError()});
        }

        var window = createWindow(width, height);
        var context = createOpenGLContext(window);

        _ = c.gladLoadGL();

        return Self{
            .window = window,
            .openglContext = context,
            .shouldQuit = false,
        };
    }

    pub fn deinit(self: *const Self) void {
        c.SDL_GL_DeleteContext(self.openglContext);
        c.SDL_DestroyWindow(self.window);
        c.SDL_Quit();
    }

    pub fn processInput(self: *Self, game: *Game) void {
        var e: c.SDL_Event = undefined;

        while (c.SDL_PollEvent(&e) != 0) {
            switch (e.@"type") {
                c.SDL_WINDOWEVENT => {
                    switch (e.window.event) {
                        c.SDL_WINDOWEVENT_RESIZED => {
                            resizeViewport(e.window.data1, e.window.data2);
                        },
                        else => {},
                    }
                },
                c.SDL_QUIT => {
                    self.shouldQuit = true;
                },
                else => game.processEvent(e),
            }
        }
    }

    fn createWindow(width: i32, height: i32) ?*SDL_Window {
        var flags = c.SDL_WINDOW_OPENGL | c.SDL_WINDOW_RESIZABLE;

        var window = c.SDL_CreateWindow(
            "Mundungeon",
            c.SDL_WINDOWPOS_CENTERED,
            c.SDL_WINDOWPOS_CENTERED,
            width,
            height,
            @intCast(u32, flags),
        );

        if (window == null) {
            log.err("Could not create window: {s}", .{c.SDL_GetError()});
        }

        return window;
    }

    fn createOpenGLContext(window: ?*SDL_Window) SDL_GLContext {
        _ = c.SDL_GL_SetAttribute(c.SDL_GL_DOUBLEBUFFER, 1);
        _ = c.SDL_GL_SetAttribute(c.SDL_GL_ACCELERATED_VISUAL, 1);
        _ = c.SDL_GL_SetAttribute(c.SDL_GL_RED_SIZE, 8);
        _ = c.SDL_GL_SetAttribute(c.SDL_GL_GREEN_SIZE, 8);
        _ = c.SDL_GL_SetAttribute(c.SDL_GL_BLUE_SIZE, 8);
        _ = c.SDL_GL_SetAttribute(c.SDL_GL_ALPHA_SIZE, 8);
        _ = c.SDL_GL_SetAttribute(c.SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        _ = c.SDL_GL_SetAttribute(c.SDL_GL_CONTEXT_MINOR_VERSION, 3);
        _ = c.SDL_GL_SetAttribute(c.SDL_GL_CONTEXT_PROFILE_MASK, c.SDL_GL_CONTEXT_PROFILE_CORE);

        var context = c.SDL_GL_CreateContext(window);
        if (context == null) {
            log.err("Could not create context: {s}", .{c.SDL_GetError()});
        }

        return context;
    }

    fn resizeViewport(width: i32, height: i32) void {
        c.glViewport(0, 0, width, height);
    }
};
