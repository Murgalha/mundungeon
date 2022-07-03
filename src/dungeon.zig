const std = @import("std");
const zm = @import("zmath");
const c = @import("c.zig");

const Hero = @import("hero.zig").Hero;
const Direction = @import("hero.zig").Direction;
const Enemy = @import("enemy.zig").Enemy;
const Texture = @import("texture.zig").Texture;
const SpriteRenderer = @import("sprite_renderer.zig").SpriteRenderer;
const Grid = @import("grid.zig").Grid;
const Allocator = std.mem.Allocator;
const ArrayList = std.ArrayList;
const AutoHashMap = std.AutoHashMap;
const stdout = std.io.getStdOut().writer();

const PLAYER_POS: zm.Vec = zm.f32x4(25.0, 25.0, 0.0, 0.0);

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
    allocator: *const Allocator,
    hero: Hero,
    enemies: []Enemy,
    spriteHashTable: AutoHashMap(DungeonTile, u32),
    map: [][]DungeonTile,

    pub fn init(allocator: *const Allocator, size: u16) !Self {
        var map = try createEnvironmentMap(allocator, size);
        var hashTable = try createSpriteTable(allocator);

        return Self{
            .allocator = allocator,
            .spriteHashTable = hashTable,
            .map = map,
            .hero = try Hero.init(allocator, PLAYER_POS),
            .enemies = try createEnemies(allocator, map),
        };
    }

    pub fn deinit(self: *Self) void {
        for (self.map) |row| {
            self.allocator.free(row);
        }
        for (self.enemies) |enemy| {
            enemy.deinit();
        }

        self.hero.deinit();
        self.allocator.free(self.enemies);
        self.allocator.free(self.map);
        self.spriteHashTable.deinit();
    }

    pub fn draw(self: *Self, renderer: *SpriteRenderer, grid: *Grid) void {
        var i: usize = 0;
        for (self.map) |row| {
            defer i += 1;
            var j: usize = 0;
            for (row) |col| {
                defer j += 1;
                var texture = self.spriteHashTable.get(col).?;
                var pos = zm.f32x4(@intToFloat(f32, j), @intToFloat(f32, i), 0.0, 0.0);

                renderer.draw(
                    texture,
                    pos,
                    grid.spriteSize(),
                );
            }
        }

        for (self.enemies) |enemy| {
            enemy.draw(renderer, grid);
        }

        self.hero.draw(renderer, grid);
    }

    pub fn processEvent(self: *Self, e: c.SDL_Event) void {
        var hasMoved = false;
        var direction: Direction = .up;
        switch (e.@"type") {
            c.SDL_KEYDOWN => {
                switch (e.key.keysym.sym) {
                    c.SDLK_w => {
                        direction = .up;
                        hasMoved = true;
                    },
                    c.SDLK_a => {
                        direction = .left;
                        hasMoved = true;
                    },
                    c.SDLK_s => {
                        direction = .down;
                        hasMoved = true;
                    },
                    c.SDLK_d => {
                        direction = .right;
                        hasMoved = true;
                    },
                    else => {},
                }
            },
            else => {},
        }

        if (hasMoved) {
            self.hero.walk(direction, self.map);
            var i: usize = 0;
            while (i < self.enemies.len) : (i += 1) {
                self.enemies[i].executeAction(self.map, self.hero.gridPosition);
            }
        }
    }

    fn createSpriteTable(allocator: *const Allocator) !AutoHashMap(DungeonTile, u32) {
        var hashmap = AutoHashMap(DungeonTile, u32).init(allocator.*);

        try hashmap.put(.floor, try Texture.new(allocator, "resources/sprites/floor.png"));
        try hashmap.put(.wall, try Texture.new(allocator, "resources/sprites/wall.png"));
        try hashmap.put(.empty, try Texture.new(allocator, "resources/sprites/wall.png"));
        try hashmap.put(.door, try Texture.new(allocator, "resources/sprites/door.png"));
        try hashmap.put(.corridor, try Texture.new(allocator, "resources/sprites/floor.png"));

        return hashmap;
    }

    fn createEnvironmentMap(allocator: *const Allocator, size: u16) ![][]DungeonTile {
        c.random_init(13);
        var m = c.dungeon_generator_new_map(size);

        defer { // Freeing C map
            var i: usize = 0;
            while (i < size) : (i += 1) {
                c.free(@ptrCast(*anyopaque, m[i]));
            }
            c.free(@ptrCast(*anyopaque, m));
        }

        return try createMapSlice(allocator, m, size);
    }

    fn createMapSlice(allocator: *const Allocator, m: [*c][*c]u8, size: u16) ![][]DungeonTile {
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

    fn createEnemies(allocator: *const Allocator, map: [][]DungeonTile) ![]Enemy {
        var list = ArrayList(Enemy).init(allocator.*);

        try list.append(try Enemy.init(
            allocator,
            map,
            zm.f32x4(24.0, 24.0, 0.0, 0.0),
            zm.f32x4(25.0, 25.0, 0.0, 0.0),
        ));
        try list.append(try Enemy.init(
            allocator,
            map,
            zm.f32x4(27.0, 41.0, 0.0, 0.0),
            zm.f32x4(25.0, 25.0, 0.0, 0.0),
        ));

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
