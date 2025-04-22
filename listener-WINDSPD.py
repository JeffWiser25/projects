#!/usr/bin/python
# Employees Listener program

import mariadb         # Import the MariaDB library to interact with the database
import socket          # Import the socket library for network communication

# Connect to mariadb
conn = mariadb.connect(
    user="tudublin",          # Username to access the MariaDB database
    password="tudublinpwd",   # Password for the database user
    host="localhost",         # Database is hosted on the same machine
    database="WEATHER"        # Name of the database to connect to
)
cur = conn.cursor()           # Create a cursor to execute SQL queries

HOST = ''           # Empty string means the server will listen on all available interfaces
PORT = 20001        # Port number to listen on (must match the sender device, e.g. Arduino)

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)  # Create a TCP socket
s.bind((HOST, PORT))          # Bind the socket to the host and port
s.listen(1)                   # Start listening for incoming connections (1 = max queue length)
print('listening')            # Let the user know it's ready

sockconn, addr = s.accept()   # Accept a connection from the client (e.g. Arduino)
print('Connection from Arduino at IP Address:', addr)  # Show client address

while 1:                      # Infinite loop to keep receiving data
    print('waiting')          # Waiting for data from the client
    data = sockconn.recv(1024)  # Receive up to 1024 bytes from the client
    if not data:              # If no data is received, break the loop
        break
    print(data)               # Print the raw data received (in bytes)

    for line in data.splitlines():  # Split data into individual lines (in case multiple are sent)
        if len(line) > 0:           # Only process non-empty lines
            l = line.decode('utf-8')  # Decode bytes to string
            term = l.split(",")       # Split the line into a list, using comma as separator

            # Check if there are exactly 3 elements (expected format: VAL, UNIT, TIME)
            if len(term) == 3:
                wind_speed = term[0]  # First value is wind speed
                unit = term[1]        # Second value is the unit (e.g., km/h, m/s)
                time = term[2]        # Third value is the time stamp
                # Print the values in a clear format
                print(f"{wind_speed},{unit},{time}")

                try:
                    value = float(wind_speed)  # Try to convert wind speed to a float
                    # Insert the data into the WINDSP table in the database
                    cur.execute("INSERT INTO WINDSP (VAL, UNIT, TIME) VALUES (?, ?, ?)", (value, unit, time))
                    conn.commit()  # Save the change to the database
                    print(f"Last Inserted ID: {cur.lastrowid}")  # Show ID of the new row
                except mariadb.Error as e:  # Handle database errors
                    print(f"Error: {e}")
                    sockconn.close()  # Close socket connection if error occurs
                    conn.close()      # Close database connection if error occurs
                except ValueError as ve:  # Handle invalid number format
                    print(f"Invalid value: {ve}")
            else:
                # If line does not have exactly 3 values, report it and close connections
                print("Invalid data format:", l)
                sockconn.close()
                conn.close()

# Close the connection to the database after exiting the loop
conn.close()
