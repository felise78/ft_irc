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
	touch host_to_container.txt container_to_host.txt
	chmod 777 host_to_container.txt container_to_host.txt
	@echo "$(GREEN)Image '$(NAME)' is ready!$(NC)"
	@echo "$(GREEN)Type 'make run' to launch GPT app in the container.$(NC)"

run:
	@echo "$(MAGENTA)Launching $(NAME) container...$(NC)"
# docker run --detach -v .:/usr/src/app --rm --name $(NAME) --env-file .env $(NAME)
	docker run --detach -v $(PWD):/usr/src/app --rm --name $(NAME) --env-file .env $(NAME)
	@echo "$(GREEN)Container is UP and running! Container ID ^^^$(NC)"
	@echo "$(GREEN)You can use the 'bot' in 'ircserv' by sending 'PRIVMSG' to 'NeoBot' in '#helpdesk' channel.$(NC)"
# docker run -it --rm --name $(NAME) --env-file .env $(NAME)

down:
	@echo "$(RED)Stopping and removing $(NAME) container...$(NC)"
	@docker stop $(NAME) || true
	@echo "$(RED)container $(NAME) removed$(NC)"
# @docker rm $(NAME) || true

login:
	@echo "$(YELLOW)>>> $(MAGENTA)Logging into $(NAME)...$(NC)"
	@docker exec -it $(NAME) /bin/sh
	@echo "$(NC)"

log:
	@echo "$(YELLOW)>>> $(MAGENTA)Showing logs for $(NAME)...$(NC)"
	@docker logs $(NAME)
	@echo "$(NC)"

clean: down
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

