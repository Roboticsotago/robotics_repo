import time
message = 0
def send2pd(message):
	print(str(message) + ";" )

while True: 
	message += 1
	send2pd(message)
