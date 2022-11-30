# Libraries

This directory is intended for project specific (private) libraries.
PlatformIO will compile them to static libraries and link into executable file.

The source code of each library should be placed in a an own separate directory
(`lib/your_library_name/[here are source files]`).

For example, see a structure of the following two libraries `Foo` and `Bar`:

```
|--lib
|  |
|  |--Bar
|  |  |--docs
|  |  |--examples
|  |  |--src
|  |     |- Bar.c
|  |     |- Bar.h
|  |  |- library.json (optional, custom build options, etc)
|  |
|  |--Foo
|  |  |- Foo.c
|  |  |- Foo.h
|  |
|  |- README --> THIS FILE
|
|- platformio.ini
|--src
   |- main.c
```

and a contents of `src/main.c`:

```c
#include <Foo.h>
#include <Bar.h>

int main (void)
{
  ...
}
```

PlatformIO Library Dependency Finder will find automatically dependent
libraries scanning project source files.

More information about the `library.json` file can be found [here](https://docs.platformio.org/page/librarymanager/config.html).

More information about PlatformIO Library Dependency Finder can be found [here](https://docs.platformio.org/page/librarymanager/ldf.html).
