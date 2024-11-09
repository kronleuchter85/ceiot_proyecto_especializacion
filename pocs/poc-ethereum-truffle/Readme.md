docker build -t poc-ethereum-truffle-docker .

docker run -it -v $(pwd):/app -w /app poc-ethereum-truffle-docker bash

#
# dentro del contenedor, ejecutar los comandos de Truffle: 
#
truffle init
truffle compile 
truffle migrate