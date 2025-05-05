FROM node:16

WORKDIR /app

RUN npm install axios dotenv @truffle/hdwallet-provider  json-stringify-safe @aws-sdk/client-dynamodb @aws-sdk/lib-dynamodb
# RUN npm install
RUN npm install -g truffle

RUN truffle init
RUN truffle compile 

COPY ./smart-contracts/ /app
COPY ./js/repository.js /app/migrations


CMD ["/bin/bash"]