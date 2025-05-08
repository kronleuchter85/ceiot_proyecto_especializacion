FROM node:16

WORKDIR /app


# RUN npm install web3 dotenv  axios @truffle/hdwallet-provider
RUN npm install express body-parser
# RUN npm install -g truffle

COPY index.js .

CMD ["node", "index.js"]