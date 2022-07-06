# writing-materials-manager

PROJECT NUNOTABA

================

写作素材管理器&emsp;又名：写作助手

Writing Materials Manager a.k.a. Writing Assistant

撰写文学作品的辅助工具。

An auxiliary tool for writing literary works.

开发中

IN DEVELOPMENT

# zh-CN

# 依赖、开发与编译

## Windows

### 第三方库的编译

需要安装的软件：
- Visual Studio 2022，或：至少安装 MSVC（VS C++ Build Tools）14.3（其它支持 C17 和 C++20 的版本亦可）
- CMake
- Qt（版本不低于 6.3）
- MongoDB

打开 PowerShell，将工作目录切换到本项目目录，输入

``` powershell
set-executionpolicy bypass process
./setup
```

下载、编译、安装必要的依赖到项目的 3rd 目录下。

### IDE 支持

#### Microsoft Visual Studio

在菜单栏上，依次点击：Project -> CMake Settings for wmm，打开 CMakeSettings.json。

添加所需的生成类型（build type）后，在 Command arguments 中的 CMake command arguments 输入框，设置 CMAKE_PREFIX_PATH 变量的值为 Qt 二进制文件所在的目录。例如：

```powershell
-DCMAKE_PREFIX_PATH=C:/Qt/6.3.1/msvc2019_64
```

#### JetBrains CLion

在菜单栏上，依次点击：File -> Settings -> Build, Execution, Deployment -> CMake，在 Profiles 下添加所需的生成类型。在 CMake Options 输入框中，设置 CMAKE_PREFIX_PATH 变量的值为 Qt 二进制文件所在的目录。

#### Qt Creator

在 Qt Creator 中，可以直接编译。打开本仓库根目录下的 writing_assistant 项目，在 Projects 选项卡中选定正确的 Kit，编译即可。

如需在 Qt Creator 中查看 qDebug() 等的调试内容，需要启用 “使用 Unicode UTF-8 提供全球语言支持”。

## Linux

# 使用 

## 创建 Python venv

### Windows

可以选择下载安装包并安装，也可以选择下载源码并编译安装（Python 官方提供的解释器是纯 C 写的）。

安装完毕后，在本软件的 py 目录下，按照所需的 NLP 库的要求，创建 venv。例如：

```powershell
python -m venv venv/latest
&"c:\program files\python3.8\python" -m venv venv/3.8
```
注意：venv 创建完毕后，不要移动创建 venv 时使用的 Python 的目录，否则 venv 将无法正常使用。

按照所需的 NLP 库的版本要求，在对应的 venv 下安装这些库即可（为避免下载失败，最好不要使用代理）。例如：

```powershell
venv/3.8/scripts/python -m pip install synonyms jiagu jionlp textrank4zh jieba
venv/latest/scripts/python -m pip install synonyms jiagu textrank4zh jieba
```
一些 NLP 库在 import（或在初次 import）时，由于需要下载模型或将模型读入内存，需等待一段时间（例如数十秒）方可使用。

安装以后，可以导出 venv 下已安装的全部包，供日后出现意外时重新安装用：

```powershell
venv/3.8/scripts/python -m pip freeze > requirements-3.8.txt
venv/latest/scripts/python -m pip freeze > requirements-latest.txt
```

重新安装：

```powershell
venv/3.8/scripts/python -m pip install -r requirements-3.8.txt
venv/latest/scripts/python -m pip install -r requirements-latest.txt
```

### Linux

# en-US

# ja-JP
