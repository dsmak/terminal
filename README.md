#Terminal project

To install and run, in your terminal do:

'''
# git clone https://github.com/dsmak/terminal.git
# cd terminal/build
# make build
# ./server
'''

The server is running on your localhost:8888

To access the API requests must be send to localhost:8888/devices

To get the list of available devices you may do:
'''
# curl -X GET http://localhost:8888/devices
'''
To get the details of the device with the particalar ID, add ID to your request
'''
# curl -X GET http://localhost:8888/devices/{{ID_OF_DEVICE_YOU_WANT}}
'''
To connect new device to the system, use POST request 
'''
# curl -X POST -d \
# 'json={"cardType": ["Visa", "MasterCard", "EFTPOS" ], "TransactionType":["Cheque", "Savings", "Credit"]}' \
# http://localhost:8888/devices \
#/
'''
The server will accept json file, attach the available ID and respond with updated information back to the terminal.

Resources:
[GNU libmicrohttpd Tutorial](https://www.gnu.org/software/libmicrohttpd/tutorial.html).
[GNU microhttpd Documentation](https://www.gnu.org/software/libmicrohttpd/)
[JSON-C](https://github.com/json-c/json-c/wiki)
