# BWL Wayland

`这是一个类Wayland混成器的显示服务器，用于BadOS项目的图形环境`

BadOS项目的所有程序只有在BadWayland完成后，才能够正常测试、运行。

目前已经接入linux drm，具备显示功能，在字符界面运行命令

```bash
./build/bwl-server -c 0
```

可得到全黑屏幕。
