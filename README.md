# BWL Wayland

`这是一个类Wayland混成器的显示服务器，用于BadOS项目的图形环境`

BadOS项目的所有程序只有在BadWayland完成后，才能够正常测试、运行。

目前已经接入linux drm，具备显示功能。

## routine

1. 用户的bwl.hh即将加入Page类和Frame类以方便用户程序使用。

2. 屏幕刷新函数即将加入所有当前对象的刷新。

## 代码阅读指引

```bash
1. includes/bsv.hh
2. libs/bsv.cc
3. BWLserver/server.cc
4. BWLserver/server.hh
5. BWLserver/display.hh
6. BWLserver/display.cc
```
