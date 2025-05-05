FROM node:16


# Establece el directorio de trabajo en el contenedor
WORKDIR /app


# Instala Truffle globalmente
RUN npm install axios dotenv @truffle/hdwallet-provider  json-stringify-safe @aws-sdk/client-dynamodb @aws-sdk/lib-dynamodb
# RUN npm install
RUN npm install -g truffle

RUN truffle init
RUN truffle compile 

COPY . .
# RUN truffle migrate --network development

CMD ["/bin/bash"]