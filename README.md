# MPF SDK

Qt Modular Plugin Framework - Core SDK

## 概述

MPF SDK 提供插件框架的核心接口定义，是所有 MPF 组件的基础依赖。

## 提供的接口

| 接口 | 说明 |
|------|------|
| `IPlugin` | 插件基类接口 |
| `INavigation` | 页面导航服务 |
| `IMenu` | 菜单管理服务 |
| `ISettings` | 配置存储服务 |
| `ITheme` | 主题服务 |
| `IEventBus` | 事件总线服务 |
| `ILogger` | 日志服务 |
| `ServiceRegistry` | 服务注册中心 (抽象类) |

## 安装

### 从源码构建

```bash
# 设置环境变量
export QT_DIR=/path/to/qt6
export MPF_SDK=/path/to/install

# 构建
cmake -B build -G Ninja \
    -DCMAKE_PREFIX_PATH="$QT_DIR" \
    -DCMAKE_INSTALL_PREFIX="$MPF_SDK"
cmake --build build
cmake --install build
```

### 从 Releases 下载

```bash
# 下载预编译包
curl -LO https://github.com/dyzdyz010/mpf-sdk/releases/latest/download/mpf-sdk-linux-x64.tar.gz
tar -xzf mpf-sdk-linux-x64.tar.gz -C ~/mpf-sdk
```

## 使用

```cmake
find_package(MPF REQUIRED)
target_link_libraries(your_target PRIVATE MPF::foundation-sdk)
```

## 许可证

MIT License
