FROM node:16

WORKDIR /app

COPY . .

RUN npm install web3 dotenv express body-parser axios @truffle/hdwallet-provider
RUN npm install
RUN npm install -g truffle

CMD ["node", "index.js"]