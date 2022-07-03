const std = @import("std");
const zm = @import("zmath");
const math = std.math;
const Order = math.Order;
const FLOAT_MAX = std.math.f32_max;
const Allocator = std.mem.Allocator;
const DungeonTile = @import("dungeon.zig").DungeonTile;
const ArrayList = std.ArrayList;

const Node = struct {
    position: zm.Vec,
    parentPosition: zm.Vec,
    gCost: f32,
    hCost: f32,
    fCost: f32,
};

pub fn aStar(allocator: *const Allocator, map: []const []const DungeonTile, start: zm.Vec, goal: zm.Vec) !ArrayList(zm.Vec) {
    if (vecEquals(start, goal)) {
        return ArrayList(zm.Vec).init(allocator.*);
    }

    var nodeMap = try initNodeMap(allocator, map.len);
    defer {
        for (nodeMap) |row| {
            allocator.free(row);
        }
        allocator.free(nodeMap);
    }
    var closedList = try initClosedList(allocator, map.len);
    defer {
        for (closedList) |row| {
            allocator.free(row);
        }
        allocator.free(closedList);
    }

    var startNode = nodeMap[@floatToInt(usize, start[1])][@floatToInt(usize, start[0])];
    startNode.position = start;
    startNode.fCost = 0.0;
    startNode.gCost = 0.0;
    startNode.hCost = 0.0;

    var openList = ArrayList(Node).init(allocator.*);
    defer openList.deinit();

    try openList.append(startNode);

    while (openList.items.len != 0) {
        // Pop element with min cost
        var idx: usize = 0;
        var minIdx: usize = 0;
        var minCost: f32 = 0;
        while (idx < openList.items.len) : (idx += 1) {
            var tmpNode = openList.items[idx];
            if (tmpNode.fCost < minCost) {
                minCost = tmpNode.fCost;
                minIdx = idx;
            }
        }
        var currentNode = openList.orderedRemove(minIdx);
        var currentX = @floatToInt(usize, currentNode.position[0]);
        var currentY = @floatToInt(usize, currentNode.position[1]);

        closedList[currentY][currentX] = true;

        var neighbors = getNeighbors(nodeMap, currentNode);
        for (neighbors) |neighbor| {
            var neighborX = @floatToInt(usize, neighbor.position[0]);
            var neighborY = @floatToInt(usize, neighbor.position[1]);
            if (isWalkable(map, neighbor)) {
                if (vecEquals(neighbor.position, goal)) {
                    var nodeToUpdate = &(nodeMap[neighborY][neighborX]);
                    nodeToUpdate.parentPosition = currentNode.position;

                    return makePath(allocator, nodeMap, goal);
                } else if (closedList[neighborY][neighborX] == false) {
                    var gNew = currentNode.gCost + 1.0;
                    var hNew = heuristic(neighbor.position, goal);
                    var fNew = gNew + hNew;
                    var nodeToUpdate = &(nodeMap[neighborY][neighborX]);
                    if (nodeToUpdate.fCost == FLOAT_MAX or nodeMap[neighborY][neighborX].fCost > fNew) {
                        nodeToUpdate.fCost = fNew;
                        nodeToUpdate.gCost = gNew;
                        nodeToUpdate.hCost = hNew;
                        nodeToUpdate.parentPosition = currentNode.position;
                        try openList.append(nodeToUpdate.*);
                    }
                }
            }
        }
    }

    return ArrayList(zm.Vec).init(allocator.*);
}

fn initClosedList(allocator: *const Allocator, size: usize) ![][]bool {
    var x: usize = 0;
    var y: usize = 0;
    var closedList = ArrayList([]bool).init(allocator.*);
    defer closedList.deinit();

    while (y < size) : (y += 1) {
        var row = ArrayList(bool).init(allocator.*);
        defer row.deinit();
        x = 0;
        while (x < size) : (x += 1) {
            try row.append(false);
        }
        try closedList.append(row.toOwnedSlice());
    }

    return closedList.toOwnedSlice();
}

fn initNodeMap(allocator: *const Allocator, size: usize) ![][]Node {
    var x: usize = 0;
    var y: usize = 0;
    var nodeMap = ArrayList([]Node).init(allocator.*);
    defer nodeMap.deinit();

    while (y < size) : (y += 1) {
        var row = ArrayList(Node).init(allocator.*);
        defer row.deinit();
        x = 0;
        while (x < size) : (x += 1) {
            var node = Node{
                .fCost = FLOAT_MAX,
                .gCost = FLOAT_MAX,
                .hCost = FLOAT_MAX,
                .parentPosition = zm.f32x4(-1.0, -1.0, 0.0, 0.0),
                .position = zm.f32x4(@intToFloat(f32, x), @intToFloat(f32, y), 0.0, 0.0),
            };
            try row.append(node);
        }
        try nodeMap.append(row.toOwnedSlice());
    }

    return nodeMap.toOwnedSlice();
}

fn getNeighbors(map: [][]Node, node: Node) [4]Node {
    var vec = node.position;
    var x = @floatToInt(usize, vec[0]);
    var y = @floatToInt(usize, vec[1]);

    return [_]Node{
        map[y + 1][x],
        map[y - 1][x],
        map[y][x + 1],
        map[y][x - 1],
    };
}

fn isWalkable(map: []const []const DungeonTile, node: Node) bool {
    var vec = node.position;
    var tile = map[@floatToInt(usize, vec[1])][@floatToInt(usize, vec[0])];
    return tile != .empty and tile != .wall;
}

fn vecEquals(vec1: zm.Vec, vec2: zm.Vec) bool {
    return vec1[0] == vec2[0] and
        vec1[1] == vec2[1];
}

fn heuristic(v1: zm.Vec, v2: zm.Vec) f32 {
    return math.fabs(v1[0] - v2[0]) + math.fabs(v1[1] - v2[1]);
}

fn makePath(allocator: *const Allocator, map: [][]Node, goal: zm.Vec) !ArrayList(zm.Vec) {
    var path = ArrayList(zm.Vec).init(allocator.*);
    defer path.deinit();

    var x = @floatToInt(usize, goal[0]);
    var y = @floatToInt(usize, goal[1]);

    var node = map[y][x];
    while (node.parentPosition[0] != -1.0 and node.parentPosition[1] != -1.0) {
        node = map[y][x];
        var parent = node.parentPosition;

        if (parent[0] == -1.0 and parent[1] == -1.0) {
            break;
        }
        try path.append(node.position);

        var parentX = @floatToInt(usize, parent[0]);
        var parentY = @floatToInt(usize, parent[1]);
        x = parentX;
        y = parentY;
    }

    var reversePath = ArrayList(zm.Vec).init(allocator.*);

    var i: i32 = @intCast(i32, path.items.len) - 1;
    while (i > 0) : (i -= 1) {
        try reversePath.append(path.items[@intCast(usize, i)]);
    }

    return reversePath;
}
