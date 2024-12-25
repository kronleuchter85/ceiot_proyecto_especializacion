FROM node:16

# Instala Truffle globalmente
RUN npm install -g truffle

# Establece el directorio de trabajo en el contenedor
WORKDIR /app

COPY . .

CMD ["truffle", "develop"]