# Writing Materials Manager (WMM)

写作素材管理器&emsp;又名：写作助手

Writing Materials Manager (WMM) a.k.a. Writing Assistant

撰写文学作品的辅助工具。

An auxiliary tool for writing literary works.

开发中

IN DEVELOPMENT

# 重要内容 (Imperatives)

## 关于隐藏测试数据的声明

本项目为本人的本科毕业设计。过去将测试数据在本仓库中公布，只是为了方便毕业论文评审团队检验已实现功能的正确性。由于后来发现测试数据的公开可能引发版权和个人信息泄露等问题，而毕业论文已经通过，因此暂时不再公开测试数据。

为了将这些数据从全网彻底删除，原项目仓库不再公开；通过将删除私密数据后的剩余文件全部复制到本仓库，来等效实现清除全部涉及的 commit。

## Statement of Hiding Test Data

This project acted as my graduation project of my undergrad stage. The publication of the test data in the past was just for the convenience of the review team of graduation papers to verify the correctness of the implemented functions. By reason of the notice of copyright and privacy leak problems and my success of the undergrad thesis, I will impermanently not publish the test data.

With the purpose of purging these data outright, the original repository is not public any more. The elimination of all involved commits was equivalently achieved by copying the remaining files to this repository after the deletion of the private data.

# zh-CN

# 开发与运行

## Windows

需要安装的软件：
- Visual Studio 2022，或：至少安装 MSVC（VS C++ Build Tools）14.3（其它支持 C17 和 C++20 的版本亦可）
- CMake
- Qt（版本不低于 6.3）
- MongoDB

### IDE 支持

#### Microsoft Visual Studio

在菜单栏上，依次点击：Project -> CMake Settings for wmm，打开 CMakeSettings.json。

添加所需的生成类型（Debug, Release, RelWithDebInfo, MinSizeRel）后，在 Command arguments 中的 CMake command arguments 输入框，设置 CMAKE_PREFIX_PATH 变量的值为 Qt 二进制文件所在的目录。例如：

```powershell
-DCMAKE_PREFIX_PATH=C:/Qt/6.4.0/msvc2019_64
```

#### JetBrains CLion

在菜单栏上，依次点击：File -> Settings -> Build, Execution, Deployment -> CMake，在 Profiles 下添加所需的生成类型（Debug, Release, RelWithDebInfo, MinSizeRel）。

在 CMake Options 输入框中，同样地，设置 CMAKE_PREFIX_PATH 变量的值为 Qt 二进制文件所在的目录。

#### Qt Creator

在 Qt Creator 中，可以直接编译。

打开本仓库根目录下的 writing_assistant 项目，在 Projects 选项卡中选定正确的 Kit，选择所需的生成类型以后，编译即可。

如需在 Qt Creator 中查看 qDebug() 等的调试内容，需要启用 “使用 Unicode UTF-8 提供全球语言支持”。

### 第三方库的编译

提示：若运行 PowerShell 脚本时因权限问题被拒绝：

``` powershell
File ... cannot be loaded. The file ... is not digitally signed. You cannot run this script on the current system. For more information about running scripts and setting execution policy, see about_Execution_Policies at https:/go.microsoft.com/fwlink/?LinkID=135170.
```

则需先在打开的 PowerShell 会话中，输入

``` powershell
set-executionpolicy bypass process
```

取消限制本次会话的执行策略以后，再执行脚本。

打开 PowerShell，将工作目录切换到本项目目录，输入

``` powershell
./setup
```

下载、编译、安装必要的依赖到项目的 3rd 目录下。

注意：必须先将第三方库编译完毕，才能使用 IDE 便捷地进行开发。否则，CMake 无法正确运行，进而导致语法高亮、代码补全等功能不正常。

### 后编译阶段（编译后处理）

编译成功后，运行 PowerShell，进入编译生成的 exe 所在目录，运行

```powershell
# Qt bin 目录视 Qt 安装目录的不同而不同，需要替换成你的计算机上的相应目录。
# 编译生成的文件夹必须事先存在，否则无法进行编译后处理。若生成类型省略不写，则默认为 Release。
./post-build -qtbindir c:/qt/6.4.0/msvc2019_64/bin -bindir out/build/x64-release/bin --buildtype release
```

复制需要的文件到编译出来的 exe 所在的目录下。

此时，编译生成的 exe 即可脱离 Qt Creator 等 IDE 运行：可在 Windows 资源管理器中直接双击打开。

Clean 或 Rebuild 后，无需重新进行编译后处理。如需重新进行，可删除整个 build 目录，重新生成并后处理。

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
