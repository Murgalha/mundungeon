const std = @import("std");
const zm = @import("zmath");

pub const Grid = struct {
    const Self = @This();
    sprite_width: f32,
    sprite_height: f32,

    pub fn fromSpriteSize(size: zm.Vec) Self {
        return Grid{
            .sprite_width = size[0],
            .sprite_height = size[1],
        };
    }

    pub fn toScreenPosition(self: *const Self, position: zm.Vec) zm.Vec {
        return zm.f32x4(position[0] * self.sprite_width, position[1] * self.sprite_height, 0.0, 0.0);
    }

    pub fn spriteSize(self: *const Self) zm.Vec {
        return zm.f32x4(self.sprite_width, self.sprite_height, 0.0, 0.0);
    }
};
