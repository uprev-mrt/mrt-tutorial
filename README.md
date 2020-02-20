# MrT Tutorial <a id="top"></a>

This is a guide for incorporating MrT modules into a project. The guides walks through the full implementation of a project using MrT, a generated device driver, and a custom messaging protocol.  This guide will be broken up into stages. There is a 'reference' branch with Tags showing the end of each stage:

0) [Project start](#start)
1) [Installing mrtutils ](Instructions/install-mrtutils.md)
2) [Add MrT Modules using the mrt-config tool ](Instructions/add-modules.md)
3) [Toggle LED using MrT Abstraction Layer ](Instructions/toggle-led.md)
4) [Create Device Driver using mrt-device tool](Instructions/create-device.md)
5) [Create Poly Packet service](Instructions/create-proto.md)
6) [Customize Our Service](Instructions/custom-proto.md)
7) [Interact with poly-packet CLI](Instructions/poly-cli.md)



# 0) Project Start <a id="start"></a>
At head of the master branch is the project start. An STM32 project has already been created to target the STM32L4 ( Their IOT node dev board)

- Uart1: 115200 baud
- I2C2 
- GPIO PB14 as output, labeled as LED_GRN

The setup for this project is not in the scope of this tutorial, but usine STM32CUBE is pretty well documented online 


