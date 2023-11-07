# kube

![https://giphy.com/gifs/6sgx3EvsKJJYwJsvnN](https://media.giphy.com/media/v1.Y2lkPTc5MGI3NjExcjJ1eWs1MzhqcGNtajNpeWQzeG42YTlsMmw3eHozNDZucDdlMHl6bCZlcD12MV9pbnRlcm5hbF9naWZfYnlfaWQmY3Q9Zw/6sgx3EvsKJJYwJsvnN/giphy.gif)

This is a project I started to learn OpenGL and maybe one day make a game engine
that I could use for future C++ games. At the moment all it does is render a few
free model assets with different shaders. Free free to use it as an example for
building your own project. In each demo, you can use the arrow keys and shift to
rotate the models along each of the x, y, and z axes. `make demo-creatures` is
probably the most interesting, and you can press space to rotate through the
different creature models. Sorry the rotation is so fast

### Build dependencies

* glew-devel v2.1.0 release 7
* glfw-devel v3.3.2 release 8
* libglu-devel v9.0.1 release 8
* libassimp (third_party/assimp)

## Development

To build the project and all demos run:

```
make pull-submodules
make
```

This will create a bunch of demos in /bin.

If clang cannot find shared libraries, run this command and try again:

```
eval $(cat ./scripts/export_library_path.sh)
```

Run the cube demo using:

```
make demo-cube
```

You can run other demos using a similar command:

```
make demo-<name>
```

See all demos in /demos.


