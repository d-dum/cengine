import "core" for DisplayManager, MeshLoader, Vec3, Entity, Camera, Shader, ShaderType, ShaderProgram, Light
import "tst.wren" for tst

class GameEngine {
    static launch(){
        var dm = DisplayManager.create(800, 600)

        var mesh = MeshLoader.create("../res/models/stall.obj", true)
        var en = Entity.create(mesh)

        var vc = Vec3.create(1, 2, 3)
        System.print(vc[0])

        en.scale(Vec3.create(0.1, 0.1, 0.1))

        var cam = Camera.create(Vec3.create(0, 0, -20), Vec3.create(0, 0, 0), Vec3.axisY())

        var vert = Shader.create("../res/shaders/vert.glsl", 0)
        var frag = Shader.create("../res/shaders/frag.glsl", ShaderType.FRAGMENT())

        var prog = ShaderProgram.create(vert, frag)

        var light = Light.create(Vec3.create(0, 0, -20), Vec3.create(1, 1, 1))

        while(!dm.isCloseRequested()){
            prog.start()

            prog.stop()

            dm.update()
        }
    }
}