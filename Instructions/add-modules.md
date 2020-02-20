# 2) Add MrT Modules
[index](../README.md)


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

> note: right click the project and hit refresh so it sees the new directories

>**To use the STM32 platform, cofigure the following settings:**
>
>**Project->Properties->C/C++ General->Path and Symbols**:
>>Under the Symbols tab add a symbol named **MRT_PLATFORM** with the value **MRT_STM32_HAL**
>Under the Source Location tab click add and select the **Modules** directory under Mr T
> Add The modules directory to the include list for GCC (and G++ if used)
>
>


Build the project, you will see a warning about 'MRT_SPI_TRANSFER', this is normal because no spi is configured 