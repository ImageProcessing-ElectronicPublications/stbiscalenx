# stbiscalenx

Stb Image Scale Near X. This algorithm is based on [the scale2x pixel graphics algorithm](https://www.scale2x.it/). In this algorithm, “equals” is replaced by “near”, which allows it to work with full-color graphics. The algorithm remained simple, like Scale2x. This implementation can enlarge the image by 2 and 3 times.

| origin | x2 | x3 |
| --- | --- | --- |
| ![orig](images/test.png) | ![orig](images/test.x2.png) | ![orig](images/test.x3.png) |

## build

### load submodules

submodules:
- [stb](https://github.com/nothings/stb.git) -> [src/stb](src)

```shell
$ git submodule init
$ git submodule update
```

### install dependencies

build dependencies:

- build-essential
- cmake

```shell
$ sudo apt-get install build-essential cmake
```

### compilation
```shell
$ mkdir build
$ cd build
$ cmake ..
$ make
```
## use

The first and second parameters specify the paths to the image and the result {PNG}. The `-k` option sets the scale factor, valid values ​​are 2 and 3.
```shell
./stbiscalenx -k 2 ${IMAGE_PATH} ${IMAGE_PATH}.out.png
```

## structure

- `dependencies.c` - API [stb](https://github.com/nothings/stb.git)
- `scalenx.h` - scalen2x and scalen3x.
- `stb/` - [stb](https://github.com/nothings/stb.git)
- `stbiscalenx.c` - CLI program.

---

