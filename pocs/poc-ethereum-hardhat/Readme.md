docker build -t poc-ethereum-hardhat-docker .

docker run -it -v $(pwd):/app -w /app poc-ethereum-hardhat-docker bash

#
# dentro del contenedor, ejecutar los comandos de Truffle: 
#
npx hardhat
npx hardhat compile