const std = @import("std");
const zmath = @import("deps/zmath/build.zig");
const FileSource = std.build.FileSource;

pub fn build(b: *std.build.Builder) void {
    // Standard target options allows the person running `zig build` to choose
    // what target to build for. Here we do not override the defaults, which
    // means any target is allowed, and the default is native. Other options
    // for restricting supported target set are available.
    const target = b.standardTargetOptions(.{});

    // Standard release options allow the person running `zig build` to select
    // between Debug, ReleaseSafe, ReleaseFast, and ReleaseSmall.
    //const mode = b.standardReleaseOptions();
    const mode = std.builtin.Mode.Debug;

    const exe = b.addExecutable("mundungeon", "src/main.zig");

    exe.addPackage(zmath.pkg);

    exe.setTarget(target);
    exe.addIncludeDir("include/");
    exe.linkLibC();
    exe.linkSystemLibrary("SDL2");
    exe.linkSystemLibrary("GL");
    exe.linkSystemLibrary("freetype2");
    exe.addCSourceFile("src/c/glad.c", &[_][]const u8{"-std=c99"});
    exe.addCSourceFile("src/c/stb_image.c", &[_][]const u8{"-std=c99"});
    exe.addCSourceFile("src/c/dungeon_generator.c", &[_][]const u8{"-std=c99"});
    exe.setBuildMode(mode);

    exe.install();

    const run_cmd = exe.run();
    run_cmd.step.dependOn(b.getInstallStep());
    if (b.args) |args| {
        run_cmd.addArgs(args);
    }

    const run_step = b.step("run", "Run the app");
    run_step.dependOn(&run_cmd.step);

    const exe_tests = b.addTest("src/main.zig");
    exe_tests.setTarget(target);
    exe_tests.setBuildMode(mode);

    const test_step = b.step("test", "Run unit tests");
    test_step.dependOn(&exe_tests.step);
}
