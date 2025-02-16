FROM node:16


# Establece el directorio de trabajo en el contenedor
WORKDIR /app

COPY . .

# Instala Truffle globalmente
RUN npm install axios dotenv @truffle/hdwallet-provider 
RUN npm install
RUN npm install -g truffle

RUN truffle init
RUN truffle compile 

# RUN truffle migrate --network development

CMD ["/bin/bash"]