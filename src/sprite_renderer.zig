const std = @import("std");
const Allocator = std.mem.Allocator;
const c = @import("c.zig");
const Shader = @import("shader.zig").Shader;
const Texture = @import("texture.zig").Texture;
const zm = @import("zmath");

pub const SpriteRenderer = struct {
    const Self = @This();
    shader: Shader,
    quad_vao: u32,

    pub fn init(allocator: *Allocator) Self {
        var shader = Shader.init();
        shader.create(allocator, .vertex, "resources/shaders/shader.vert");
        shader.create(allocator, .fragment, "resources/shaders/shader.frag");
        shader.createProgram();

        var vao: u32 = undefined;
        var vbo: u32 = undefined;
        var vertices = &[_]f32{
            // pos      // tex
            0.0, 1.0, 0.0, 1.0,
            1.0, 0.0, 1.0, 0.0,
            0.0, 0.0, 0.0, 0.0,

            0.0, 1.0, 0.0, 1.0,
            1.0, 1.0, 1.0, 1.0,
            1.0, 0.0, 1.0, 0.0,
        };

        c.glGenVertexArrays(1, &vao);
        c.glGenBuffers(1, &vbo);

        c.glBindBuffer(c.GL_ARRAY_BUFFER, vbo);
        c.glBufferData(c.GL_ARRAY_BUFFER, vertices.len * @sizeOf(f32), vertices, c.GL_STATIC_DRAW);

        c.glBindVertexArray(vao);
        c.glEnableVertexAttribArray(0);
        c.glVertexAttribPointer(0, 4, c.GL_FLOAT, c.GL_FALSE, 4 * @sizeOf(f32), null);
        c.glBindBuffer(c.GL_ARRAY_BUFFER, 0);
        c.glBindVertexArray(0);

        return Self{
            .quad_vao = vao,
            .shader = shader,
        };
    }

    pub fn deinit(self: *Self) void {
        c.glDeleteVertexArrays(self.quad_vao);
    }

    pub fn draw(self: *const Self, texture: u32, position: zm.Vec, scale: zm.Vec) void {
        self.shader.use();

        var model = zm.mul(zm.translationV(position), zm.scalingV(scale));
        self.shader.set_mat4("model", model);

        c.glActiveTexture(c.GL_TEXTURE0);
        Texture.bind(texture);

        c.glBindVertexArray(self.quad_vao);
        c.glDrawArrays(c.GL_TRIANGLES, 0, 6);
        c.glBindVertexArray(0);
    }
};
