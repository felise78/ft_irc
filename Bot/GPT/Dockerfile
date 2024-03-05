FROM python:alpine

WORKDIR /usr/src/app

RUN pip install --no-cache-dir typer openai python-dotenv

COPY main.py /usr/src/app

EXPOSE 80

CMD ["python", "./main.py"]
