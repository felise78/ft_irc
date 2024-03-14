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
		240, help="Controls the length of the response."
	),
	model: str = typer.Option(
		"gpt-3.5-turbo", help="The model to use for generating the response."
	),
):
	# """ Interactive CLI chat with Chat GPT via API """
	# typer.echo(
	# 	"Welcome to the interactive chat with Chat GPT! Type 'exit' to end the chat."
	# )

	messages = []

	# start the chat loop
	while True:
		# getting the prompt from the host
		with open('/usr/src/app/host_to_container.txt', 'r') as host_to_container:
			prompt = host_to_container.readline().strip()
			# print(f"Received from host: {prompt}")

		# saving each prompt to the messages list
		messages.append({"role": "user", "content": prompt})

		if prompt.lower() == "exit":
			# typer.echo("Goodbye!")
			break

		# getting the response from the model via the OpenAI API
		response = client.chat.completions.create(
			model=model,
			messages=messages,
			temperature=temperature,
			max_tokens=max_tokens,
		)

		# Creating a lock file for the host to wait for the response to be completed
		with open('/usr/src/app/lock', 'w') as lock:
			pass

		with open('/usr/src/app/container_to_host.txt', 'w') as container_to_host:
			container_to_host.write(response.choices[0].message.content)
			# print(f"Sending to host: {response.choices[0].message.content}")

		os.remove('/usr/src/app/lock') # Removing the lock file

		# saving each response to the messages list
		messages.append({"role": "assistant", "content": response.choices[0].message.content})

if __name__ == "__main__":
	app()
