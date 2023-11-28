import paho.mqtt.subscribe as subscribe
import csv
from time import strftime

#Opens CSV file for writing and sets sets it so that every line will end in a newline.
with open('mqtt_data.csv', mode='w', newline='') as csvfile:

    #Creates a csv.writer object for the csvfile. Values should be seperated by commas.
    data_writer = csv.writer(csvfile, delimiter=',', quotechar='"', quoting=csv.QUOTE_MINIMAL)

    #Writer object writes the headers to the file. 
    data_writer.writerow(['Timestamp', 'Temperature', 'Pressure', 'Humidity', 'Altitude', 'Air Quality', 'Motion'])

    #Infinite while loop.
    while True:

        #An object is created according to the subscriber.simple function, passing the topic and broker address.
        msg_temp = subscribe.simple("789hb23rtemp", hostname="test.mosquitto.org")

        #Prints the payload of the incoming message to the CSV file.
        print("%s" % ( msg_temp.payload))

        #Decodes the MQTT message and gets rid of formatting that is the result of the MQTT message format.
        #The value is also turned into a float and stored in a variable.
        value_temp = float(msg_temp.payload.decode("utf-8").strip("[]'"))

        #An object is created according to the subscriber.simple function, passing the topic and broker address.
        msg_pres = subscribe.simple("789hb23rpres", hostname="test.mosquitto.org")

        #Prints the payload of the incoming message to the CSV file.
        print("%s %s" % (msg_pres.topic, msg_pres.payload))

        #Decodes the MQTT message and gets rid of formatting that is the result of the MQTT message format.
        #The value is also turned into a float and stored in a variable.
        value_pres = float(msg_pres.payload.decode("utf-8").strip("[]'"))

        #An object is created according to the subscriber.simple function, passing the topic and broker address.
        msg_hum = subscribe.simple("789hb23rhum", hostname="test.mosquitto.org")

        #Prints the payload of the incoming message to the CSV file.
        print("%s %s" % (msg_hum.topic, msg_hum.payload))

        #Decodes the MQTT message and gets rid of formatting that is the result of the MQTT message format.
        #The value is also turned into a float and stored in a variable.
        value_hum = float(msg_hum.payload.decode("utf-8").strip("[]'"))

        #An object is created according to the subscriber.simple function, passing the topic and broker address.
        msg_air = subscribe.simple("789hb23rair", hostname="test.mosquitto.org")

        #Prints the payload of the incoming message to the CSV file.
        print("%s %s" % (msg_air.topic, msg_air.payload))

        #Decodes the MQTT message and gets rid of formatting that is the result of the MQTT message format.
        #The value is also turned into a float and stored in a variable.
        value_air = float(msg_air.payload.decode("utf-8").strip("[]'"))

        #An object is created according to the subscriber.simple function, passing the topic and broker address.
        msg_mot = subscribe.simple("789hb23rmot", hostname="test.mosquitto.org")

        #Prints the payload of the incoming message to the CSV file.
        print("%s %s" % (msg_mot.topic, msg_mot.payload))

        #Decodes the MQTT message and gets rid of formatting that is the result of the MQTT message format.
        #The value is also turned into a float and stored in a variable.
        value_mot = float(msg_mot.payload.decode("utf-8").strip("[]'"))

        #Instructs the writer object to write the varaibles into a row of the csv file with the first column being data and time
        #then the variables.
        data_writer.writerow([strftime("%Y-%m-%d %H:%M:%S"), value_temp, value_pres, value_hum, value_alt, value_air, value_mot])

        #The objects buffer is flushed and the stored values in the buffer are immedietly pushed onto the CSV file.
        csvfile.flush()
