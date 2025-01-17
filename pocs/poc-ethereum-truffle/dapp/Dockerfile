FROM node:16

WORKDIR /app

COPY . .
RUN npm install web3 dotenv express body-parser axios
RUN npm install
RUN npm install -g truffle

CMD ["node", "index.js"]