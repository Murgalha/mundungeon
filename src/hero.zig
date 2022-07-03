const std = @import("std");
const zm = @import("zmath");
const Allocator = std.mem.Allocator;
const Grid = @import("grid.zig").Grid;
const Texture = @import("texture.zig").Texture;
const SpriteRenderer = @import("sprite_renderer.zig").SpriteRenderer;
const DungeonTile = @import("dungeon.zig").DungeonTile;

pub const Direction = enum {
    unknown,
    up,
    down,
    left,
    right,
};

pub fn directionToVec(direction: Direction) zm.Vec {
    return switch (direction) {
        .unknown => zm.f32x4(0.0, 0.0, 0.0, 0.0),
        .up => zm.f32x4(0.0, 1.0, 0.0, 0.0),
        .down => zm.f32x4(0.0, -1.0, 0.0, 0.0),
        .left => zm.f32x4(-1.0, 0.0, 0.0, 0.0),
        .right => zm.f32x4(1.0, 0.0, 0.0, 0.0),
    };
}

pub const Hero = struct {
    const Self = @This();
    texture: u32,
    gridPosition: zm.Vec,

    pub fn init(allocator: *const Allocator, gridPos: zm.Vec) !Self {
        return Self{
            .texture = try Texture.new(allocator, "resources/sprites/hero.png"[0..]),
            .gridPosition = gridPos,
        };
    }

    pub fn deinit(_: *const Self) void {
        std.log.warn("Hero deinit is not doing anything", .{});
    }

    pub fn draw(self: *const Self, spriteRenderer: *const SpriteRenderer, grid: *const Grid) void {
        spriteRenderer.draw(self.texture, self.gridPosition, grid.spriteSize());
    }

    pub fn walk(self: *Self, direction: Direction, map: []const []const DungeonTile) void {
        var dest = self.gridPosition + directionToVec(direction);
        var destTile = map[@floatToInt(usize, dest[1])][@floatToInt(usize, dest[0])];
        if (destTile == .floor or destTile == .door or destTile == .corridor) {
            self.gridPosition = dest;
        }
    }
};
