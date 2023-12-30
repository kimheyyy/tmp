FROM  debian:buster

# install php
RUN apt-get update -y && apt-get upgrade -y
RUN apt-get install -y php
# php와 mysql을 연동하기위한 확장프로그램 설치
RUN apt-get install -y php-mysql
#압축도구 설치
RUN apt-get install -y tar
RUN apt-get install -y php-fpm7
# 파일다운 도구 설치
RUN apt-get install -y wget

# install wordpress
# 최선버전의 wordpress 다운
RUN wget https://wordpress.org/latest.tar.gz
# 다운받은거 압축풀고
RUN tar xzvf latest.tar.gz
# 압축파일 삭제하고
RUN rm latest.tar.gz
# 압축해제한 디렉토리는 옮기고
RUN mv -f wordpress /var/www/
# www-data 사용자와 그룹으로 변경
RUN chown -R www-data:www-data /var/www/wordpress

#config wordpress
RUN rm -rf /var/www/wordpress/wp-config-sample.php
COPY ./conf/wp-config.php /var/www/wordpress
COPY ./conf/www.conf /etc/php/7.3/fpm/pool.d/www.conf
RUN mkdir -p /run/php/
# 9000포트 노툴
EXPOSE 9000
# php-fpm은 서버와 php간의 통신개선 및 리소스 관리를 해주는 역할로 동시에 오는 php 요청을 병렬적으로 처리가능
ENTRYPOINT ["php-fpm7.3", "-F"]