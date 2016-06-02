LoRa
====

We use the following configuration::

	Raspberry Pi 
		|
		| (UART)
		|
	LoRa module
		|
		| (LoRa)
		|
	Proximus 
		|
		| (HTTP)
		|
	server UA 
		|
		| (HTTP)
		|
	own VM (also at UA)

The code for the VM at the end of the chain can be found in `lora_server`.

Currently, it is just a very simple Tornado server that logs all POST requests it gets.

