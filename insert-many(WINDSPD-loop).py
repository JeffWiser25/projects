import mariadb  # Used to connect to MariaDB database
import sys  # Used to handle system-level operations like exiting the program
import random  # Used for generating random numbers
import string  # Used for generating random strings
from datetime import datetime, timedelta  # Used for working with dates and times

# Try to connect to the MariaDB database
try:
    conn = mariadb.connect(
        user="tudublin",         # Username for the database
        password="tudublinpwd",  # Password for the user
        host="127.0.0.1",        # Localhost (same computer)
        port=3306,               # Port number for MariaDB (default is 3306)
        database="WEATHER"       # Name of the database you want to use
    )
except mariadb.Error as e:
    # If connection fails, print the error and exit the program
    print(f"Error connecting to MariaDB Platform: {e}")
    sys.exit(1)

# Create a cursor so we can run SQL commands
cur = conn.cursor()

# Function to create a random string (for example, a sensor ID)
def random_string(length=5):
    return ''.join(random.choices(string.ascii_uppercase + string.digits, k=length))

# Function to generate a list of fake/random weather entries
def generate_random_entries(num_entries):
    entries = []  # List to store the generated entries
    base_time = datetime(2024, 3, 3, 22, 45, 0)  # Starting time for the first entry

    # Loop to create the desired number of entries
    for _ in range(num_entries):
        val = round(random.uniform(5.0, 15.0), 2)  # Random value for wind speed
        lat = round(random.uniform(90.0, 100.0), 4)  # Random latitude
        longt = round(random.uniform(7.0, 15.0), 4)  # Random longitude
        unit = random.choice(['M/S', 'KPH', 'FT/S'])  # Random unit for wind speed
        time_offset = timedelta(minutes=random.randint(0, 60))  # Random time offset up to 60 mins
        time = (base_time + time_offset).strftime('%H:%M:%S')  # Create time string with offset
        sens_id = random_string(4)  # Random sensor ID (4 characters)
        note = random.choice(['ENTRY ONE', 'ENTRY TWO', 'ENTRY THREE', 'RANDOM ENTRY'])  # Random comment/note
        temp_id = random.randint(1, 100)  # Random TEMP_ID (possibly linking to another table)

        # Add the data as a tuple to the list
        entries.append((val, lat, longt, unit, base_time.strftime('%Y-%m-%d'), time, sens_id, note, temp_id))

        # Move base time forward by 5 minutes for next entry
        base_time += timedelta(minutes=5)

    return entries  # Return the list of entries

# Set how many entries you want to insert
num_entries = 80

# Generate the entries
entries = generate_random_entries(num_entries)

# Try to insert all entries into the WINDSP table
try:
    cur.executemany(
        "INSERT INTO WINDSP (VAL, LAT, LONGT, UNIT, DATE, TIME, SENS_ID, NOTE, TEMP_ID) "
        "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)",  # SQL command with placeholders
        entries  # The list of tuples to insert
    )
except mariadb.Error as e:
    # Print the error if insertion fails
    print(f"Error: {e}")
else:
    # Commit the transaction if everything goes well
    conn.commit()
    print(f"{num_entries} Entries inserted successfully.")  # Confirm how many entries were added
    print(f"Last Inserted ID: {cur.lastrowid}")  # Show the ID of the last entry

# Close the database connection
conn.close()
