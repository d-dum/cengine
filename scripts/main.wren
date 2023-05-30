import "core" for DisplayManager, MeshLoader, Vec3, Entity

class GameEngine {
    static launch(){
        var dm = DisplayManager.create(800, 600)

        var mesh = MeshLoader.create("../res/models/stall.obj", true)
        var en = Entity.create(mesh)

        var vc = Vec3.create(1, 2, 3)
        System.print(vc.getElement(0))

        while(!dm.isCloseRequested()){
            dm.update()
        }
    }
}