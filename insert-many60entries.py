# Module Imports
import mariadb   # This imports the MariaDB connector to interact with the database
import sys       # This allows us to exit the program if something goes wrong

# Try to connect to the MariaDB database
try:
    conn = mariadb.connect(
        user="tudublin",         # username for the database
        password="tudublinpwd",  # password for the database
        host="127.0.0.1",        # database is hosted locally (localhost)
        port=3306,               # default MariaDB port
        database="WEATHER"       # the name of the database we're connecting to
    )
except mariadb.Error as e:
    # If connection fails, print the error and exit the program
    print(f"Error connecting to MariaDB Platform: {e}")
    sys.exit(1)

# Create a cursor object to interact with the database
cur = conn.cursor()

# A list of sample weather data entries to insert into the database
# Each entry includes: temperature, latitude, longitude, unit, date, time, sensor ID, and a note
entries = [
    (12.12, 97.0087, 7.8907, 'm/s', '2024-03-03', '22:45:01', '25rt', 'reading 1'),
    (14.55, 96.0000, 8.9000, 'kph', '2024-03-03', '22:46:00', '25rt', 'reading 2'),
    (10.25, 98.1234, 7.6543, 'm/s', '2024-03-03', '22:47:00', '25rt', 'reading 3'),
    (11.78, 96.4523, 8.4523, 'm/s', '2024-03-03', '22:48:00', '25rt', 'reading 4'),
    (13.40, 97.7832, 7.9231, 'kph', '2024-03-03', '22:49:00', '25rt', 'reading 5'),
    (9.85, 96.3021, 8.4120, 'm/s', '2024-03-03', '22:50:00', '25rt', 'reading 6'),
    (15.20, 97.2341, 7.9341, 'kph', '2024-03-03', '22:51:00', '25rt', 'reading 7'),
    (13.95, 98.1230, 7.2345, 'm/s', '2024-03-03', '22:52:00', '25rt', 'reading 8'),
    (12.67, 96.8734, 8.1234, 'kph', '2024-03-03', '22:53:00', '25rt', 'reading 9'),
    (11.90, 97.0001, 7.0001, 'm/s', '2024-03-03', '22:54:00', '25rt', 'reading 10'),
    (10.44, 97.1234, 7.5432, 'kph', '2024-03-03', '22:55:00', '25rt', 'reading 11'),
    (9.99, 96.6543, 8.0987, 'm/s', '2024-03-03', '22:56:00', '25rt', 'reading 12'),
    (14.33, 97.0987, 7.3456, 'm/s', '2024-03-03', '22:57:00', '25rt', 'reading 13'),
    (13.77, 98.0000, 7.1000, 'kph', '2024-03-03', '22:58:00', '25rt', 'reading 14'),
    (10.88, 96.7890, 8.1111, 'm/s', '2024-03-03', '22:59:00', '25rt', 'reading 15'),
    (11.33, 97.3210, 7.3210, 'kph', '2024-03-03', '23:00:00', '25rt', 'reading 16'),
    (14.88, 96.1111, 8.2222, 'm/s', '2024-03-03', '23:01:00', '25rt', 'reading 17'),
    (12.00, 97.8888, 7.9999, 'm/s', '2024-03-03', '23:02:00', '25rt', 'reading 18'),
    (13.50, 98.4321, 7.1234, 'kph', '2024-03-03', '23:03:00', '25rt', 'reading 19'),
    (10.10, 96.2000, 8.1000, 'm/s', '2024-03-03', '23:04:00', '25rt', 'reading 20'),
    (11.80, 97.2100, 7.7100, 'm/s', '2024-03-03', '23:05:00', '25rt', 'reading 21'),
    (13.10, 96.8900, 8.2100, 'kph', '2024-03-03', '23:06:00', '25rt', 'reading 22'),
    (12.90, 97.6700, 7.6700, 'm/s', '2024-03-03', '23:07:00', '25rt', 'reading 23'),
    (9.80, 98.2000, 7.8700, 'm/s', '2024-03-03', '23:08:00', '25rt', 'reading 24'),
    (10.75, 96.7900, 8.0900, 'kph', '2024-03-03', '23:09:00', '25rt', 'reading 25'),
    (14.12, 97.2500, 7.2500, 'm/s', '2024-03-03', '23:10:00', '25rt', 'reading 26'),
    (13.32, 96.0000, 8.0000, 'kph', '2024-03-03', '23:11:00', '25rt', 'reading 27'),
    (10.99, 97.4500, 7.5000, 'm/s', '2024-03-03', '23:12:00', '25rt', 'reading 28'),
    (11.45, 98.1000, 7.3000, 'kph', '2024-03-03', '23:13:00', '25rt', 'reading 29'),
    (12.88, 97.6000, 7.9000, 'm/s', '2024-03-03', '23:14:00', '25rt', 'reading 30'),
    (14.77, 96.9000, 8.5000, 'm/s', '2024-03-03', '23:15:00', '25rt', 'reading 31'),
    (13.11, 98.5000, 7.1000, 'kph', '2024-03-03', '23:16:00', '25rt', 'reading 32'),
    (9.67, 96.5678, 8.5678, 'm/s', '2024-03-03', '23:17:00', '25rt', 'reading 33'),
    (11.23, 97.1230, 7.6540, 'kph', '2024-03-03', '23:18:00', '25rt', 'reading 34'),
    (13.00, 96.3210, 8.3210, 'm/s', '2024-03-03', '23:19:00', '25rt', 'reading 35'),
    (12.12, 97.7654, 7.8765, 'm/s', '2024-03-03', '23:20:00', '25rt', 'reading 36'),
    (10.10, 96.3456, 8.1234, 'kph', '2024-03-03', '23:21:00', '25rt', 'reading 37'),
    (14.00, 97.9999, 7.1111, 'm/s', '2024-03-03', '23:22:00', '25rt', 'reading 38'),
    (13.85, 96.2222, 8.2222, 'kph', '2024-03-03', '23:23:00', '25rt', 'reading 39'),
    (11.90, 98.4444, 7.3333, 'm/s', '2024-03-03', '23:24:00', '25rt', 'reading 40'),
    (9.44, 96.6666, 8.4444, 'm/s', '2024-03-03', '23:25:00', '25rt', 'reading 41'),
    (13.30, 97.8888, 7.7777, 'kph', '2024-03-03', '23:26:00', '25rt', 'reading 42'),
    (12.50, 98.3333, 7.5555, 'm/s', '2024-03-03', '23:27:00', '25rt', 'reading 43'),
    (10.50, 97.1111, 7.8888, 'kph', '2024-03-03', '23:28:00', '25rt', 'reading 44'),
    (13.70, 96.9876, 8.5432, 'm/s', '2024-03-03', '23:29:00', '25rt', 'reading 45'),
    (11.77, 98.7654, 7.2345, 'm/s', '2024-03-03', '23:30:00', '25rt', 'reading 46'),
    (14.21, 97.6543, 7.4567, 'kph', '2024-03-03', '23:31:00', '25rt', 'reading 47'),
    (12.33, 96.5432, 8.1234, 'm/s', '2024-03-03', '23:32:00', '25rt', 'reading 48'),
    (9.88, 98.2100, 7.6400, 'kph', '2024-03-03', '23:33:00', '25rt', 'reading 49'),
    (10.67, 97.2100, 7.8400, 'm/s', '2024-03-03', '23:34:00', '25rt', 'reading 50'),
    (14.89, 98.1010, 7.9990, 'm/s', '2024-03-03', '23:35:00', '25rt', 'reading 51'),
    (13.10, 96.4567, 8.7654, 'kph', '2024-03-03', '23:36:00', '25rt', 'reading 52'),
    (12.77, 97.6543, 7.3210, 'm/s', '2024-03-03', '23:37:00', '25rt', 'reading 53'),
    (11.50, 96.1111, 8.0000, 'kph', '2024-03-03', '23:38:00', '25rt', 'reading 54'),
    (9.70, 98.3333, 7.1234, 'm/s', '2024-03-03', '23:39:00', '25rt', 'reading 55'),
    (14.12, 97.3210, 7.6543, 'm/s', '2024-03-03', '23:40:00', '25rt', 'reading 56'),
    (12.66, 96.7890, 8.5432, 'kph', '2024-03-03', '23:41:00', '25rt', 'reading 57'),
    (13.45, 97.0000, 7.0000, 'm/s', '2024-03-03', '23:42:00', '25rt', 'reading 58'),
    (10.44, 96.2000, 8.3000, 'kph', '2024-03-03', '23:43:00', '25rt', 'reading 59'),
    (11.99, 98.4000, 7.9000, 'm/s', '2024-03-03', '23:44:00', '25rt', 'reading 60')

]

# Try to insert the entries into the TEMP table in the database
try:
    cur.executemany(
        "INSERT INTO TEMP (VAL, LAT, LONGT, UNIT, DATE, TIME, SENS_ID, NOTE) "
        "VALUES (?, ?, ?, ?, ?, ?, ?, ?)",
        entries  # pass the list of entries to insert
    )
except mariadb.Error as e:
    # If there is a problem during insertion, print the error
    print(f"Error: {e}")
else:
    # If successful, commit the changes to the database
    conn.commit()
    print("Entries inserted successfully.")
    # Print the ID of the last inserted row (may refer to the last of the batch)
    print(f"Last Inserted ID: {cur.lastrowid}")

# Close the database connection when done
conn.close()
