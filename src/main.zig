const std = @import("std");
const sim = @import("simulation");
const gfx = @import("graphics");
const glfw3 = @cImport(@cInclude("GLFW/glfw3.h"));
const gl = @import("gl");

var procs: gl.ProcTable = undefined;

pub fn main() !void {
    if (glfw3.glfwInit() == glfw3.GLFW_FALSE)
        @panic("Couldn't initialize glfw");

    defer glfw3.glfwTerminate();

    glfw3.glfwWindowHint(glfw3.GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfw3.glfwWindowHint(glfw3.GLFW_CONTEXT_VERSION_MINOR, 6);
    glfw3.glfwWindowHint(glfw3.GLFW_OPENGL_PROFILE, glfw3.GLFW_OPENGL_CORE_PROFILE);

    var win = gfx.Window.init(1280, 720);
    win.activate();
    if (!procs.init(glfw3.glfwGetProcAddress)) return error.InitFailed;

    gl.makeProcTableCurrent(&procs);
    defer gl.makeProcTableCurrent(null);

    win.update();
}
