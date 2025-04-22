#!/usr/bin/python
# Employees Listener program

import mariadb
import socket
import string

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

            print("The Whole line" + l)

            if len(term) == 2:
                print("the Temperature is " + str(value))
                print("the time is " + str(term[1]))
                try:
                    value = (term[0] - 449.8)/13.146)  # inserting rescale here
                    time_str = term[1].strip()  # keep time as string
                    cur.execute("INSERT INTO TEMP (VAL, TIME) VALUES (?, ?)", (value, time_str))
                    conn.commit()
                    print(f"Last Inserted ID: {cur.lastrowid}")
                except mariadb.Error as e:
                    print(f"Error: {e}")
                    sockconn.close()
                    conn.close()
                except ValueError as ve:
                    print(f"Invalid value: {ve}")
            else:
                print("something went wrong" + l)
                sockconn.close()
                conn.close()
