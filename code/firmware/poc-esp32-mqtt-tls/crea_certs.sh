#!/bin/bash

# -- Set local IP --
IP1="192.168.0.3"
IP2="eclipse-mosquito-tls"

SUBJECT_CA="/C=AR/ST=CABA/L=CABA/O=FiUBA/OU=CA/CN=$IP1/CN=$IP2"
SUBJECT_SERVER="/C=AR/ST=CABA/L=CABA/O=FiUBA/OU=Server/CN=$IP1/CN=$IP2"
SUBJECT_CLIENT="/C=AR/ST=CABA/L=CABA/O=FiUBA/OU=Client/CN=$IP1/CN=$IP2"


function generate_CA () {
   echo "$SUBJECT_CA"
   openssl req -x509 -nodes -sha256 -newkey rsa:2048 -subj "$SUBJECT_CA"  -days 365 -keyout ca.key -out ca.crt
}

function generate_server () {
   echo "$SUBJECT_SERVER"
   openssl req -nodes -sha256 -new -subj "$SUBJECT_SERVER" -keyout server.key -out server.csr
   openssl x509 -req -sha256 -in server.csr -CA ca.crt -CAkey ca.key -CAcreateserial -out server.crt -days 365
}

function generate_client () {
   echo "$SUBJECT_CLIENT"
   openssl req -new -nodes -sha256 -subj "$SUBJECT_CLIENT" -out client.csr -keyout client.key 
   openssl x509 -req -sha256 -in client.csr -CA ca.crt -CAkey ca.key -CAcreateserial -out client.crt -days 365
}

function copy_keys_to_broker () {
   mv ca.crt ./mosquitto/certs/
   mv server.crt ./mosquitto/certs/
   mv server.key ./mosquitto/certs/
}

generate_CA
generate_server
generate_client
# copy_keys_to_broker
