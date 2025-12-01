const glfw3 = @cImport(@cInclude("GLFW/glfw3.h"));

pub const Window = struct {
    width: u32,
    height: u32,
    windowPtr: *glfw3.GLFWwindow,

    pub fn init(width: u32, height: u32) Window {
        const ptr = glfw3.glfwCreateWindow(@intCast(width), @intCast(height), "Powda", null, null);

        var new_window = Window{
            .width = width,
            .height = height,
            .windowPtr = ptr.?,
        };

        glfw3.glfwSetWindowAspectRatio(ptr, 16, 9);
        glfw3.glfwSetWindowUserPointer(ptr, @as(?*anyopaque, &new_window));

        return new_window;
    }

    pub fn activate(self: *Window) void {
        glfw3.glfwMakeContextCurrent(self.windowPtr);
        glfw3.glfwSwapInterval(0);
    }

    pub fn update(self: *Window) void {
        glfw3.glfwSwapBuffers(self.windowPtr);
        glfw3.glfwPollEvents();
    }

    pub fn should_close(self: *Window) bool {
        return glfw3.glfwWindowShouldClose(self.windowPtr);
    }

    pub fn close(self: *Window) bool {
        glfw3.glfwSetWindowShouldClose(self.windowPtr, glfw3.GLFW_TRUE);
    }
};
