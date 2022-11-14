param(
    [switch] $help,
    [string] $PythonInterpreterPrefix = "C:\Program Files",
    [string] $venvPrefix = "",
    [string[]] $CommonLibs,
    [hashtable[]] $tasks
)

set usage -option constant -value @"

Usage
    Deploy Python venv.

    Parameters:
        help
            View this help.
        PythonInterpreterPrefix
            (Default: C:\Program Files)
            If Python interpreters that are using to create venvs have a common path prefix, then you can specify it to reduce the char count when typing subsequent arguments.
        venvPrefix
            (Default: "")
            if venv destination paths have a common path prefix, then you can specify it to reduce the char count when typing subsequent arguments.
        CommonLibs
            Libraries which should be installed in every venv
        tasks
            The format of tasks are hashtables which have the following keys:
                srcdir
                    The directory where Python interpreter locates.
                dstdir
                    The venv destination path.
                libs
                    Libraries that should be installed in this venv
    
    Examples:
    [1]
        ./py-deploy -PythonInterpreterPrefix "C:/Program Files" -venvPrefix "out/build/x64-Release/bin/py/venv" ``
        -tasks ``
        @{
            srcdir = "Python";
            dstdir = "latest"
        },
        @{
            srcdir = "Python3.8"
            dstdir = "3.8"
            libs = "jionlp"
        } ``
        -CommonLibs synonyms, jiagu, textrank4zh, jieba
    [2]
        `$BuildTypes = 'Debug', 'Release', 'RelWithDebInfo', 'MinSizeRel'
        foreach (`$BuildType in `$BuildTypes) {
            .\py-deploy -PythonInterpreterPrefix "C:\Program Files" -venvPrefix "out\build\x64-`$BuildType/bin/py/venv" ``
            -CommonLibs synonyms, jiagu, textrank4zh, jieba ``
            -tasks ``
            @{
                srcdir = "Python";
                dstdir = "latest"
            },
            @{
                srcdir = "Python3.8"
                dstdir = "3.8"
                libs = "jionlp"
            }
        }
"@

if (($help -eq $true) -or ($tasks.Length -eq 0)) {
    echo $usage
    exit
}

$cwd = $pwd

foreach ($task in $tasks) {
    if ([string]::IsNullOrEmpty($PythonInterpreterPrefix) -eq $false) {
        $task.srcdir = "$PythonInterpreterPrefix/$($task.srcdir)"
    }
    if ([string]::IsNullOrEmpty($venvPrefix) -eq -$false) {
        $task.dstdir = "$venvPrefix/$($task.dstdir)"
    }
    &"$($task.srcdir)/python" -m venv $task.dstdir                             # create venv
    &"$($task.dstdir)/scripts/python" -m pip install --upgrade pip             # upgrade pip
    &"$($task.dstdir)/scripts/python" -m pip install $CommonLibs $task.libs    # pip install libraries
}

cd $cwd
