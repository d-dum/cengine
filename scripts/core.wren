

foreign class DisplayManager {
    construct create(width, height){}

    foreign isCloseRequested()
    foreign update()
}

foreign class MeshLoader {
    construct create(path, hasUvs){}

    foreign loadTexture(path)
}

foreign class Vec3 {
    construct create(x, y, z){}

    foreign getElement(inx)

    [index] {
        return this.getElement(index)
    }
}


foreign class Entity {
    construct create(mesh){}

    foreign translate(transform)
    foreign rotate(angle, axis)
    foreign scale(scaleVec)
}