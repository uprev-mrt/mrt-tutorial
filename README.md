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

## 3) Toggle LED <a id="toggle-led" style="font-size:0.4em;" href="#top">back to top</a>

Now we can use the MrT abstraction layer for stm32. We are going to blink the LED on the board just as a basic example. Add the following code snippets:

**main.c:26** (in the USER CODE INCLUDES section)
```c
#include "Platforms/Common/mrt_platform.h" /* This will include the stm32 layer based on the MRT_PLATFORM symbol we set*/
```
<br>


**main.c:108** ( before the USER CODE WHILE section)
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


## 4) Create a device driver <a id="mrt-device" style="font-size:0.4em;" href="#top">back to top</a>

Obviously an abstraction layer to toggle a gpio is a bit overkill. But the point of this is to write device drivers that can run on any platform. So now we are going to create a device driver for the HTS221 temperature and humidity sensor on the board. 

For this we will use the [mrt-device tool](https://github.com/uprev-mrt/mrtutils/wiki/mrt-device)

This is part of mrtutils, so it is already installed. 

Normally you would create a device driver as submodule, so that it can be re-used as a MrT module, but for the purpose of this tutorial we will just create it in a subdirectory. mrt-device can generate a template to get you started:

```bash
mkdir MrT/Modules/Devices/hts221
cd MrT/Modules/Devices/hts221
mrt-device -t my_device
```

now you should have a new file 'my_device.yml' to fill out. in the 'doc' folder there are 2 files to looks at:
- **device.yml** - this is the yaml file for the device driver that I already created 
- **hts221.pdf** - this is the section of the datasheet that describes the registers. 

Comparing the two files and referencing the [mrt-device wiki](https://github.com/uprev-mrt/mrtutils/wiki/mrt-device) should help you get an idea of how to structure the file. *(A lot of the information at the top is not really needed, but good for documentation)*

Once you feel comfortable with the structure, generate the driver:

```bash
mrt-device -i my_device.yml -o .
```

This will create 3 new files:
- **hts221.h** - header for driver 
- **hts221.c** - source for driver 
- **hts221_regs.h** - various symbols and macros for device registers

>adding the -d flag will generate documentation:
>```bash
>mrt-device -i my_device.yml -o . -d .
>```


Now we have a basic driver with access to all of the register/fields in the device. If the temerature and humidity values could be read directly, wed be done.. But they cant. So we just need to add the logic. 

>This particular device has a pretty convoluted calibration table that has to be read to get conversion constants. You can ignore the logic involved, the take away is that there are code blocks in the driver that will **not** be overwritten if you regenerate the driver. It also shows use of the devices macros for reading fields/registers


**First we are going to add some properties to the device struct:**

hts221.h:85 *between the user-block-struct tags* :
```c

    int mPrevTemp;  
    int mPrevHum;

    struct{
      int16_t T0_out;
      int16_t T1_out;
      int16_t T0_degC;
      int16_t T1_degC;
      uint8_t H0_rH;
      uint8_t H1_rH;
      int16_t H0_T0_OUT;
      int16_t H1_T0_OUT;
    } mCalData;

```

<br>

**Next add functions for reading temperature and humidity :**

hts221.h:100 *between the user-block-bottom tags* :
```c

/**
 * @brief reads humidity from device
 * @param dev ptr to hts221 device
 * @return relative humidity in 1/100th of a percent. i.e. 4520 = %45.2 
 */
int hts_read_humidity(hts221_t* dev);


/**
 * @brief reads temperature from device
 * @param dev ptr to hts221 device
 * @return temperature in 1/100th of a degress C. i.e. 2312 = 23.12 C 
 */
int hts_read_temp(hts221_t* dev);

```

<br>

**Add the code to get calibration constants from calibration table :**

hts221.c:40 *in the user-block-init section* :
```c
 dev->mPrevHum =0;
    dev->mPrevTemp =0;

    /* device requires a bit ORd with register address to auto increment reg addr */
    dev->mRegDev.mAutoIncrement = true;
    dev->mRegDev.mAiMask = 0x80;

    /* Load calibration data */
     uint8_t H0_rh_x2, H1_rh_x2, T0_degC_x8, T1_degC_x8, T1T0_msb;

    /* These registers can be read directly into the cal values */
    dev->mCalData.H0_T0_OUT = hts_read_reg(dev, &dev->mH0T0Out);
    dev->mCalData.H1_T0_OUT = hts_read_reg(dev, &dev->mH1T0Out);
    dev->mCalData.T0_out = hts_read_reg(dev, &dev->mT0Out);
    dev->mCalData.T1_out = hts_read_reg(dev, &dev->mT1Out);

    /* These registers need to be processed to get the values we need */
    H0_rh_x2 = hts_read_reg(dev, &dev->mH0RhX2);
    H1_rh_x2 = hts_read_reg(dev, &dev->mH1RhX2);
    T0_degC_x8 = hts_read_reg(dev, &dev->mT0DegcX8);
    T1_degC_x8 = hts_read_reg(dev, &dev->mT1DegcX8);
    T1T0_msb = hts_read_reg(dev, &dev->mT1t0Msb);

    /* These values just need to be divided down (for some reason they are stored with a multiplier of 2..) */
    dev->mCalData.H0_rH = H0_rh_x2 >> 1;
    dev->mCalData.H1_rH = H1_rh_x2 >> 1;

    /* T0 and T1 are 10 bits, the MSBs are stored together in the T1T0_MSB Register. They have to be put together, and then divided by 8.. (see link to application note) */
    dev->mCalData.T0_degC = ((uint16_t) T0_degC_x8 | (((uint16_t)(T1T0_msb & 0x03)) << 8)) >> 3;
    dev->mCalData.T1_degC = ((uint16_t) T1_degC_x8 | (((uint16_t)(T1T0_msb & 0x0C)) << 6)) >> 3;

```

<br>

**The driver is generated with a 'test' function. we will add the logic to test the devices connection:**

hts221.c:97 *in the user-block-test section* :
```c
    if( hts_read_reg(dev, &dev->mWhoAmI) == HTS_WHO_AM_I_DEFAULT)
    {
        return MRT_STATUS_OK;
    }
```

<br>

**Finally add the code for the temperature and humidity functions:**

hts221.c:108 *in the user-block-bottom section* :
```c
int hts_read_humidity(hts221_t* dev)
{
    int16_t raw_adc;
    float tmp_f;

    //check to make sure data is ready, if not just use previous value
    if(! hts_check_flag(dev,&dev->mStatus, HTS_STATUS_HUM_READY))
    {
        return dev->mPrevHum;
    }

    //get raw adc value
    raw_adc = hts_read_reg(dev, &dev->mHumidityOut);

    //Use calibration coefs to interpolate data to RH%
    tmp_f = ((float)(raw_adc - dev->mCalData.H0_T0_OUT) * (float)(dev->mCalData.H1_rH - dev->mCalData.H0_rH) / (float)(dev->mCalData.H1_T0_OUT - dev->mCalData.H0_T0_OUT))  +  dev->mCalData.H0_rH;

    dev->mPrevHum = tmp_f * 100;
    return dev->mPrevHum;
}

int hts_read_temp(hts221_t* dev)
{   
    int16_t raw_adc;
    float tmp_f;

    //check to make sure data is ready, if not just use previous value
    if(! hts_check_flag(dev,&dev->mStatus, HTS_STATUS_TEMP_READY))
    {
        return dev->mPrevTemp;
    }
    

    //get raw adc value
    raw_adc = hts_read_reg(dev, &dev->mTempOut);

    //Use calibration coefs to interpolate data to deg C
    tmp_f = ((float)(raw_adc - dev->mCalData.T0_out) * (float)(dev->mCalData.T1_degC - dev->mCalData.T0_degC) / (float)(dev->mCalData.T1_out - dev->mCalData.T0_out))  +  dev->mCalData.T0_degC;
    
    
    dev->mPrevTemp = tmp_f * 100;
    return dev->mPrevTemp;
}

```

**Now lets try out our driver:**

main.c:27 *USER CODE includes section*
```c
#include "Devices/hts221/hts221.h;
```

<br>

main.c:95 *USER CODE 2 section*
```c
  uint32_t ticks =0;
  int temperature;
  int humidity;
  hts221_t hts;                               /* create instance of hts221 device*/
  hts_init_i2c(&hts, &hi2c2);                 /* Initialize it on I2C2 bus*/

  if(hts_test(&hts) == MRT_STATUS_OK)         /* Turn on LED if device passes test */
  {
      MRT_GPIO_WRITE(MRT_GPIO(LED_GRN),HIGH );
  }

  /* Set flags/fields for start up and 1hz data*/

  hts_set_flag(&hts, &hts.mCtrl1, HTS_CTRL1_PD);  /* set PD flag of CTRL 1 register, to turn on device*/
  hts_set_ctrl1_odr(&hts, HTS_CTRL1_ODR_1HZ);     /* Set ODR field in CTRL1 Register to 1Hz*/

  /* OR we could use the configuration we created with: HTS_LOAD_CONFIG_AUTO_1HZ(&hts) */

```

main.c:122 *Replace entire while loop*:
```c
/* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    

    /* Every 500 ms see if new data is ready, and read it */
    MRT_EVERY( 50, ticks)   /* convenience macro for systick timing*/
    {
      if(hts_check_flag(&hts, &hts.mStatus, ( HTS_STATUS_TEMP_READY | HTS_STATUS_HUM_READY )  )) /*wait until both flags are set */
      {
        temperature = hts_read_temp(&hts);
        humidity = hts_read_humidity(&hts);
      } 
    }
    ticks++;
    MRT_DELAY_MS(10);

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
```
build the project and run it should toggle the led everytime it 

## Create a PolyPacket Service <a id="poly-make" style="font-size:0.4em;" href="#top">back to top</a>

Now That we have a working device driver, lets create a messaging protocol so we can ask the device for data over the com port. 

first we will need to add in some more MrT modules to support polypacket. back out to your projects root directory and open mrt-config again:

```bash
cd /path/to/project/root
mrt-config MrT
```

Select the following modules to import :
- Utilities/PolyPacket
- Utilities/JSON
- Utilities/COBS 

Once they are imported, create a new directory for your service files

```bash
mkdir MrT/Modules/my_service
cd MrT/Modules/my_service
```
create a copy of the example protocol and rename it: *(yes, there should be a -t for template to match mrt-device.. I plan to change it...)*

```bash
poly-make -e
mv sample_protocol.yml my_protocol.yml
```

now modify the file to match the my_protocol.yml in the doc folder. For a detailed eplanation of the document reference [PolyPacket.wiki/Defining-a-protocol](https://github.com/up-rev/PolyPacket/wiki/Defining-a-Protocol)

Once the descriptor is filled out, generate your service with an application layer:

```bash
poly-make -i my_protocol.yml -a -o .
```
>*Add "-d . " to create an ICD*

This will generate 4 files: 
- **my_protocolService.h** - header for service, you should never need to edit this
- **my_protocolService.c** - source for service, you should never need to edit this
- **app_my_protocol.h** - header for application layer
- **app_my_protocol.c** - source for application layer, this is where you will fill out packet handlers

First include the service:

main.c:28:
```c
#include "my_service/app_my_protocol.h"
```

## Project Start <a id="poly-packet" style="font-size:0.4em;" href="#top">back to top</a>
