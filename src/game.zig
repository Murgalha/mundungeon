const std = @import("std");
const c = @import("c.zig");
const fmt = std.fmt;
const Allocator = std.mem.Allocator;
const zm = @import("zmath");
const SpriteRenderer = @import("sprite_renderer.zig").SpriteRenderer;
const TextRenderer = @import("text_renderer.zig").TextRenderer;
const Camera = @import("camera.zig").Camera;
const Texture = @import("texture.zig").Texture;
const Dungeon = @import("dungeon.zig").Dungeon;
const Grid = @import("grid.zig").Grid;
const Hero = @import("hero.zig").Hero;
const Enemy = @import("enemy.zig").Enemy;
const Clock = @import("clock.zig").Clock;

const SCREEN_WIDTH: i32 = 800;
const SCREEN_HEIGHT: i32 = 600;

pub const Game = struct {
    const Self = @This();
    shouldQuit: bool,
    camera: Camera,
    spriteRenderer: SpriteRenderer,
    textRenderer: TextRenderer,
    dungeon: Dungeon,
    grid: Grid,
    clock: Clock,

    pub fn init(allocator: *Allocator) !Self {
        var spriteRenderer = SpriteRenderer.init(allocator);
        var textRenderer = try TextRenderer.init(allocator);

        spriteRenderer.shader.use();
        var projection = zm.orthographicRh(800.0, 600.0, -10.0, 10.0);
        spriteRenderer.shader.set_mat4("projection", projection);

        var dungeon = try Dungeon.init(allocator, 50);
        var grid = Grid.fromSpriteSize(zm.f32x4(32.0, 32.0, 0.0, 0.0));

        return Self{
            .camera = Camera.init(zm.f32x4(0.0, 0.0, 0.0, 0.0)),
            .spriteRenderer = spriteRenderer,
            .shouldQuit = false,
            .textRenderer = textRenderer,
            .dungeon = dungeon,
            .grid = grid,
            .clock = Clock.init(),
        };
    }

    pub fn deinit(self: *Self) void {
        self.textRenderer.deinit();
        self.spriteRenderer.deinit();
        self.dungeon.deinit();
    }

    pub fn processEvent(self: *Self, e: c.SDL_Event) void {
        self.dungeon.processEvent(e);
    }

    pub fn update(self: *Self) void {
        var spriteSize = self.grid.spriteSize();
        spriteSize[0] /= 2.0;
        spriteSize[1] /= 2.0;
        self.camera.focusOn(self.grid.toScreenPosition(self.dungeon.hero.gridPosition) + spriteSize);
        self.spriteRenderer.shader.use();
        self.spriteRenderer.shader.set_mat4("view", self.camera.viewMatrix());
    }

    pub fn draw(self: *Self) !void {
        self.dungeon.draw(&self.spriteRenderer, &self.grid);

        var buf: [15]u8 = undefined;
        const result = try fmt.bufPrint(&buf, "Frame: {d}ms", .{self.clock.deltaTime()});

        self.textRenderer.draw(result, -SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 20, 0.4, zm.f32x4(1.0, 1.0, 1.0, 0.0));
    }
};
