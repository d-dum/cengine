import "core" for DisplayManager


class GameEngine {
    static launch(){
        var dm = DisplayManager.create(800, 600)

        while(!dm.isCloseRequested()){
            dm.update()
        }
    }
}