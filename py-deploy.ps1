param(
    [string] $PythonInterpreterPrefix = "C:\Program Files",
    [string] $venvPrefix = "",
    [hashtable[]] $tasks,
    [string[]] $CommonLibs
)

set usage -option constant -value @"

Usage
    
    Examples:
        .\py-deploy -PythonInterpreterPrefix "C:/Program Files" -venvPrefix "out/build/x64-Release/bin/py/venv" `
        -tasks `
        @{
            srcdir = "Python";
            dstdir = "latest"
        },
        @{
            srcdir = "Python3.8"
            dstdir = "3.8"
            libs = "jionlp"
        } `
        -CommonLibs synonyms, jiagu, textrank4zh, jieba

"@

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

# ./py-sample -PythonInterpreterPrefix "C:/Program Files" -venvPrefix "out/build/x64-Release/bin/py/venv" `
# -tasks `
# @{
#     srcdir = "Python";
#     dstdir = "latest"
# },
# @{
#     srcdir = "Python3.8"
#     dstdir = "3.8"
#     libs = "jionlp"
# } `
# -CommonLibs synonyms, jiagu, textrank4zh, jieba
