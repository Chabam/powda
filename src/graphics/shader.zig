const gl = @import("gl");

const Kind = enum { Fragment, Vertex };
const ShaderError = error{CompilationFailed};

const Shader = struct {
    id: u32,
    kind: Kind,
    fn init(kind: Kind, source: []u8) !Shader {
        const shader_kind = switch (kind) {
            Kind.Fragment => gl.FRAGMENT_SHADER,
            Kind.Vertex => gl.VERTEX_SHADER,
        };

        const id = gl.CreateShader(shader_kind);
        gl.ShaderSource(id, 1, source, null);
        gl.CompileShader(id);

        var is_compiled = undefined;
        gl.GetShaderiv(id, gl.COMPILE_STATUS, &is_compiled);
        if (is_compiled == gl.FALSE) {}
    }
};
