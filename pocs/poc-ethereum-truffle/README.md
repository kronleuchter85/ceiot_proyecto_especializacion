# ceiot_proyecto_especializacion


Comandos utiles de la api Web3 para interactuar con la red

## inicializar el cliente Truffle para ejecutar queries Web3
truffle console --network development

## obtener un bloque random
block = await web3.eth.getBlock(<blockId | blockHash>)

## Obtener el ultimo bloque de la red:
block = await web3.eth.getBlock('latest')

## Obtener la lista de cuentas
accounts = await web3.eth.getAccounts()

## obtener el balance de una cuenta
web3.eth.getBalance( <accountId> )

## obtener las transacciones de un bloque
block.transactions

## obtener detalles de una transaccion
tx = await web3.eth.getTransaction( <txHash> )


## Instanciacion de un Smart Contract desde la api Web3. Por ejemplo 'Storage' con un metodo 'set' y otro 'get'
const storage = await Storage.deployed();

// Guardar un valor
await storage.set(42);

// Obtener el valor
const value = await storage.get();
console.log(value.toString());