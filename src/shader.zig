const std = @import("std");
const os = std.os;
const log = std.log;
const math = std.math;
const Allocator = std.mem.Allocator;
const cstr = std.cstr;
const c = @import("c.zig");
const zm = @import("zmath");

pub const ShaderType = enum {
    vertex,
    fragment,
};

pub const Shader = struct {
    const Self = @This();
    programId: u32,
    vertexId: u32,
    fragmentId: u32,

    pub fn init() Self {
        return Self{
            .programId = 0,
            .vertexId = 0,
            .fragmentId = 0,
        };
    }

    pub fn deinit(_: *Self) void {
        std.log.warn("Shader deinit is not doing anything", .{});
    }

    pub fn create(self: *Self, allocator: *Allocator, shader_type: ShaderType, filename: []const u8) void {
        var success: i32 = undefined;
        var shader: u32 = undefined;
        var opengl_shader = switch (shader_type) {
            ShaderType.vertex => c.GL_VERTEX_SHADER,
            ShaderType.fragment => c.GL_FRAGMENT_SHADER,
        };

        var c_str = cstr.addNullByte(allocator.*, filename) catch unreachable;
        defer allocator.free(c_str);
        var file = std.fs.cwd().openFile(c_str, .{}) catch unreachable;
        defer file.close();

        var src = file.readToEndAlloc(allocator.*, math.maxInt(usize)) catch {
            log.err("Could not read {s}: File too big", .{filename});
            return;
        };
        defer allocator.free(src);
        var cShaderSrc = cstr.addNullByte(allocator.*, src) catch unreachable;
        defer allocator.free(cShaderSrc);

        shader = c.glCreateShader(@intCast(c_uint, opengl_shader));
        c.glShaderSource(shader, 1, @ptrCast([*c]const [*c]const u8, &cShaderSrc), null);
        c.glCompileShader(shader);
        c.glGetShaderiv(shader, c.GL_COMPILE_STATUS, &success);

        if (success == 0) {
            var info_log: [512]u8 = undefined;
            c.glGetShaderInfoLog(shader, 512, null, info_log[0..]);

            log.err("Shader failed to compile: {s}", .{info_log});
        }

        switch (shader_type) {
            ShaderType.vertex => self.vertexId = shader,
            ShaderType.fragment => self.fragmentId = shader,
        }

        return;
    }

    pub fn createProgram(self: *Self) void {
        self.programId = c.glCreateProgram();
        c.glAttachShader(self.programId, self.vertexId);
        c.glAttachShader(self.programId, self.fragmentId);
        c.glLinkProgram(self.programId);

        c.glDeleteShader(self.vertexId);
        c.glDeleteShader(self.fragmentId);
    }

    pub fn use(self: *const Self) void {
        c.glUseProgram(self.programId);
    }

    pub fn set_mat4(self: *const Self, name: []const u8, mat: zm.Mat) void {
        var loc = self.getUniformLocation(name);
        c.glUniformMatrix4fv(loc, 1, c.GL_FALSE, @ptrCast([*c]const f32, &mat));
    }

    pub fn set_vec3(self: *const Self, name: []const u8, vec: zm.Vec) void {
        var loc = self.getUniformLocation(name);
        c.glUniform3fv(loc, 1, @ptrCast([*c]const f32, &vec));
    }

    fn getUniformLocation(self: *const Self, name: []const u8) i32 {
        var loc = c.glGetUniformLocation(self.programId, name.ptr);
        if (loc == -1) {
            std.log.info("Could not locate uniform '{s}'", .{name});
        }
        return loc;
    }

    pub fn set(_: []const u8, _: anytype) void {}
};
