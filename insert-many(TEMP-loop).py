import mariadb  # Library for connecting to MariaDB
import sys  # System-related functionality (e.g. exiting the script)
import random  # Library to generate random numbers
import string  # Library to generate random letters or characters
from datetime import datetime, timedelta  # For handling and manipulating dates and times

# Connect to MariaDB Platform
try:
    # Attempt to establish a connection to the MariaDB database using credentials
    conn = mariadb.connect(
        user="tudublin",         # Database username
        password="tudublinpwd",  # Password for the database user
        host="127.0.0.1",        # Localhost IP address (same machine)
        port=3306,               # Default MariaDB port
        database="WEATHER"       # Name of the database you're connecting to
    )
except mariadb.Error as e:
    # If connection fails, print the error message and exit the program
    print(f"Error connecting to MariaDB Platform: {e}")
    sys.exit(1)  # Exit the script with error code 1

# Create a cursor object which is used to execute SQL queries
cur = conn.cursor()

# Function to generate a random string of letters and digits
def random_string(length=5):
    # Return a random string of specified length (default is 5 characters)
    return ''.join(random.choices(string.ascii_uppercase + string.digits, k=length))

# Function that generates fake/random data entries to simulate sensor input
def generate_random_entries(num_entries):
    entries = []  # Create an empty list to store all generated entries
    base_time = datetime(2024, 3, 3, 22, 45, 0)  # Set a fixed starting date and time

    # Loop to generate the number of entries requested
    for _ in range(num_entries):
        val = round(random.uniform(5.0, 15.0), 2)  # Random value between 5.0 and 15.0 (rounded to 2 decimals)
        lat = round(random.uniform(90.0, 100.0), 4)  # Random latitude value
        longt = round(random.uniform(7.0, 15.0), 4)  # Random longitude value
        unit = random.choice(['m/s', 'kph', 'ft/s'])  # Randomly select a measurement unit
        time_offset = timedelta(minutes=random.randint(0, 60))  # Add random number of minutes (0-60)
        time = (base_time + time_offset).strftime('%H:%M:%S')  # Format time as a string
        sens_id = random_string(4)  # Generate a 4-character sensor ID
        note = random.choice(['entry one', 'entry two', 'entry three', 'random entry'])  # Pick a random note

        # Create a tuple with all the values and add it to the list
        entries.append((val, lat, longt, unit, base_time.strftime('%Y-%m-%d'), time, sens_id, note))

        # Increase the base time by 5 minutes for the next entry
        base_time += timedelta(minutes=5)

    # Return the full list of generated entries
    return entries

# Define how many fake entries to generate
num_entries = 80  # You can change this number if needed

# Generate the entries using the function above
entries = generate_random_entries(num_entries)

# Try to insert the generated entries into the TEMP table in the database
try:
    # Insert all the entries using a single batch command
    cur.executemany(
        "INSERT INTO TEMP (VAL, LAT, LONGT, UNIT, DATE, TIME, SENS_ID, NOTE) "
        "VALUES (?, ?, ?, ?, ?, ?, ?, ?)",  # SQL query with placeholders
        entries  # Data to be inserted
    )
except mariadb.Error as e:
    # If there's an error during insert, print the error
    print(f"Error: {e}")
else:
    # If insertion is successful, commit the transaction
    conn.commit()
    print(f"{num_entries} Entries inserted successfully.")  # Print number of successful entries
    print(f"Last Inserted ID: {cur.lastrowid}")  # Show the last ID inserted into the table

# Close the connection to the database
conn.close()
