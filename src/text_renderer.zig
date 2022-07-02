const std = @import("std");
const zm = @import("zmath");
const c = @import("c.zig");
const Shader = @import("shader.zig").Shader;
const log = std.log;
const Allocator = std.mem.Allocator;
const AutoHashMap = std.AutoHashMap;

const Character = struct {
    texture: u32,
    size: zm.Vec,
    bearing: zm.Vec,
    advance: u64,
};

pub const TextRenderer = struct {
    const Self = @This();
    characters: AutoHashMap(u8, Character),
    shader: Shader,
    vao: u32,
    vbo: u32,

    pub fn init(allocator: *Allocator) !Self {
        var characters = try createCharacters(allocator);
        var shader = Shader.init();
        shader.create(allocator, .vertex, "resources/shaders/text.vert");
        shader.create(allocator, .fragment, "resources/shaders/text.frag");
        shader.createProgram();

        shader.use();
        var projection = zm.orthographicRh(800.0, 600.0, -10.0, 10.0);
        shader.set_mat4("projection", projection);

        var VAO: u32 = undefined;
        var VBO: u32 = undefined;

        c.glGenVertexArrays(1, &VAO);
        c.glGenBuffers(1, &VBO);
        c.glBindVertexArray(VAO);
        c.glBindBuffer(c.GL_ARRAY_BUFFER, VBO);
        c.glBufferData(c.GL_ARRAY_BUFFER, @sizeOf(f32) * 6 * 4, null, c.GL_DYNAMIC_DRAW);
        c.glEnableVertexAttribArray(0);
        c.glVertexAttribPointer(0, 4, c.GL_FLOAT, c.GL_FALSE, 4 * @sizeOf(f32), null);
        c.glBindBuffer(c.GL_ARRAY_BUFFER, 0);
        c.glBindVertexArray(0);

        return Self{
            .characters = characters,
            .shader = shader,
            .vao = VAO,
            .vbo = VBO,
        };
    }

    pub fn deinit(self: *Self) void {
        self.characters.deinit();
        self.shader.deinit();
    }

    pub fn draw(self: *Self, text: []const u8, x: f32, y: f32, scale: f32, color: zm.Vec) void {
        // activate corresponding render state
        var x_tmp = x;
        self.shader.use();
        self.shader.set_vec3("textColor", color);

        c.glActiveTexture(c.GL_TEXTURE0);
        c.glBindVertexArray(self.vao);

        for (text) |char| {
            var nullableCharacter = self.characters.get(char);
            var character: Character = undefined;
            if (nullableCharacter) |value| {
                character = value;
            } else {
                character = self.characters.get('$').?;
            }

            var xpos = x_tmp + character.bearing[0] * scale;
            var ypos = y - (character.size[1] - character.bearing[1]) * scale;

            var w = character.size[0] * scale;
            var h = character.size[1] * scale;
            // update VBO for each character
            var vertices = &[_][4]f32{
                [_]f32{ xpos, ypos + h, 0.0, 0.0 },
                [_]f32{ xpos, ypos, 0.0, 1.0 },
                [_]f32{ xpos + w, ypos, 1.0, 1.0 },
                [_]f32{ xpos, ypos + h, 0.0, 0.0 },
                [_]f32{ xpos + w, ypos, 1.0, 1.0 },
                [_]f32{ xpos + w, ypos + h, 1.0, 0.0 },
            };
            // render glyph texture over quad
            c.glBindTexture(c.GL_TEXTURE_2D, character.texture);
            // update content of VBO memory
            c.glBindBuffer(c.GL_ARRAY_BUFFER, self.vbo);
            c.glBufferSubData(c.GL_ARRAY_BUFFER, 0, @sizeOf(f32) * 6 * 4, vertices); // be sure to use glBufferSubData and not glBufferData

            c.glBindBuffer(c.GL_ARRAY_BUFFER, 0);
            // render quad
            c.glDrawArrays(c.GL_TRIANGLES, 0, 6);
            // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
            x_tmp += @intToFloat(f32, character.advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
        }
        c.glBindVertexArray(0);
        c.glBindTexture(c.GL_TEXTURE_2D, 0);
    }

    fn createCharacters(allocator: *Allocator) !AutoHashMap(u8, Character) {
        var characters = AutoHashMap(u8, Character).init(allocator.*);
        var ft: c.FT_Library = undefined;

        if (c.FT_Init_FreeType(&ft) != 0) {
            @panic("FREETYPE: Could not init FreeType Library");
        }

        var path = "resources/fonts/DejaVuSans.ttf";

        var face: c.FT_Face = undefined;
        if (c.FT_New_Face(ft, path, 0, &face) != 0) {
            @panic("FREETYPE: Failed to load font");
        } else {
            _ = c.FT_Set_Pixel_Sizes(face, 0, 48);
            c.glPixelStorei(c.GL_UNPACK_ALIGNMENT, 1);

            var char: u8 = 0;
            while (char < 128) : (char += 1) {
                if (c.FT_Load_Char(face, char, c.FT_LOAD_RENDER) != 0) {
                    log.err("FREETYTPE: Failed to load Glyph", .{});
                    continue;
                }

                var texture: u32 = undefined;
                c.glGenTextures(1, &texture);
                c.glBindTexture(c.GL_TEXTURE_2D, texture);
                c.glTexImage2D(
                    c.GL_TEXTURE_2D,
                    0,
                    c.GL_RED,
                    @intCast(i32, face.*.glyph.*.bitmap.width),
                    @intCast(i32, face.*.glyph.*.bitmap.rows),
                    0,
                    c.GL_RED,
                    c.GL_UNSIGNED_BYTE,
                    face.*.glyph.*.bitmap.buffer,
                );

                c.glTexParameteri(c.GL_TEXTURE_2D, c.GL_TEXTURE_WRAP_S, c.GL_CLAMP_TO_EDGE);
                c.glTexParameteri(c.GL_TEXTURE_2D, c.GL_TEXTURE_WRAP_T, c.GL_CLAMP_TO_EDGE);
                c.glTexParameteri(c.GL_TEXTURE_2D, c.GL_TEXTURE_MIN_FILTER, c.GL_LINEAR);
                c.glTexParameteri(c.GL_TEXTURE_2D, c.GL_TEXTURE_MAG_FILTER, c.GL_LINEAR);

                var character = Character{
                    .texture = texture,
                    .size = zm.f32x4(
                        @intToFloat(f32, face.*.glyph.*.bitmap.width),
                        @intToFloat(f32, face.*.glyph.*.bitmap.rows),
                        0.0,
                        0.0,
                    ),
                    .bearing = zm.f32x4(
                        @intToFloat(f32, face.*.glyph.*.bitmap_left),
                        @intToFloat(f32, face.*.glyph.*.bitmap_top),
                        0.0,
                        0.0,
                    ),
                    .advance = @intCast(u64, face.*.glyph.*.advance.x),
                };
                try characters.put(char, character);
            }
            c.glBindTexture(c.GL_TEXTURE_2D, 0);
        }

        _ = c.FT_Done_Face(face);
        _ = c.FT_Done_FreeType(ft);

        return characters;
    }
};
