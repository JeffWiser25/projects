import serial
import time
import os

# Try to open the serial connection
try:
    # Set the correct COM port (e.g., "COM3" on Windows, "/dev/ttyUSB0" on Linux)
    ser = serial.Serial('COM3', 9600, timeout=1)
except serial.SerialException:
    # Print error message if Arduino is not connected
    print("Error: Could not connect to Arduino. Please connect the Arduino, upload the program, and open the Serial Monitor.")
    exit()

# Folder where the logs will be saved (Desktop path)
desktop_path = os.path.expanduser("~/Desktop")
log_folder = os.path.join(desktop_path, "wind-sensor-logs")

# Create the folder if it doesn't exist
if not os.path.exists(log_folder):
    os.makedirs(log_folder)

# Create a new log file with timestamp
filename = f"wind_data_{time.strftime('%Y-%m-%d_%H-%M-%S')}.txt"
file_path = os.path.join(log_folder, filename)

with open(file_path, "w") as file:
    file.write("Time (s), Wind Speed (km/h), Blade Count\n")  # Header for the file

    start_time = time.time()

    while True:
        line = ser.readline().decode().strip()  # Read data from Arduino

        if "Wind Speed:" in line:  # Check if the line contains the wind speed
            try:
                # Extract wind speed and counter values from the sentences
                wind_speed = float(line.split(":")[1].strip().split()[0])  # Wind speed value
            except ValueError:
                continue  # Skip if we can't parse the value

        elif "Counter:" in line:  # Check if the line contains the counter
            try:
                blade_count = int(line.split(":")[1].strip())  # Blade count value
            except ValueError:
                continue  # Skip if we can't parse the value

            # Calculate elapsed time
            elapsed_time = time.time() - start_time

            # Write the data to the file
            file.write(f"{elapsed_time:.2f}, {wind_speed}, {blade_count}\n")

            # Print the data to the console
            print(f"Time: {elapsed_time:.2f}s, Wind Speed: {wind_speed} km/h, Counter: {blade_count}")

            # Optional: you can add a small delay to slow down logging
            time.sleep(0.1)
