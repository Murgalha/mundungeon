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
    allocator: *Allocator,

    pub fn init(allocator: *Allocator, map: []const []const DungeonTile, gridPos: zm.Vec, heroPos: zm.Vec) !Self {
        var path = try getPathToHero(allocator, map, gridPos, heroPos);

        return Self{
            .texture = try Texture.new(allocator, "resources/sprites/enemy.png"[0..]),
            .gridPosition = gridPos,
            .path = path,
            .pathIndex = 0,
            .allocator = allocator,
        };
    }

    pub fn deinit(self: *Self) void {
        self.path.deinit();
    }

    pub fn draw(self: *const Self, spriteRenderer: *const SpriteRenderer, grid: *const Grid) void {
        spriteRenderer.draw(self.texture, self.gridPosition, grid.spriteSize());
    }

    pub fn executeAction(self: *Self, map: []const []const DungeonTile, heroPos: zm.Vec) void {
        self.path.deinit();
        self.path = getPathToHero(self.allocator, map, self.gridPosition, heroPos) catch unreachable;

        if (self.path.items.len == 0) {
            std.log.info("Enemy should attack", .{});
            // TODO: Attack
        } else {
            self.move();
        }
    }

    fn move(self: *Self) void {
        self.gridPosition = self.path.orderedRemove(0);
    }

    fn getPathToHero(allocator: *Allocator, map: []const []const DungeonTile, enemyPos: zm.Vec, heroPos: zm.Vec) !ArrayList(zm.Vec) {
        return try aStar(allocator, map, enemyPos, heroPos);
    }

    fn inRange(enemyPos: zm.Vec, heroPos: zm.Vec) bool {
        var tmp = enemyPos - heroPos;
        tmp[0] = @fabs(tmp[0]);
        tmp[1] = @fabs(tmp[1]);
        return tmp[0] == 1.0 and tmp[1] == 0.0 or
            tmp[0] == 0.0 and tmp[1] == 1.0;
    }
};
