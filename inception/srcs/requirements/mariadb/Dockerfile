#베이스 이미지를 받아서
FROM debian:buster

# 패키지 목록 업데이트 후 패키를 최신버전으로
RUN apt-get update -y && apt-get upgrade -y
#mariaDB설치
RUN apt-get install mariadb-server -y

# 이미지 안으로 로컬의 파일을 옮김
COPY tools/mariadb_setting.sh /mariadb_setting.sh
COPY tools/wp_backup.sql /wp_backup.sql
COPY ./conf/my.cnf /etc/mysql/my.cnf
#3306 포트 외부에 노출
EXPOSE 3306
#이미지 run으로 컨테이너 올릴때 실행할 명령어
ENTRYPOINT ["sh", "./mariadb_setting.sh"]
