const std = @import("std");
const c = @import("c.zig");
const log = std.log;
const Allocator = std.mem.Allocator;
const cstr = std.cstr;

pub const Texture = struct {
    pub fn new(allocator: *Allocator, filename: []const u8) !u32 {
        var texture: u32 = undefined;
        var width: i32 = undefined;
        var height: i32 = undefined;
        var nchannels: i32 = undefined;

        c.stbi_set_flip_vertically_on_load(@intCast(c_int, 1));
        var cFilename = try cstr.addNullByte(allocator.*, filename);
        defer allocator.free(cFilename);
        var data = c.stbi_load(cFilename, &width, &height, &nchannels, 0);

        if (data == null) {
            log.err("Could not load {s}", .{filename});
        }

        c.glGenTextures(1, &texture);
        c.glBindTexture(c.GL_TEXTURE_2D, texture);

        c.glTexParameteri(c.GL_TEXTURE_2D, c.GL_TEXTURE_WRAP_S, c.GL_REPEAT);
        c.glTexParameteri(c.GL_TEXTURE_2D, c.GL_TEXTURE_WRAP_T, c.GL_REPEAT);
        c.glTexParameteri(c.GL_TEXTURE_2D, c.GL_TEXTURE_MIN_FILTER, c.GL_LINEAR);
        c.glTexParameteri(c.GL_TEXTURE_2D, c.GL_TEXTURE_MAG_FILTER, c.GL_LINEAR);

        c.glTexImage2D(
            c.GL_TEXTURE_2D,
            0,
            c.GL_RGBA,
            width,
            height,
            0,
            c.GL_RGBA,
            c.GL_UNSIGNED_BYTE,
            data,
        );
        c.glBindTexture(c.GL_TEXTURE_2D, 0);

        c.stbi_image_free(data);

        return texture;
    }

    pub fn bind(id: u32) void {
        c.glBindTexture(c.GL_TEXTURE_2D, id);
    }
};
