import "core" for DisplayManager, MeshLoader

class GameEngine {
    static launch(){
        var dm = DisplayManager.create(800, 600)

        var mesh = MeshLoader.create("../res/models/stall.obj", true)

        while(!dm.isCloseRequested()){
            dm.update()
        }
    }
}