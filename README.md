# Terminal project

To install and run, in your terminal do:

```
# git clone https://github.com/dsmak/terminal.git
# cd terminal/build
# make build
# ./server
```

The server is running on your localhost:8888

To access the API requests must be send to localhost:8888/devices

To get the list of available devices you may do:
```
# curl -X GET http://localhost:8888/devices
```
To get the details of the device with the particalar ID, add ID to your request
```
# curl -X GET http://localhost:8888/devices/{{ID_OF_DEVICE_YOU_WANT}}
```
To connect new device to the system, use POST request 
```
# curl -X POST -d \
# 'json={"cardType": ["Visa", "MasterCard", "EFTPOS" ], "TransactionType":["Cheque", "Savings", "Credit"]}' \
# http://localhost:8888/devices \
#/
```
The server will accept json file, attach the available ID and respond with updated information back to the terminal.

Tests

[Ceedling](http://www.throwtheswitch.org/ceedling) is used for tests.
To install Ceedling please refer to the documentation the link above.
To run tests type ```ceedling test:storage``` and  ```ceedling test:json_processor``` in your cli.
Please note that storage tests has to be run sequentially due to shared resources.
Comment and uncomment TEST_IGNORE_MESSAGE when required.

Resources:
[GNU libmicrohttpd Tutorial](https://www.gnu.org/software/libmicrohttpd/tutorial.html)\
[GNU microhttpd Documentation](https://www.gnu.org/software/libmicrohttpd/)\
[JSON-C](https://github.com/json-c/json-c/wiki)\
[Ceedling](https://github.com/ThrowTheSwitch/Ceedling)\
[Add unit tests to your current project with Ceedling](http://www.electronvector.com/blog/add-unit-tests-to-your-current-project-with-ceedling)\
[Unity](https://github.com/ThrowTheSwitch/Unity)\
