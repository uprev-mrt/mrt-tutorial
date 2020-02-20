# Interact with poly-packet CLI 
[index](../README.md)


Now that our service is complete, we can interact with it using the poly-packet cli

```bash
poly-packet -i my_protocol.yml -c connect serial:/dev/ttyS3:115200
```
> -c lets you pass a command on start-up, I use it as a convenient way to connect 

Once you are in the CLI, you can send some packets 

```bash
whoAreYou
getData
setName deviceName: Jason Berger
whoAreYou
```

produced the following output:

```bash
______     _      ______          _        _   
| ___ \   | |     | ___ \        | |      | |  
| |_/ /__ | |_   _| |_/ /_ _  ___| | _____| |_ 
|  __/ _ \| | | | |  __/ _` |/ __| |/ / _ \ __|
| | | (_) | | |_| | | | (_| | (__|   <  __/ |_ 
\_|  \___/|_|\__, \_|  \__,_|\___|_|\_\___|\__|    [my_protocol protocol]
              __/ |                            
             |___/                             

 Port Opened : /dev/ttyS3

 --> { "packetType" : "whoAreYou"}
 <-- { "packetType" : "myNameIs", "deviceName" : "Jerry"}

 --> { "packetType" : "getData"}
 <-- { "packetType" : "sensorData", "temp" : 2865, "humidity" : 4939}

 --> { "packetType" : "setName", "deviceName" : "Jason Berger"}
 <-- { "packetType" : "Ack"}

 --> { "packetType" : "whoAreYou"}
 <-- { "packetType" : "myNameIs", "deviceName" : "Jason Berger"}
```


[index](../README.md)


