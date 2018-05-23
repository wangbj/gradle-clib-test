# gradle-clib-test
Using gradle to build C libraries and executables

This is just a project template how to build shared libraries and (main) executables.

  run `gradle build` to build all tasks

and

```shell
LD_LIBRARY_PATH=bulid/libs/preload/shared build/exe/main/main
```

to run the main executable.

or

```shell
LD_PRELOAD=bulid/libs/preload/shared/libpreload.so /path/to/executable
```

to run `/path/to/executable` with LD_PRELOAD.
