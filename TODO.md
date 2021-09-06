# 2.0 Roadmap

## Common

- 拆分为CustomWindowCore，CustomWindowWidget和CustomWindowQuick三个模块
- 需要对窗口进行操作时，如果是widget，尽量使用widget本身的指针而不是其qwindow的指针
- 尽量不要直接依赖widget或window，能用句柄就用句柄
- 在深色与浅色之间切换时，使用调色板而不是样式表
- 提供一个预置的自绘标题栏
- 要支持5.6~6.x之间的所有版本，注意是否能编译，运行是否正常
- 默认构建动态库
- 尽量不要添加平台相关或编译器相关参数

## Windows

- shell和dwm的函数都动态加载，避免显式链接系统库
- DPI相关函数除动态加载外，还要提供合适的fallback代码
- win平台由于核心代码大量依赖win32原生函数，所以其他方面也尽量用win32，减少win32与qt的混用
- 用win32函数获取dpi和dpr，不要依赖qt的函数
- 获取DPI感知级别？有用吗？待研究
- 调整Alt + Space弹出的系统菜单的位置
- 支持 Win7~11
- 支持 MinGW

## Linux

- 添加原生支持，不要依赖Qt 5.15那个功能
- 检测系统主题，自动切换
- 支持 KDE和GNOME
- 支持 X11 + Wayland

## macOS

- 添加原生支持，不要依赖Qt 5.15那个功能
- 检测系统主题，自动切换
- 支持M1
