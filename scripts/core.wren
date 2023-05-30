

foreign class DisplayManager {
    construct create(width, height){}

    foreign isCloseRequested()
    foreign update()
}

foreign class MeshLoader {
    construct create(path, hasUvs){}

    foreign loadTexture(path)
}