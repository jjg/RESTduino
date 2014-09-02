#!/usr/bin/python

# simple python script to read network interface utilization and display via RESTDuino

import httplib
from math import trunc
from time import sleep
from pysnmp.entity.rfc3413.oneliner import cmdgen

# config
restduino_address = '10.0.1.3'
router_address = '10.1.10.1'
router_communitystring = 'public'
interface_oid = '1.3.6.1.2.1.31.1.1.1.6.15' # '1.3.6.1.2.1.2.2.1.10.5' # Apple Airport Extreme vlan0
gauge_max = 50 # if the meter has an existing scale, this is the largest number
warn_threshold = 100 # when to turn on yellow led (currently pwm value, should be gauge value) 
danger_threshold = 200 # when to turn on red led (currently pwm value, should be gauge value) 
gauge_update_threshold = 10 # how often (in seconds) to send an update to RESTDuino (10 is safe)

# globals
last_sample = 0
gauge_update_counter = 0
value_avg_accumulator = 0
		
# turn on green led
conn = httplib.HTTPConnection(restduino_address)
gauge_url = '/14/HIGH'
conn.request('GET', gauge_url)
conn.close()

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
			conn = httplib.HTTPConnection(restduino_address)
			gauge_url = '/5/%d' % value_avg 
			conn.request('GET', gauge_url)
			conn.close()

			# set led indicators
			# need to wait between requests or we may overwhelm RESTduino
			sleep(1)

			# set yellow led 
 			if value_avg > warn_threshold:
				gauge_url = '/15/HIGH'
			else:
				gauge_url = '/15/LOW'

			conn = httplib.HTTPConnection(restduino_address)
			conn.request('GET', gauge_url)
			conn.close()

			sleep(1)

			# set red led
			if value_avg > danger_threshold:
				gauge_url = '/16/HIGH'
			else:
				gauge_url = '/16/LOW'

			conn = httplib.HTTPConnection(restduino_address)
			conn.request('GET', gauge_url)
			conn.close()
		
		except:
			print('error displaying reading, waiting for next round')

		gauge_update_counter = 0
		value_avg_accumulator = 0

	last_sample = this_sample
	
	sleep(1)
