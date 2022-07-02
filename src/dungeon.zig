const std = @import("std");
const zm = @import("zmath");
const c = @import("c.zig");
const SpriteRenderer = @import("sprite_renderer.zig").SpriteRenderer;
const Texture = @import("texture.zig").Texture;
const Grid = @import("grid.zig").Grid;
const Allocator = std.mem.Allocator;
const ArrayList = std.ArrayList;
const AutoHashMap = std.AutoHashMap;
const stdout = std.io.getStdOut().writer();

pub const DungeonTile = enum {
    unknown,
    floor,
    wall,
    empty,
    door,
    corridor,
};

pub const Dungeon = struct {
    const Self = @This();
    allocator: *Allocator,
    map: [][]DungeonTile,
    spriteTable: AutoHashMap(DungeonTile, u32),

    pub fn init(allocator: *Allocator, size: u16) !Self {
        c.random_init(13);
        var m = c.dungeon_generator_new_map(size);
        var map = try createMapSlice(allocator, m, size);

        freeCMap(m, size);

        var hashmap = try createSpriteTable(allocator);

        try printMap(map);

        return Self{
            .allocator = allocator,
            .map = map,
            .spriteTable = hashmap,
        };
    }

    pub fn deinit(self: *Self) void {
        for (self.map) |row| {
            self.allocator.free(row);
        }
        self.allocator.free(self.map);
        self.spriteTable.deinit();
    }

    pub fn draw(self: *Self, renderer: *SpriteRenderer, grid: *Grid) void {
        var i: usize = 0;
        for (self.map) |row| {
            defer i += 1;
            var j: usize = 0;
            for (row) |col| {
                defer j += 1;
                var texture = self.spriteTable.get(col).?;
                var pos = zm.f32x4(@intToFloat(f32, j), @intToFloat(f32, i), 0.0, 0.0);

                renderer.draw(
                    texture,
                    pos,
                    grid.spriteSize(),
                );
            }
        }
    }

    fn createMapSlice(allocator: *Allocator, m: [*c][*c]u8, size: u16) ![][]DungeonTile {
        var list = ArrayList([]DungeonTile).init(allocator.*);
        defer list.deinit();

        var i: usize = 0;
        while (i < size) : (i += 1) {
            var j: usize = 0;
            var row = ArrayList(DungeonTile).init(allocator.*);
            defer row.deinit();

            while (j < size) : (j += 1) {
                try row.append(toDungeonTile(m[i][j]));
            }
            try list.append(row.toOwnedSlice());
        }

        return list.toOwnedSlice();
    }

    fn printMap(map: []const []const DungeonTile) !void {
        for (map) |row| {
            for (row) |col| {
                try stdout.print("{c}", .{toChar(col)});
            }
            try stdout.print("\n", .{});
        }
    }

    fn createSpriteTable(allocator: *Allocator) !AutoHashMap(DungeonTile, u32) {
        var hashmap = AutoHashMap(DungeonTile, u32).init(allocator.*);

        try hashmap.put(.floor, try Texture.new(allocator, "resources/sprites/floor.png"));
        try hashmap.put(.wall, try Texture.new(allocator, "resources/sprites/wall.png"));
        try hashmap.put(.empty, try Texture.new(allocator, "resources/sprites/wall.png"));
        try hashmap.put(.door, try Texture.new(allocator, "resources/sprites/door.png"));
        try hashmap.put(.corridor, try Texture.new(allocator, "resources/sprites/floor.png"));

        return hashmap;
    }

    fn freeCMap(map: [*c][*c]u8, size: u16) void {
        var i: usize = 0;
        while (i < size) : (i += 1) {
            c.free(@ptrCast(*anyopaque, map[i]));
        }
        c.free(@ptrCast(*anyopaque, map));
    }

    fn toChar(tile: DungeonTile) u8 {
        return switch (tile) {
            .unknown => '$',
            .wall => '#',
            .floor => ' ',
            .empty => '.',
            .corridor => 'C',
            .door => 'D',
        };
    }

    fn toDungeonTile(char: u8) DungeonTile {
        switch (char) {
            ' ' => return .floor,
            '#' => return .wall,
            '.' => return .empty,
            'D' => return .door,
            'C' => return .corridor,
            else => return .wall, // TODO: Have an error texture to return here
        }
    }
};
