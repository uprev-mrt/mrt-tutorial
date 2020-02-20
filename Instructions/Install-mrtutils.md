# 1) Installing mrtutils 
[index](../README.md)

MrT modules are just individual git repositories that get included in your project as [submodules](https://git-scm.com/book/en/v2/Git-Tools-Submodules). You could simply add them as submodules manually, but this would require looking up the urls, and making sure the path to each module is correct, because some modules reference others. 

To make this easier, you can use the mrt-config tool from the [mrtutils](https://github.com/uprev-mrt/mrtutils/wiki) package. 

[mrtutils](https://github.com/uprev-mrt/mrtutils/wiki) is a python package managed with pip 

**To install in linux:**
```bash
pip3 install mrtutils
```

**WSL:**
In my experince python setup in windows is very finnicky. If you are developing in windows I **highly** recommend using WSL which is a linux subsytem. 
1) [install wsl](https://docs.microsoft.com/en-us/windows/wsl/install-win10) I use the ubuntu image
2) [install Xming](https://sourceforge.net/projects/xming/) - this allows you to run graphical applications from WSL which will let you use the mrt-config gui
3) run Xming
4) open a wsl terminal and enter the following commands
    - ``` export DISPLAY=:0```
    - ``` sudo apt update```
    - ``` sudo apt install python3 python3-pip python3-pyqt5```
    - ``` pip3 install mrtutils```



Once you have it installed test with:
```bash
mrt-config -h
```
You should see the help output of mrt-config


[Next Step](add-modules.md)
