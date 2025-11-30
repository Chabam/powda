const std = @import("std");
const ArrayList = std.ArrayList;

pub const Material = struct {
    pub const Category = enum { Powder, Liquid, Gas, Wall };
    pub const Kind = enum { Sand, Gravel, Water, Oil, Smoke, Metal };
    pub const Direction = enum { Up, Down, Left, Right };
    category: Category,
    kind: Kind,
    density: f32,
    inertia: ?Direction,
    pub fn init(kind: Kind) Material {
        const kind_to_cat: Category = switch (kind) {
            Kind.Sand, Kind.Gravel => Category.Powder,
            Kind.Water, Kind.Oil => Category.Liquid,
            Kind.Smoke => Category.Gas,
            Kind.Metal => Category.Wall,
        };
        const kind_to_density: f32 = switch (kind) {
            Kind.Sand => 1.0,
            Kind.Gravel => 1.5,
            Kind.Water => 0.5,
            Kind.Oil => 0.4,
            Kind.Smoke => 0.3,
            Kind.Metal => 10.0,
        };
        return Material{ .category = kind_to_cat, .kind = kind, .density = kind_to_density, .inertia = null };
    }
};

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
