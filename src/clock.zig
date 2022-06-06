const c = @import("c.zig");

pub const Clock = struct {
    const Self = @This();
    current_frame: u64,
    last_frame: u64,

    pub fn init() Self {
        return Self{
            .current_frame = 0,
            .last_frame = 0,
        };
    }

    pub fn deltaTime(self: *Self) u64 {
        self.current_frame = c.SDL_GetTicks64();
        var dt = self.current_frame - self.last_frame;
        self.last_frame = self.current_frame;

        return dt;
    }
};
