FROM node:16

WORKDIR /app


# RUN npm install js-yaml
RUN npm install web3 dotenv express body-parser axios @truffle/hdwallet-provider swagger-ui-express @aws-sdk/client-dynamodb @aws-sdk/lib-dynamodb 
RUN npm install -g truffle

COPY ./js/repository.js ./js/utils.js /app
COPY ./dapp/ /app

CMD ["node", "index.js"]