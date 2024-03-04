
NAME = bot-gpt

build:
	@echo "Building $(NAME)..."
	docker build -t $(NAME) .

run:
	@echo "Launching $(NAME) container..."
	# docker run -it --rm --name $(NAME) --env-file .env $(NAME)
	docker run -v .:/usr/src/app --rm --name $(NAME) --env-file .env $(NAME)

clean:
	@echo "Removing $(NAME) image..."
	@if [ -n "$$(docker images -q)" ]; then docker rmi $$(docker images -q); else echo "No images to remove."; fi
	rm -rf container_to_host* host_to_container*

ls:
	@echo "Listing all images..."
	@docker images
	@echo "Listing all containers..."
	@docker ps -a
	@echo "Listing all volumes..."
	@docker volume ls

.PHONY: build run clean ls

