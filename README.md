# EchoNeighbors - Your Friendly Neighborhood Security System 🏠

EchoNeighbors is a prototype pre-emptive neighborhood security system that protects homeowners by alerting the users' 2-3 nearest neighbors in the case of intruder break-ins. In addition, the project features an anti-forgetting mechanism that alerts users if they are leaving home without their keys, wallets, ID cards, or any other customizable item the user wishes to remember to carry with them.  Our team is currently working on adding new features, such as alerts if there are gas leaks, fires, open garage doors when the user is away, floods, or any other disaster. 
<br>
<br> The project was built within 24 hours as part of IEEE's 2025 Hardware Hackathon.

EchoNeighbors has 2 main functions:

1) Immediately and automatically notifies 2-3 nearest neighbors in the case of an intrusion or break-in
2) Anti-Forgetting Mechanism: Pre-emptively alerts user if keys, wallets, or ID cards have been left behind


Team Name: The Breadboard Bandits
<br> 
Team Members: Raghav Bharath, Tadael Mihret, Arnav Revankar, Nadir Ghani, Munachim Akanno

Tools Used: SolidWorks CAD, ESP32 S3-Devkit C-1, RFID scanner with 2 tags, 4x4 Matrix Membrane Keypad, 2 Ultrasonic Sensors, Servo Motor, 9V battery, MB102 Breadboard Adapter, Piezo Buzzer, Resistors, FSR Pressure Sensor

Resources Used: 3D Printers, Laser Cutting Machine, Soldering Machine

Software Used: C++, FastAPI Backend, JavaScript, PlatformIO


## Problem Statement

<ul> 
 <li> Home intrusions remain a major concern in residential security, with an estimated 2.5 million burglaries occurring annually in the United States, according to FBI crime reports
66% of these are home break-ins, and over 65% of burglaries happen during the day when homeowners are at work or school </li>
<li> While the average police response time for a home security alarm ranges from 10 to 20 minutes, sometimes too late to prevent theft or catch intruders. </li>
<li> On the other hand, neighbors are typically within seconds of your home</li>
   <ul>
      <li> Studies show that neighborhood watch programs can reduce crime by up to 26% </li> 
      <li> Offers a faster, human-first layer of intervention. Neighbors can visually confirm the intrusion, provide real-time eyewitness accounts, or deter intruders by simply with their presence </li>
   </ul>
 
<li> A major reason for these break-ins are people also frustratingly misplacing keys, which can also delay schedules, cause stress, or lead to being locked out. Studies suggest the average person spends 10 minutes a day looking for lost items, with keys being among the top three most misplaced objects. </li>

</ul>

## Project Photos 

![Image](https://github.com/user-attachments/assets/9f115c1f-634d-40c5-8f24-48b46119b268)
![Image](https://github.com/user-attachments/assets/33b9ec82-4b1a-4875-b350-4f8360e4085b)

![Image](https://github.com/user-attachments/assets/daf43d92-ebb7-457a-86a7-b2ee9b10c944)

## How it works
Alert Neighbors: Imagine you are walking into your house through the main door with EchoNeighbors installed at your house. As you enter, right outside of the door, there will be an ultrasonic sensor above the door casing or door head that points downwards, scanning to detect if there is a person at the door front. To the right of the door, there will be a module box presenting you with 2 options: 1) an RFID scanner, and 2) a 4x4 Keypad. Based on your preference, you will either scan your ID or type in your password to enter the home. The ID card and Numpad passwords will be set when first installing the system at your house. Once you open the door, a symmetric replica of the outdoor ultrasonic sensor will be on the inside of the door, detecting that you have now entered the house. If a person enters the house passing through both sensors after scanning the RFID or typing the Numpad password, it is a legal entry. If a person bypasses the RFID scan and the Numpad, the person is an intruder. We used an RFID scanner, a numpad, 2 ultrasonic sensors, a servo motor, and an ESP32 S3-Devkit C-1 Module. The idea is, there is an ultrasonic sensor on the outside of the door and one on the inside. In the latter case, a push alert notification will be sent to 2-3 nearest neighbors through the FastAPI backend, which will then display it on the React Native frontend. If there is a legal entry, a signal will be sent to trigger a servo motor to unlock the door, permitting the user to enter the home. To detect if a user is entering or leaving the house, we wrote an algorithm to determine which ultrasonic sensor is tagged first. 


Reminder & Anti-Forgetting Mechanism: After entering the house, there will be a keyholder offering 4 slots to put your keys, wallets, or ID cards. When the key is placed on the holder, there is a see-saw mechanism that presses up against the FSR pressure sensors on the backend, allowing it to measure up to 100 Newtons or 20 lbs. If a weight above the empty slot weight is detected, that means that the key has been placed. After setting the key, the next time the user exits the home, the ultrasonic sensor inside the house will detect the user trying to leave, which will then trigger the piezo buzzer and an LED that will remind the user to take the item from the holder. Moreover, on the app, there are systems in development that allow the user to receive alerts if the key is still on the holder by a certain time (ex. 8am). 


## Project Demos 🎥

<ul>
    <li><a href="https://youtu.be/NRGQGvXyf9I?si=fxueIcJhqP37jlyk">Security System Circuitry Demo</a></li>
    <li><a href="https://youtube.com/shorts/avhj43t_PJ4?feature=share">In Progress App Demo</a></li>
  </ul>


## Challenges we ran into
Backend integration: One complication was integrating the backend API with the ESP32's web server/hotspot. Essentially, once the intruder is recognized, the ESP32 should send/receive a signal to/from the FastAPI backend using HTTP requests and WiFI. Once the FastAPI backend receives the signal, the frontend will display the UI on the screen with the appropriate information depending on the backend code. Our issue with this process was connecting the web server generated by the ESP32 to control the API backend. Essentially, the MakeNJIT network had a firewall blocking the FastAPI port, so we had to use the ESP32's "hotspot" as the data communication device, which resulted in a lot of data formatting issues and protocol mismatches. Along with such issues, we also had issues using the Expo Map UI for the map application, which resulted in us not being able to use a real-time GPS-like UI to alert nearby houses, and rather show a custom-designed imported image with pop-ups based on alerts. 


CAD: Another prominent problem we encountered was trying to figure out the design for the keyholder CAD system. The pressure sensors given in the MakeNJIT toolkit are quite fragile and don't measure too accurately. Therefore, trying to figure out the weighing mechanism for the keys on the holder was quite difficult. Initially, we thought of designing a spring, which, when weighed down, will disconnect an aluminium foil electric connection to the circuit board. When there was a low or 0V signal, the microcontroller would recognize this state, and would then set the keyIsFound() state to high. However, the issue with this design was that the spring was practically hard to install onto the back side of the keyholder system, as it would take up too much space. Moreover, the aluminium physical connection could be quite funky, as it could be unstable and shaky. Therefore, we decided to go with the see-saw idea. With this idea, the weight would press down on one side, lifting the other side towards an FSR pressure sensor. The weight of the key will be proportional to the weight applied to the sensor. However, parts of the see-saw took too long to print, as the queue for the 3D printers was too long. In addition, certain parts were printed in an unflexible filament that took up too much space, resulting in us having to redo the idea. 


## What we learned
Hardware, IoT, and Embedded Systems
- Learned about BLE (Bluetooth Low Energy) communication for proximity-based item tracking 

- Explored microcontroller interfacing with ESP32 WHO using BLE modules for real-time object presence detection - incomplete during hackathon, currently updating

- How to debug UART inputs, design smart circuits to detect object removal, and integrate WiFi communication and HTTP requests

Backend, APIs, Software
- How to develop a cross-platform mobile app using React Native with real-time alerts
  
- Built a FastAPI backend to manage real-time event handling and API routing

- How to use WebSockets or polling for near-instant alerts between devices
  
- Implemented ngrok to expose FastAPI backend to mobile apps during development

Security & UX Considerations
- Learned to manage secure neighbor-to-neighbor alerting via authenticated device IDs or IP restrictions - incomplete, currently in the works
  
- Designed a lightweight REST API for mobile-to-backend communication

- Explored the integration device sensor data into a mobile UI via asynchronous APIs


## Future improvements
<ul>
   <li> One improvement to be made is the design of a notification system that would send alerts even if your neighbors don't have the phone app (similar to Amber Alert or through SMS). This system will utilize a form of satellite communication </li>
   <li> Another improvement that can be made are alert triggers for if there are gas leaks, fires, open garage doors when the user is away, floods, or any other disasters; for this, we are using KY-026 Flame Sensor Modules, MQ5 Gas Sensors, and SE045 Water Sensors. </li>
   <li> Last but not least, a real time map feature using React Native Expo Map UI features would be really practical. Using this feature, the user's real time locations can be updated. For instance, if the user was on vacation, using the real time location updates, the GPS would be able to locate 2-3 nearest neighbors and send them alerts. </li>
</ul>


