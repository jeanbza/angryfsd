# angryfsd

A belligerent, british file system daemon.

![](./angry.gif)

## Building

Once:

```
meson setup builddir
```

Every time:

```
cd builddir
meson --reconfigure
ninja
```

## Running

Normally:

```
angryfsd .
```

Debugging:

```
angryfsd . -f -d -s
```