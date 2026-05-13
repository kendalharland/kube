import "kube" for Window, Entity, Engine

var title = "Rotating Kube"
var width = 1000
var height = 800

Window.open(width, height, title)

var cube = Entity.new()
cube.setModel("cube")

Engine.run()
