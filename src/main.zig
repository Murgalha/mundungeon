const std = @import("std");
const log = std.log;
const c = @import("c.zig");
const App = @import("app.zig").App;
const Clock = @import("clock.zig").Clock;
const Texture = @import("texture.zig").Texture;
const SpriteRenderer = @import("sprite_renderer.zig").SpriteRenderer;
const Shader = @import("shader.zig").Shader;
const ShaderType = @import("shader.zig").ShaderType;
const Game = @import("game.zig").Game;
const zm = @import("zmath");
const stderr = std.io.getStdErr().writer();

const SCREEN_WIDTH: i32 = 800;
const SCREEN_HEIGHT: i32 = 600;

fn messageCallbackOpenGL(
    source: c.GLenum,
    messageType: c.GLenum,
    _: c.GLuint,
    severity: c.GLenum,
    _: c.GLsizei,
    message: [*c]const c.GLchar,
    _: ?*const anyopaque,
) callconv(.C) void {
    var sourceStr: []const u8 = undefined;
    var typeStr: []const u8 = undefined;
    var severityStr: []const u8 = undefined;

    switch (source) {
        c.GL_DEBUG_SOURCE_API => sourceStr = "API",
        c.GL_DEBUG_SOURCE_WINDOW_SYSTEM => sourceStr = "Window System",
        c.GL_DEBUG_SOURCE_SHADER_COMPILER => sourceStr = "Shader Compiler",
        c.GL_DEBUG_SOURCE_THIRD_PARTY => sourceStr = "Third Party",
        c.GL_DEBUG_SOURCE_APPLICATION => sourceStr = "Application",
        c.GL_DEBUG_SOURCE_OTHER => sourceStr = "Other",
        else => sourceStr = "Unknown",
    }

    switch (messageType) {
        c.GL_DEBUG_TYPE_ERROR => typeStr = "Error",
        c.GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR => typeStr = "Deprecated Behaviour",
        c.GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR => typeStr = "Undefined Behaviour",
        c.GL_DEBUG_TYPE_PORTABILITY => typeStr = "Portability",
        c.GL_DEBUG_TYPE_PERFORMANCE => typeStr = "Performance",
        c.GL_DEBUG_TYPE_MARKER => typeStr = "Marker",
        c.GL_DEBUG_TYPE_PUSH_GROUP => typeStr = "Push Group",
        c.GL_DEBUG_TYPE_POP_GROUP => typeStr = "Pop Group",
        c.GL_DEBUG_TYPE_OTHER => typeStr = "Other",
        else => typeStr = "Unknown",
    }

    switch (severity) {
        c.GL_DEBUG_SEVERITY_HIGH => severityStr = "High",
        c.GL_DEBUG_SEVERITY_MEDIUM => severityStr = "Medium",
        c.GL_DEBUG_SEVERITY_LOW => severityStr = "Low",
        c.GL_DEBUG_SEVERITY_NOTIFICATION => severityStr = "Notification",
        else => severityStr = "Unknown",
    }

    if (severity != c.GL_DEBUG_SEVERITY_NOTIFICATION) {
        stderr.print("\nSource: {s}\nSeverity: {s}\n", .{ sourceStr, severityStr }) catch unreachable;
        stderr.print("Type: {s}\nMessage: {s}\n", .{ typeStr, message }) catch unreachable;
    }
}

pub fn main() anyerror!void {
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    var allocator = gpa.allocator();
    defer _ = gpa.deinit();

    var app = App.init(SCREEN_WIDTH, SCREEN_HEIGHT);
    defer app.deinit();

    _ = c.SDL_GL_SetSwapInterval(1);
    c.glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    c.glEnable(c.GL_BLEND);
    c.glBlendFunc(c.GL_SRC_ALPHA, c.GL_ONE_MINUS_SRC_ALPHA);

    // Enable debug output
    c.glEnable(c.GL_DEBUG_OUTPUT);
    c.glDebugMessageCallback(messageCallbackOpenGL, null);

    var game = try Game.init(&allocator);
    defer game.deinit();

    while (!app.shouldQuit) {
        app.processInput(&game);

        game.update();

        c.glClearColor(0.1, 0.2, 0.1, 1.0);
        c.glClear(c.GL_COLOR_BUFFER_BIT);

        try game.draw();

        c.SDL_GL_SwapWindow(app.window);
    }
}
