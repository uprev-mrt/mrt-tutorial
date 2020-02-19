# MrT Tutorial <a id="top"></a>

This is a guide for incorporating MrT modules into a project. This guide will be broken up into stages. There is a 'reference' branch where each stage has a tag to provide an example. The stages are:

0) [Project start](#start)
1) [Installing mrtutils ](#mrtutils)
2) [Add MrT Modules using the mrt-config tool ](#mrt-config)
3) [Toggle LED using MrT Abstraction Layer ](#toggle-led)
4) [Create Device Driver using mrt-device tool](#mrt-device)
5) [Create Poly Packet service](#poly-make)
6) [Talk to Service using poly-packet CLI](#poly-packet)


## Project Start <a id="start" style="font-size:0.4em;" href="#top">back to top</a>
At head of the mast branch is the project start. An STM32 project has already been created to target the STM32L4 ( Their IOT node dev board)

- Uart1: 115200 baud
- I2C2 
- GPIO PB14 as output, labeled as LED_GRN

The setup for this project is not in the scope of this tutorial, but usine STM32CUBE is pretty well documented online 

## 1) Installing mrtutils <a id="mrt-config" style="font-size:0.4em;" href="#top">back to top</a>

MrT modules are just individual git repositories that get included in your project as [submodules](https://git-scm.com/book/en/v2/Git-Tools-Submodules). You could simply add them as submodules manually, but this would require looking up the urls, and making sure the path to each module is correct, because some modules reference others. 

To make this easier, you can use the mrt-config tool from the [mrtutils](https://github.com/uprev-mrt/mrtutils/wiki) package. 

[mrtutils](https://github.com/uprev-mrt/mrtutils/wiki) is a python package managed with pip 

**To install in linux:**
```bash
pip3 install mrtutils
```

**WSL:**
In my experince python setup in windows is very finnicky. If you are developing in windows I **highly** recommend using WSL which is a linux subsytem. 
1) [install wsl](https://docs.microsoft.com/en-us/windows/wsl/install-win10) - I use ubuntu
2) [install Xming](https://sourceforge.net/projects/xming/) - this allows you to run graphical applications from WSL which will let you use the mrt-config gui
3) run Xming
4) open a wsl terminal and enter the following commands
    - ``` export DISPLAY=:0```
    - ``` sudo apt update```
    - ``` sudo apt install python3 python3-pip```
    - ``` pip3 install mrtutils```



Once you have it installed test with:
```bash
mrt-config -h
```
You should see the help output of mrt-config


## 2) Add MrT Modules <a id="mrt-config" style="font-size:0.4em;" href="#top">back to top</a>

Once you have installed mrtutils, adding modules is very simple. just run mrt-config and tell it where you want to put the modules *(It will create the directory)*

```bash
cd /path/to/mrt-tutorial
mrt-config MrT
```
This will open the mrt-config gui:

<img src="doc/mrtconfig.png">

The tool will open up a treeview of the available submodules. Select the modules you would like to import. clicking on a module will show its README file in the view panel. This should show if other modules are required *(I am working on automating dependencies)*

For Now select the following modules:
- **Platforms/STM32** - this is the absctraction layer for STM32 MCUs. it provides definitions/Macros to map hardware interaction with the STM32 HAL
- **Platforms/Common** - this module is required when using any platform abstraction layer
- **Devices/RegDevice** - This is the base module for generic register based devices. It is needed later in [Creating Device Driver using mrt-device tool](#mrt-device)

Once you have selected the required modules, click 'Import Submodules'.

You should now have a folder called 'MrT' in your projects directory with the submodules inside of it. 

Now you need to configure the project to use these submodules. Each platform module should have instructions in its [README](MrT/Platforms/STM32/README.md) document

Here are the instructions from STM32/README.md:



>**To use the STM32 platform, cofigure the following settings:**
>
>**Project->Properties->C/C++ General->Path and Symbols**:
>>Under the Symbols tab add a symbol named **MRT_PLATFORM** with the value **MRT_STM32_HAL**<br/>
>Under the Source Location tab click add and select the **Modules** directory under Mr T
>
>**Project->Properties->C/C++ Build->Settings->Includes**:
>
>> Add The modules directory to the include list for GCC (and G++ if used)
>
>**NOTE**: Sometimes reconfiguring the project with STM32CubeMx will overwrite these changes.
>
>>To test that it is configured Correctly, build the project and look at the output. There should be a message that says "MrT Platform: STM32"

If you see the message when you build, you should be all set to use MrT in this project!

## 3) Toggle LED <a id="toggle-led" style="font-size:0.4em;" href="#top">back to top</a>

Now we can use the MrT abstraction layer for stm32. We are going to blink the LED on the board just as a basic example. Add the following code snippets:

**main.c:26** (in the USER CODE INCLUDES section)
```c
#include "Platforms/mrt_platform" /* This includes will include the stm32 layer based on the MRT_PLATFORM symbol we set*/
```
<br>


**main.c:107** ( before the USER CODE WHILE section)
```c
/** STM32 HAL does not have a type for pins, all of its functions use (port,pin). MRT_GPIO() is a macro that wraps them 
  * This is so that device drives have a single struct for pins 
  */
MRT_GPIO_WRITE(MRT_GPIO(LED_GRN),HIGH);     //set the pin high
MRT_DELAY_MS(1000);                         //wait 1000 ms
MRT_GPIO_WRITE(MRT_GPIO(LED_GRN),LOW);      //set the pin low
MRT_DELAY_MS(1000);                         //wait 1000 ms

```

Now build and run the project, the green LED on the board should blink!


## Create a device driver <a id="mrt-device" style="font-size:0.4em;" href="#top">back to top</a>

While an abstraction layer to blink an LED is not very practical, the point of the abtraction layer is to interact with hardware. So we will now


## Project Start <a id="poly-make" style="font-size:0.4em;" href="#top">back to top</a>
## Project Start <a id="poly-packet" style="font-size:0.4em;" href="#top">back to top</a>
