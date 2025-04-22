#!/usr/bin/python
# Employees Listener program

import mariadb
import socket

# Connect to mariadb
conn = mariadb.connect(
    user="tudublin",
    password="tudublinpwd",
    host="localhost",
    database="WEATHER"
)
cur = conn.cursor()

HOST = ''  # all available interfaces
PORT = 20001  # unprivileged ports are >20000

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind((HOST, PORT))
s.listen(1)
print('listening')
sockconn, addr = s.accept()

print('Connection from Arduino at IP Address:', addr)

while 1:
    print('waiting')
    data = sockconn.recv(1024)
    if not data:
        break
    print(data)
    for line in data.splitlines():
        if len(line) > 0:
            l = line.decode('utf-8')
            term = l.split(",")

            # Check if there are exactly 3 elements
            if len(term) == 3:
                wind_speed = (1.6406*(term[0]*term[0])-10.06)  # Wind Speed rescale here
                unit = term[1]         # Unit
                time = term[2]         # Time
                # Print in the desired format: VAL, UNIT, TIME
                print(f"{wind_speed},{unit},{time}")

                try:
                    value = float(wind_speed)  # Convert to float for VAL
                    cur.execute("INSERT INTO WINDSP (VAL, UNIT, TIME) VALUES (?, ?, ?)", (value, unit, time))
                    conn.commit()
                    print(f"Last Inserted ID: {cur.lastrowid}")
                except mariadb.Error as e:
                    print(f"Error: {e}")
                    sockconn.close()
                    conn.close()
                except ValueError as ve:
                    print(f"Invalid value: {ve}")
            else:
                print("Invalid data format:", l)
                sockconn.close()
                conn.close()

# Close the connection to the database
conn.close()
