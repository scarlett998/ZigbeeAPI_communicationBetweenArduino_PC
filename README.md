# wirelesscommunication Using Zigbee API 


!!Have to include the javax. first 
!!Have to configure Zigbee 


Materials used
•Xbee radio modules with adapter boards •Bluetooth/Zigbee bridge•Arduino boards with ATMEGA328P or similar processor •Atmel Studio 6 IDE environment for AVR processors•AVRISP mkII Programmer (which is the tool to burn the code in your hardware)•LEDs (to simulate lighting control) and resistors,  •Transducers (temperature sensor), •Signal wires and USB cables.•Prototype board for connecting components •PC and/or Android smart phone with Bluetooth interface B.BackgroundA short presentation shall be given about WSN.  In addition, read the documentation provided in the previous lab. 


Procedure:
1.Set up a Zigbee network in two floors of the building H212.Use the Zigbee nodes for  -room temperature measurement-room lighting control (on/off switch)3.Send information to and from the Zigbee network to a remote server and users via Android telephones and/or PC.  Experiment with Java or Android APIs for-Bluetooth device discovery -Bluetooth service discovery-Bluetooth data communication 4.Set up a database server to store information on:-Network configuration -State of individual sensor nodes-temperature measurement data  (If time allows)5.Set up a user interface for remote users to:-View the state of the network and individual nodes-control the light switches  -view temperature data-set temperature measurement intervals6.Fine tuning: -To make the network extensible with minimal effort -To perform global changes in network configuration and various settings-To introduce security features at different levels  -To build an intuitive GUI for remote users


Detailed Instructions:
-Set up a Zigbee networki.Network configuration (addressing, channel assignment,)ii.Router configurationiii.Communication between Zigbee and Bluetooth -Use the Zigbee API mode to address the nodes individually.  Unlike in Lab3 where messages are broadcast to all nodes in the network, only one specified Zigbee node should be able to receive a message sent to it.-Set up one Zigbee node (end device) to measure room temperature (sensor)-Set up one Zigbee node (end device) for controlling a light switchRecall the tasks you performed in Lab 3. Now you start with doing the same thing, but in API mode (not AT mode).  Use the X-CTU software on your PC to send the command to the Arduino and display the confirmation received.   Note that you should access Zigbee nodes via API mode programmatically.  You need to read the structure of API packets on the documentation. 1.Set up a Zigbee network with a coordinator and at least 2 end devices (nodes).2.Connect one end device to an Arduino board and the other end device to your PC.   3.Wire the arduino board and a LED, so that the LED will be switched on if the output is ‘high’ and switched off if the output is ‘low’. 4.Add another end device to your network to measure temperature data.  Modify your program above to send sensor data to the PC. 

Programming tasks: 
Hint: extend your work in Lab3 to meet the objectives.-Communication between Zigbee and a PC or phone via Bluetooth-A program for communication between the PC/phone and the database server-A program for communication between a remote user and end devicesi.For managing Zigbee nodes (e.x. turn on off light, change sensor settings)ii.Monitor environment via the Zigbee network iii.View measured data-System integration
