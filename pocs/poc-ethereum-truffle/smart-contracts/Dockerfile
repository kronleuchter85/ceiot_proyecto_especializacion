FROM node:16

# Instala Truffle globalmente
RUN npm install -g truffle

# Establece el directorio de trabajo en el contenedor
WORKDIR /app

COPY . .

RUN truffle init
RUN truffle compile 
# RUN truffle migrate --network development

CMD ["/bin/bash"]