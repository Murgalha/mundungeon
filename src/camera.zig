const zm = @import("zmath");

pub const Camera = struct {
    const Self = @This();
    const up: zm.Vec = zm.f32x4(0.0, 1.0, 0.0, 0.0);
    position: zm.Vec,
    front: zm.Vec,

    pub fn init(starting_position: zm.Vec) Self {
        return Self{
            .position = starting_position,
            .front = zm.f32x4(0.0, 0.0, -1.0, 0.0),
        };
    }

    /// Returns the view matrix of the camera
    pub fn viewMatrix(self: *Self) zm.Mat {
        return zm.lookAtRh(self.position, self.position + self.front, Camera.up);
    }

    pub fn focusOn(self: *Self, pos: zm.Vec) void {
        self.position = pos;
    }
};
