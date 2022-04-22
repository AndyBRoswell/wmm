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

# 编译

## Windows

安装：
- Visual Studio 2022，或至少安装 MSVC（VS C++ Build Tools）14.3
- CMake
- Qt（版本不低于 6.3）
- MongoDB

打开 PowerShell，将工作目录切换到本项目目录，输入

``` powershell
set-executionpolicy bypass process
./setup
```

下载、编译、安装必要的依赖到项目的 3rd 目录下。

在 Qt Creator 中，可以直接编译。打开本仓库根目录下的 writing_assistant 项目即可。

CLion 的代码补全和语法高亮效果更佳。但经测试，在 CLion 中编译后，运行依然会报错，提示缺少某些库。因此，若使用 CLion 开发，仍需要在 Qt Creator 中编译。

编译器需要支持 C17 和 C++20。

把 Qt Creator 的 Projects 页面的全部参数粘贴到 CLion 的 Settings | Build, Execution, Deployment | CMake | Profiles | Debug 里（注意：Initial CMake Parameters 中含有 Qt Creator 内部的变量，点击该输入框右上角的图标，可以查询到各个变量的值，用这些值替换掉内部变量）。于是，可以在 Debug 配置下正常跑完 CMake，代码补全和语法高亮正常。

如需在 Qt Creator 中看到 qDebug() 等的调试内容，需要启用 “使用 Unicode UTF-8 提供全球语言支持”。

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

### Linux

# en-US

# ja-JP
