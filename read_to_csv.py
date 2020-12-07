from sys import argv
from datetime import datetime
import serial
import os

HEADER = "time,t_thermistor,t_dht,humidity"
CSV_DIRECTORY = "csv"


def main():
    if not len(argv) == 2:
        print("Usage: <script.py> path-to-device")
        exit()

    device_name = argv[1]
    filename = datetime.now().strftime("%Y-%m-%d") + ".csv"
    filepath = os.path.join(CSV_DIRECTORY, filename)

    if not os.path.exists(filepath):
        os.makedirs(CSV_DIRECTORY, exist_ok=True)
        outfile = open(filepath, "a")
        outfile.write(f"{HEADER}\n")
    else:
        outfile = open(filepath, "a")

    try:
        # we should receive a measurement every 5 minutes
        # timeout is 600s=10min
        handle = serial.Serial(device_name, 9600, timeout=600)
        while True:
            line = handle.readline()
            try:
                # try to decode it; skip on error
                values = line.strip().decode("utf-8").split(",")
                #  print(values)
            except Exception as err:
                print("Skipping because of error: {err}")
                continue

            # check if we acutally received three values
            # and if value length is longer than 0 (empty)
            if len(values) == 3 and all([len(x) > 0 for x in values]):
                # strip the second number after the comma
                # eg. strip number 2 with string 23.42
                string = ",".join([val[:-1] for val in values])
                time = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
                string = f"{time},{string}"

                #  print(string)
                outfile.write(f"{string}\n")
                outfile.flush()

    except Exception as err:
        print(err)
    finally:
        handle.close()
        outfile.close()


if __name__ == "__main__":
    main()
