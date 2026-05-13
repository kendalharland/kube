// -- File: kube.wren

class Engine {
    foreign static run()
}

class Window {
    foreign static open(width, height, title)
}

foreign class Entity {
    construct new() {}
    foreign setModel(value)
}

class Key {
    left {}
    right {}
    up {}
    down {}
}

// -- File: main.wren

var title = "Rotating Kube"
var width = 1000
var height = 800

Window.open(width, height, title)

var cube = Entity.new()
cube.setModel("cube")

Engine.run()
