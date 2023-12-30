all:
		sudo mkdir -p /home/hwankim/data/wordpress
		sudo mkdir -p /home/hwankim/data/database
		sudo chmod 777 /etc/hosts
		echo "127.0.0.1 hwankim.42.fr" >> /etc/hosts
		sudo docker-compose -f srcs/docker-compose.yml up --build -d

up:
		sudo docker-compose -f srcs/docker-compose.yml up --build -d

fclean:
		sudo docker-compose -f srcs/docker-compose.yml down --rmi all --volumes
		if [ -n "$(sudo docker images -q debian:buster)" ]; then sudo docker rmi debian:buster; fi
		sudo rm -rf /home/hwankim/data

.PHONY:	all up fclean