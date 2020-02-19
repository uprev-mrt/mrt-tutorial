<!DOCTYPE html>
<html>
<head>
  <title>my_protocol ICD</title>
</head>
<body>
<div class="content">
<h1>my_protocol ICD</h1>
<ul>
  <li> Generated with <a href="https://github.com/up-rev/PolyPacket/wiki">PolyPacket</a> on 02/20/20</li>
  <li> CRC: A3571767</li>
  <li> Transport Encoding: (COBS) <a href="https://en.wikipedia.org/wiki/Consistent_Overhead_Byte_Stuffing">Consistent Overhead ByteStuffing</a></li>
</ul>
<hr/>
<h2>Description: </h2>
<p>This is a protocol for talking getting data from the hts221 sensor on the IOT board</p>

<br/>
<hr class="section">
<h2 class="right"> Index </h2>
<hr>


<a href="#packet_ping"> Packets:</a>


<ul>
  <li><a href="#packet_ping">[00]  Ping </a></li>
  <li><a href="#packet_ack">[01]  Ack </a></li>
  <li><a href="#packet_getdata">[03]  getData </a></li>
  <li><a href="#packet_whoareyou">[04]  whoAreYou </a></li>
  <li><a href="#packet_mynameis">[05]  myNameIs </a></li>
  <li><a href="#packet_setname">[06]  setName </a></li>
  <li><a href="#packet_sensordata">[07]  sensorData </a></li>
</ul>

<a href="#packet_device"> Structs:</a>

<ul>
  <li><a href="#packet_device">[02]  Device </a></li>
</ul>




<hr class="section">

<h2 class="right"> Packets</h2>
<hr class="thick">
<div class="packet" id="packet_ping">
<h3>Ping </h3>
<hr/>
<ul>
  <li class="note">Packet ID: <b>[00]</b></li>
  <li class="note"> Requests: <a href="#packet_ack">Ack</a></li>
</ul>

<p class="desc">Ping to request an <a href="#packet_ack">Ack</a>. Used for testing and ICD verification</p>
<br/>
<br/>
<b>Fields:</b>
<table class="fields">
  <tr>
    <th> Field</th>
    <th> Type</th>
    <th> Description</th>
  </tr>
  <tr>
    <td width="">icd</td>
    <td width="">  uint32_t    </td>
    <td>32bit Hash of protocol description. This is used to verify endpoints are using the same protocol</td>
  </tr>
</table>
<br/>
<hr class="thick">
</div>

<div class="packet" id="packet_ack">
<h3>Ack </h3>
<hr>
<ul>
  <li class="note">  Packet ID: <b>[01]</b></li>
  <li class="note">Responds To: <a href="#packet_ping">Ping</a></li>
</ul>

<span class="note"> This Packet type does not contain any data fields </span><br/>
<br/>
<hr class="thick">
</div>


<div id="packet_getdata" class="packet">
<h3>getData </h3>
<hr/>
<ul>
  <li class="note">  Packet ID: <b>[03]</b></li>
  <li class="note">   Requests: <a href="#packet_sensordata">sensorData</a></li>
</ul>

<p class="desc">requests sensor data from device</p>
<br/>
<span class="note"> This Packet type does not contain any data fields </span><br/>
<br/>
<hr class="thick">
</div>
<div id="packet_whoareyou" class="packet">
<h3>whoAreYou </h3>
<hr/>
<ul>
  <li class="note">  Packet ID: <b>[04]</b></li>
  <li class="note">   Requests: <a href="#packet_mynameis">myNameIs</a></li>
</ul>

<p class="desc">Asks the device what it's name is..</p>
<br/>
<span class="note"> This Packet type does not contain any data fields </span><br/>
<br/>
<hr class="thick">
</div>
<div id="packet_mynameis" class="packet">
<h3>myNameIs </h3>
<hr/>
<ul>
  <li class="note">  Packet ID: <b>[05]</b></li>
  <li class="note">Responds To:   <a href="#packet_whoareyou">whoAreYou</a>  </li>
</ul>

<p class="desc">packet to tell someone your name</p>
<br/>
<br/>
<b>Fields:</b>
<table class="fields">
  <tr>
    <th> Field</th>
    <th> Type</th>
    <th> Description</th>
  </tr>
  <tr>
    <td width="">deviceName</td>
    <td width="">  char        [32]    </td>
    <td>Name of device    </td>
  </tr>
</table>

<br/>
<hr class="thick">
</div>
<div id="packet_setname" class="packet">
<h3>setName </h3>
<hr/>
<ul>
  <li class="note">  Packet ID: <b>[06]</b></li>
</ul>

<p class="desc">packet to set the new name of a device</p>
<br/>
<br/>
<b>Fields:</b>
<table class="fields">
  <tr>
    <th> Field</th>
    <th> Type</th>
    <th> Description</th>
  </tr>
  <tr>
    <td width="">deviceName</td>
    <td width="">  char        [32]    </td>
    <td>Name of device    </td>
  </tr>
</table>

<br/>
<hr class="thick">
</div>
<div id="packet_sensordata" class="packet">
<h3>sensorData </h3>
<hr/>
<ul>
  <li class="note">  Packet ID: <b>[07]</b></li>
  <li class="note">Responds To:   <a href="#packet_getdata">getData</a>  </li>
</ul>

<p class="desc">Message containing sensor data</p>
<br/>
<br/>
<b>Fields:</b>
<table class="fields">
  <tr>
    <th> Field</th>
    <th> Type</th>
    <th> Description</th>
  </tr>
  <tr>
    <td width="">temp</td>
    <td width="">  int    </td>
    <td>Temperature in 100ths of a degree. i.e. 4500 = 45.00 deg C    </td>
  </tr>
  <tr>
    <td width="">humidity</td>
    <td width="">  int    </td>
    <td>Humidity in 100ths of a %. i.e. 5650 = 56.5%Rh    </td>
  </tr>
</table>

<br/>
<hr class="thick">
</div>


<hr class="section">
<h2 class="right"> Structs</h2>
<hr class="thick">

<div id="packet_device" class="packet">
<h3>Device </h3>
<hr/>
<ul>
  <li class="note">  Packet ID: <b>[02]</b></li>
</ul>

<p class="desc">struct for holding data on device</p>
<br/>
<br/>
<b>Fields:</b>
<table class="fields">
  <tr>
    <th> Field</th>
    <th> Type</th>
    <th> Description</th>
  </tr>
  <tr>
    <td width="">temp</td>
    <td width="">  int    </td>
    <td>Temperature in 100ths of a degree. i.e. 4500 = 45.00 deg C    </td>
  </tr>
  <tr>
    <td width="">humidity</td>
    <td width="">  int    </td>
    <td>Humidity in 100ths of a %. i.e. 5650 = 56.5%Rh    </td>
  </tr>
  <tr>
    <td width="">deviceName</td>
    <td width="">  char        [32]    </td>
    <td>Name of device    </td>
  </tr>
</table>

<br/>
<hr class="thick">
</div>

</div>
</body>
<style>
table.fixed { table-layout:auto; }
table.fixed td { overflow:visible; }

table.fields{
  table-layout:auto;
}

body {
  padding:0;
}

.content{
  padding-top: 0;
  padding-left: 1%;
  padding-right: 1%;
  background-color: #fff;
}

@media print {
  .packet {
    page-break-inside: avoid;
    padding-top: 4px;
  }
  .content{
    width: 100%;
  }
  body{
    background-color: #fff;
  }
}

@media screen {
  .content{
    width: 50%;
    margin-left: auto;
    margin-right: auto;
    margin-top: 0;
    padding-top: 4px;
    box-shadow: 5px 10px #AAA;
  }

  body{
    background-color: #ccc;
    padding: 0;
  }
}

html *{
  font-size: 1em ;
   color: #000 ;
  font-family: Arial ;
}

hr.section {
  border-style: solid;
  border-width: 2px;
  opacity: 1;
}


hr.thick {
  border-style: solid;
  border-width: 1px;
  border-color: #94b2d3;
  opacity: 1;
}

hr {
  opacity: 0.5;
}

table {
  border-collapse: collapse;
}

td {
  border: 1px solid #000000;
  text-align: left;
  padding: 8px;
  vertical-align: top;
}

.desc{
  font-size: 1.2em;
}

th {
  border: 1px solid #000000;
  text-align: left;
  padding: 8px;
  background-color: #94b2d3;
}

li.val{
  opacity: 0.6;
}

h1{
  font-size: 2.5em;
}

h2 
{
  font-size: 1.8em;
}

h2.right{
  text-align: center;
  font-size: 1.8em;
}

h3
{
  font-size: 1.8em;
  color: #003366;
}

h4 
{
  font-size: 1.1em;
  color: #003366;
}


.note{
  font-style: italic;
  opacity: 0.6;
}

a{
  text-decoration:none;
}

a:link {
  color: blue;
}

/* visited link */
a:visited {
  color: blue;
}

table.fixed tr td:last-child{
    width:1%;
    white-space:nowrap;
}
</style>
</html>