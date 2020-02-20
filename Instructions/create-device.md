# Create a device driver 
[index](../README.md)


Obviously an abstraction layer to toggle a gpio is a bit overkill. But the point of this is to write device drivers that can run on any platform. So now we are going to create a device driver for the HTS221 temperature and humidity sensor on the board. 

For this we will use the [mrt-device tool](https://github.com/uprev-mrt/mrtutils/wiki/mrt-device)

This is part of mrtutils, so it is already installed. 

Normally you would create a device driver as a submodule, so that it can be re-used as a MrT module, but for the purpose of this tutorial we will just create it in a subdirectory. mrt-device can generate a template to get you started:

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


Now we have a basic driver with access to all of the register/fields in the device. If the temperature and humidity values could be read directly, wed be done.. But they cant. So we just need to add the logic. 

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
build the project and run it. The led should turn on to show it passed the device test. If you step through code you will see valid temperature/humidity readings. 

[Next Step](create-proto.md)