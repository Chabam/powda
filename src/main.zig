const std = @import("std");
const powda = @import("powda");

pub fn main() !void {
    var world_test = powda.World.init(10, 10);
    defer world_test.deinit();
    world_test.materials[10] = powda.Material.init(powda.Material.Kind.Sand);
    world_test.materials[9] = powda.Material.init(powda.Material.Kind.Sand);
    world_test.materials[19] = powda.Material.init(powda.Material.Kind.Sand);
    for (world_test.materials) |opt_mat| {
        if (opt_mat) |mat| {
            switch (mat.kind) {
                powda.Material.Kind.Sand => {
                    std.debug.print("Powder!", .{});
                },
                else => {},
            }
        }
    }
}
