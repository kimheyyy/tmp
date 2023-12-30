#!/bin/sh
service mysql start; #maria db 서비스 시작

mysql -e "CREATE DATABASE IF NOT EXISTS $MARIA_DATABASE"; #환경변수로 지정된 이름의 데이터베이스 생성
mysql -e "CREATE USER IF NOT EXISTS '$MARIA_USER'@'%' IDENTIFIED BY '$MARIA_PASSWORD'"; #환경변수로 지정된 사용자와 패스워드를 이용해 사용자 생성
mysql -e "GRANT ALL PRIVILEGES ON $MARIA_DATABASE.* TO '$MARIA_USER'@'%'"; #위의 유저에게 이 데이터베이스의 모든 권한을 줌
mysql $MARIA_DATABASE -u root < ./wp_backup.sql #백업 파일로 데이터 베이스 복원작업
mysql -e "ALTER USER '$MARIA_ROOT'@'localhost' IDENTIFIED BY '$MARIA_ROOT_PASSWORD'; FLUSH PRIVILEGES;"
mysqladmin -u$MARIA_ROOT -p$MARIA_ROOT_PASSWORD shutdown #서비스 종료

exec mysqld #mariaDB 서비스 실행 (스크립트가 종료되고 난 후에도)