

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

    static axisY(){
        return Vec3.create(0, 1, 0)
    }

    static axisX(){
        return Vec3.create(1, 0, 0)
    }

    static axisZ(){
        return Vec3.create(0, 0, 1)
    }
}

class ShaderType {
    static VERTEX(){
        return 0
    }

    static FRAGMENT(){
        return 1
    }
}

foreign class Entity {
    construct create(mesh){}

    foreign translate(transform)
    foreign rotate(angle, axis)
    foreign scale(scaleVec)
}

foreign class Camera {
    construct create(position, lookAt, up){}
}

foreign class Shader {
    construct create(filePath, shaderType){}
}

foreign class ShaderProgram {
    construct create(vertex, fragment){}

    foreign start()
    foreign stop()
}

foreign class Light {
    construct create(position, color){}
}

foreign class Renderer {
    construct create(fov, height, width, near, far){}

    foreign prepare(program, camera)
    
    foreign render(entity, program, light)
}