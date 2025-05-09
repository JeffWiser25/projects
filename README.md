# projects - Daryl Sweeney - Electrical and control engineering - TU705
a collection of my different project codes:
# project_1: The Robosimo: Johnny705 - robot car
The car goes around a track, with different colours. It uses the colours to guide itself. It being the predecessor to the Robosumo project.
C programming used here aswell as alternate maps, speeds and colour polarity switching. The colours are based on the TCRT5000 infra-red sensor,
picking up the reflected light bounced back after being emitted.
https://github.com/JeffWiser25/projects/tree/JeffWiser25-project_1
# project_2: The shapesorter assignment: the sorting of pics
The program sorts .pgm pics that are generated by a blob.exe program given in the package. Through the development stage there were a number of
versions of the shapesorter program. The main mark was given to students who were able to tell whether the object was a blob or solid, or a ring.
through determination of sequencially scanning from the top then, when hitting black, counting the pixels and counting vertically and horizontally inward. If a
gap was detected it would be classified a ring, otherwise a blob or solid. Using this sequencial logic and counting a near centerpoint could be calculated and 
then a 20 x 20 white square could be painted there, if it was a blob, or otherwise a black one if a ring. The program was then able to separate rings and solids into separate folders as copies, then
adjusted copies were put into modified folders and an original folder was made with copies of the originals. When run in the windows command terminal with a directory, folder or image,
would sort the images accordingly. During this project we used "Irfan view 64" to give us an idea, but not neccessary.
https://github.com/JeffWiser25/projects/tree/JeffWiser25-project_2
# project_3: The Weather Station Simulation & Data Visualization Dashboard python scripts
These automated, instead of adding one entry at a time to a mariadb table in a database, you could add many entries, or also in a loop, with an option to how many times the loop would run.
Also scripts using a netcat listener for a client input to a database on the server side, which could be used in an arduino. I based our database on a Weather-station being built in another module,
and added two scripts which were temperature and windspeed. I adjusted the listening scripts to rescale the input from the client according to our groups figures.
https://github.com/JeffWiser25/projects/tree/JeffWiser25-project_3
# project_4: Robosumo competition
Robosumo is a competitive robotics project developed as part of TU Dublin’s engineering program. The objective is to design, build, and program an autonomous sumo robot capable of detecting, avoiding, and pushing its opponent out of the ring without human control. Our robot is built using an Arduino Nano, multi TCRT5000 infrared sensors for edge and line detection, and HC-SR04 ultrasonic sensors for opponent tracking. Through multiple prototypes, simulations in Tinkercad, and real-world soldered circuits, the robot was fine-tuned for performance within strict size and weight limits. This repository includes all the Arduino code, sensor calibration, logic testing, and documented steps leading up to the final robot. The project showcases skills in embedded systems, C programming, sensor integration, electronics, teamwork, and iterative design—preparing us for the Robosumo tournament challenge.
https://github.com/JeffWiser25/projects/tree/JeffWiser25-project_4_Robosumo
# project_5: Multi-Sensor Arduino Instrumentation and IoT Integration Project
This project is a hands-on exploration into instrumentation and IoT integration, where I designed and built four distinct sensor circuits using Arduino Nano microcontrollers and basic circuitry principles. The sensors include a PT100 temperature sensor configured with a Wheatstone bridge, a wind sensor leveraging a repurposed bottle and windmill design, an LDR lux sensor set up in a voltage divider configuration, and a humidity sensor. Working both individually and collaboratively, I developed the necessary Arduino code, applied mathematical and instrumentation theories, and built each circuit to reliably capture and transmit real-time data. This project was integrated with a Grafana-based dashboard that visualizes the sensor data stored in a MariaDB database, providing dynamic and accessible insights into the measurements.
https://github.com/JeffWiser25/projects/tree/JeffWiser25-project_5_Multi-Sensor-Arduino-Instrumentation-and-IoT-Integration-Project?tab=readme-ov-file

