import os # for environment variables and API keys security
import typer # for command line interface (print, input, etc.)
from openai import OpenAI
from dotenv import load_dotenv # for loading environment variables from .env file
from typing import Optional # for optional arguments (needed on Linux)

# load environment variables from .env file
load_dotenv()

client = OpenAI(api_key=os.getenv("OPENAI_API_KEY")) # for GPT-3 / GPT-4 API

# create a new Typer app instance
app = typer.Typer()

@app.command()
def interactive_chat(
	test: Optional[str] = typer.Option(None, "--text", "-t", help="Start with text to chat with the model."),
	temperature: float = typer.Option(0.7, help="Controls Randomness: Lower value means more predictable text.."),
	max_tokens: int = typer.Option(
		100, help="Controls the length of the response."
	),
	model: str = typer.Option(
		"gpt-3.5-turbo", help="The model to use for generating the response."
	),
):
	# messages = []

	# start the chat loop
	while True:
		# getting the prompt from the host
		while not os.path.exists('/usr/src/app/host_to_container.txt'):
			pass

		with open('/usr/src/app/host_to_container.txt', 'r') as host_to_container:
			prompt = host_to_container.readline().strip()
			print(f"\tReceived from host: {prompt}")

		# removing the file after reading the prompt
		os.remove('/usr/src/app/host_to_container.txt')

		# saving each prompt to the messages list
		# messages.append({"role": "user", "content": prompt})

		# if prompt.lower() == "exit":
		# 	typer.echo("Goodbye!")
		# 	break

		# getting the response from the model via the OpenAI API
		response = client.chat.completions.create(
			model=model,
			# messages=messages,
			messages=[
				{"role": "system", "content": "You are a helpful assistant that answers questions from users in a helpdesk chat. Your answers conscise and straight to the point. You are using GPT-3.5-turbo model. You are using bro like language"},
				{"role": "user", "content": prompt}
			],
			temperature=temperature,
			max_tokens=max_tokens,
		)
		# response = "..example response from GPT-3.5"

		with open('/usr/src/app/container_to_host.txt', 'w') as container_to_host:
			container_to_host.write(response.choices[0].message.content)
			print(f"Sent to host: {response.choices[0].message.content}")
			# container_to_host.write(response)
			# print(f"\tSent to host: {response}")

		# saving each response to the messages list
		# messages.append({"role": "assistant", "content": response.choices[0].message.content})

if __name__ == "__main__":
	app()
