const std = @import("std");
const zm = @import("zmath");
const Texture = @import("texture.zig").Texture;
const Allocator = std.mem.Allocator;
const ArrayList = std.ArrayList;
const DungeonTile = @import("dungeon.zig").DungeonTile;
const aStar = @import("a_star.zig").aStar;
const SpriteRenderer = @import("sprite_renderer.zig").SpriteRenderer;
const Grid = @import("grid.zig").Grid;

pub const Enemy = struct {
    const Self = @This();
    texture: u32,
    gridPosition: zm.Vec,
    path: ArrayList(zm.Vec),
    pathIndex: usize,

    pub fn init(allocator: *Allocator, map: []const []const DungeonTile, gridPos: zm.Vec, heroPos: zm.Vec) !Self {
        var path = try getPathToHero(allocator, map, gridPos, heroPos);
        std.log.info("Path: {d}", .{path.items});

        return Self{
            .texture = try Texture.new(allocator, "resources/sprites/enemy.png"[0..]),
            .gridPosition = gridPos,
            .path = path,
            .pathIndex = 0,
        };
    }

    pub fn deinit(_: *Self) void {
        //
    }

    pub fn draw(self: *const Self, spriteRenderer: *const SpriteRenderer, grid: *const Grid) void {
        spriteRenderer.draw(self.texture, self.gridPosition, grid.spriteSize());
    }

    pub fn move(self: *Self, heroPos: zm.Vec) void {
        self.gridPosition = self.path.orderedRemove(0);
        self.path.append(heroPos) catch |e| {
            std.log.err("Could not set next enemy move: {s}", .{e});
        };
    }

    fn getPathToHero(allocator: *Allocator, map: []const []const DungeonTile, enemyPos: zm.Vec, heroPos: zm.Vec) !ArrayList(zm.Vec) {
        return try aStar(allocator, map, enemyPos, heroPos);
    }
};
