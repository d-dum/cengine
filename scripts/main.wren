import "core" for DisplayManager, MeshLoader, Vec3, Entity, Camera, Shader, ShaderType, ShaderProgram, Light, Renderer
import "tst.wren" for tst

class GameEngine {
    static launch(){
        var dm = DisplayManager.create(800, 600)

        var mesh = MeshLoader.create("../res/models/stall.obj", true)
        mesh.loadTexture("../res/textures/stallTexture.png")
        var en = Entity.create(mesh)

        var vc = Vec3.create(1, 2, 3)
        System.print(vc[0])

        var cam = Camera.create(Vec3.create(0, 0, -20), Vec3.create(0, 0, 0), Vec3.axisY())

        var vert = Shader.create("../res/shaders/vert.glsl", ShaderType.VERTEX())
        var frag = Shader.create("../res/shaders/frag.glsl", ShaderType.FRAGMENT())

        var prog = ShaderProgram.create(vert, frag)

        var light = Light.create(Vec3.create(0, 0, -20), Vec3.create(1, 1, 1))

        var rnd = Renderer.create(45, 800, 600, 0.1, 100)

        while(!dm.isCloseRequested()){
            rnd.prepare(prog, cam)

            prog.start()
            
            rnd.render(en, prog, light)
            prog.stop()

            en.rotate(0.01, Vec3.axisY())

            dm.update()
        }
    }
}