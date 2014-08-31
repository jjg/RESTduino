#!/usr/bin/python

# simple python script to read network interface utilization and display via RESTDuino

import httplib
from math import trunc
from time import sleep
from pysnmp.entity.rfc3413.oneliner import cmdgen

# config
router_address = '10.0.1.1'
router_communitystring = 'public'
interface_oid = '1.3.6.1.2.1.2.2.1.10.5' # Apple Airport Extreme vlan0
gauge_max = 50 # if the meter has an existing scale, this is the largest number
gauge_update_threshold = 10 # how often (in seconds) to send an update to RESTDuino (10 is safe)

# globals
last_sample = 0
gauge_update_counter = 0
value_avg_accumulator = 0
		
while True:

	cmdGen = cmdgen.CommandGenerator()
	
	errorIndication, errorStatus, errorIndex, varBinds = cmdGen.getCmd(
	    cmdgen.CommunityData(router_communitystring),
	    cmdgen.UdpTransportTarget((router_address, 161)),
	    cmdgen.MibVariable(interface_oid)
	)
	
	this_sample = float(varBinds[0][1])
	this_value = this_sample - last_sample
	
	this_value_bits = this_value * 8
	this_value_kilobits = this_value_bits / 1000
	this_value_megabits = this_value_kilobits / 1000
	this_value_scale = round(((this_value_megabits / gauge_max) * 255))
	
	if this_value_scale > 255:
		this_value_scale = 255

	value_avg_accumulator = value_avg_accumulator + this_value_scale
	gauge_update_counter = gauge_update_counter + 1

	if gauge_update_counter > gauge_update_threshold:

		value_avg = value_avg_accumulator / gauge_update_threshold

		try:
			print('%f octets, %f megabits, %d scale' % (this_value, this_value_megabits, value_avg))

			# update restduino
			conn = httplib.HTTPConnection('10.0.1.3')
			gauge_url = '/5/%d' % value_avg 
			conn.request('GET', gauge_url)
			conn.close()
		
		except:
			print('error taking or displaying reading, waiting for next round')

		gauge_update_counter = 0
		value_avg_accumulator = 0

	last_sample = this_sample
	
	sleep(1)
