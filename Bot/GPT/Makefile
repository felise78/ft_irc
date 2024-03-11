# MAKING LIFE BRIGHTER
RED = \033[0;31m
GREEN = \033[0;32m
BLUE = \033[0;34m
CYAN = \033[0;36m
YELLOW = \033[0;33m
MAGENTA = \033[0;35m
NC = \033[0m

NAME = bot-gpt

build:
	@echo "$(MAGENTA)Building $(NAME)...$(NC)"
	docker build -t $(NAME) .

run:
	@echo "$(MAGENTA)Launching $(NAME) container...$(NC)"
	docker run --detach -v .:/usr/src/app --rm --name $(NAME) --env-file .env $(NAME)
	@echo "$(GREEN)Container is UP and running! Container ID ^^^$(NC)"
# docker run -it --rm --name $(NAME) --env-file .env $(NAME)

down:
	@echo "$(RED)Stopping and removing $(NAME) container...$(NC)"
	@docker stop $(NAME) || true
	@echo "$(RED)container $(NAME) removed$(NC)"
# @docker rm $(NAME) || true

clean:
	@echo "$(RED)Removing All Images...$(NC)"
	@if [ -n "$$(docker images -q)" ]; then docker rmi $$(docker images -q); else echo "$(BLUE)No images to remove.$(NC)"; fi
	rm -rf container_to_host* host_to_container*
	@echo "$(RED)All images removed.$(NC)"

ls:
	@echo "$(CYAN)= = = = = = = = = = = = = = = = = = = = = = = = = = $(NC)"
	@echo "$(YELLOW)>>> $(MAGENTA)Listing all images...$(CYAN)"
	@docker images
	@echo "$(NC)"
	@echo "$(YELLOW)>>> $(MAGENTA)Listing all containers...$(CYAN)"
	@docker ps -a
	@echo "$(NC)"
# @echo "$(YELLOW)>>> $(MAGENTA)Listing all volumes...$(CYAN)"
# @docker volume ls
# @echo "$(NC)"
	@echo "$(CYAN)= = = = = = = = = = = = = = = = = = = = = = = = = = $(NC)"

.PHONY: build run clean ls

