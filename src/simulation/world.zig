const std = @import("std");
const ArrayList = std.ArrayList;
const Material = @import("material.zig").Material;

pub const World = struct {
    width: u32,
    height: u32,
    count: u64,
    materials: []?Material,
    const allocator = std.heap.page_allocator;
    pub fn init(width: u32, height: u32) World {
        const count = width * height;
        const materials = allocator.alloc(?Material, count) catch @panic("OOM!");

        for (materials) |*mat| {
            mat.* = null;
        }

        return World{
            .width = width,
            .height = height,
            .count = count,
            .materials = materials,
        };
    }
    pub fn deinit(self: *World) void {
        allocator.free(self.materials);
    }
};
