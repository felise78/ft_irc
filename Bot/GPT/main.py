# This is the main program which acts as a server designed to run in the container
# It listens for incoming connections from the client and sends back the prompt received from the client

import os
import socket
from openai import OpenAI


client = OpenAI(api_key=os.getenv("OPENAI_API_KEY"))

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Read the HOST_IP from the environment variables
# hostname = os.getenv("HOSTNAME")
port = int(os.getenv("NET_PORT"))
server_address = ('0.0.0.0', port) # must be `0.0.0.0` to allow connections from outside the container
sock.bind(server_address)

sock.listen(1)

while True:
	# Wait for a connection
	print("Waiting for a connection...")
	connection, client_address = sock.accept()

	print(f"Connection from {client_address}")

	try:
		print(f"Connection from {client_address}")

		while True:
			prompt = connection.recv(512)
			print(f"Received from client: {prompt}")

			if prompt:
				# Decoding the prompt from bytes to string
				prompt_str = prompt.decode('utf-8').strip()

				response = client.chat.completions.create(
					model="gpt-3.5-turbo",
					messages=[
						{"role": "system", "content": "You are a helpful assistant that answers questions from users in a helpdesk chat. Your answers conscise and straight to the point. You are using strong bro-like language."},
						{"role": "user", "content": prompt_str}
					],
					temperature=0.7,
					max_tokens=50,
				)

				text_to_send = response.choices[0].message.content
				
				print(f"Sending response back to the client: {text_to_send}\n")
				connection.sendall(text_to_send.encode('utf-8'))

				
				# print(f"Sending prompt back to the client: {prompt}\n")
				# connection.sendall(prompt)
			else:
				print("No more prompt from client\n")
				break

	finally:
		# Clean up the connection
		connection.close()

if __name__ == '__main__':
	pass
