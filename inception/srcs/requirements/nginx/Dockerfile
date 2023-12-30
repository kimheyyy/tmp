# 베이스 이미지 가져오기
FROM debian:buster

# 패키지 목록 업데이트 및 패키지들 최신버전으로 nginx설치 및 openssl설치
RUN     apt-get update -y
RUN     apt-get upgrade -y
RUN     apt-get install -y nginx
RUN     apt-get install -y openssl

# 로컬에 존재하는 파일 옮기고
COPY    conf/default /
RUN     mv default /etc/nginx/sites-available/default
# 개인키 생성 -> CSR생성 -> 새로운 인증서 생성 -> 각 요소들 이동
RUN     openssl genrsa -out server.key 4096
RUN     openssl req -new -key server.key -subj "/C=KR/ST=Seoul/O=42Seoul/OU=GAM" -out server.csr
RUN     openssl req -x509 -in server.csr -key server.key -out server.crt
RUN     mv server.crt /etc/ssl/certs/
RUN     mv server.csr server.key /etc/ssl/private
# 443 포트로 노출
EXPOSE 443
# 컨테이너로 올라갈때 실행하는 명령어 nginx 를 포그라운드 모드로 실행
ENTRYPOINT     ["nginx", "-g", "daemon off;"]
