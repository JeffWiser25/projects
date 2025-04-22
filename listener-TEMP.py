#!/usr/bin/python
# Employees Listener program

# Import the required modules
import mariadb  # For connecting to the MariaDB database
import socket   # For creating a network socket (to receive data from Arduino)
import string   # Not used here, can be removed

# Connect to the MariaDB database
conn = mariadb.connect(
    user="tudublin",             # Database username
    password="tudublinpwd",      # Database password
    host="localhost",            # Database host (local machine)
    database="WEATHER"           # Name of the database
)

# Create a cursor object to interact with the database
cur = conn.cursor()

# Set up the socket connection
HOST = ''         # Listen on all available network interfaces
PORT = 20001      # Port number used for the server (must match Arduino)

# Create the socket (using TCP)
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Bind the socket to the address and port
s.bind((HOST, PORT))

# Start listening for incoming connections (only one allowed here)
s.listen(1)
print('listening')

# Accept a connection from a client (Arduino)
sockconn, addr = s.accept()

print('Connection from Arduino at IP Address:', addr)

# Infinite loop to keep receiving data
while 1:
    print('waiting')
    data = sockconn.recv(1024)  # Wait to receive up to 1024 bytes
    if not data:
        break  # If no data, stop the loop

    print(data)  # Print the raw received data

    # Loop through each line of the received data
    for line in data.splitlines():
        if len(line) > 0:
            l = line.decode('utf-8')  # Convert from bytes to string
            term = l.split(",")       # Split by comma: [temperature, time]

            print("The Whole line" + l)

            # Check if the data has exactly 2 parts: temperature and time
            if len(term) == 2:
                print("the Temperature is " + str(term[0]))
                print("the time is " + str(term[1]))
                try:
                    value = float(term[0])       # Convert temperature to float
                    time_str = term[1].strip()   # Clean the time string
                    # Insert the temperature and time into the TEMP table
                    cur.execute("INSERT INTO TEMP (VAL, TIME) VALUES (?, ?)", (value, time_str))
                    conn.commit()  # Save the changes to the database
                    print(f"Last Inserted ID: {cur.lastrowid}")  # Optional: show ID of last inserted row
                except mariadb.Error as e:
                    # If there's a database error, print it and close connections
                    print(f"Error: {e}")
                    sockconn.close()
                    conn.close()
                except ValueError as ve:
                    # If conversion of temperature fails, print error
                    print(f"Invalid value: {ve}")
            else:
                # If the line is not in expected format, print it and close connections
                print("something went wrong" + l)
                sockconn.close()
                conn.close()
