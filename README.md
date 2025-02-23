# ceiot_proyecto_especializacion

==========================================================
AWS services
==========================================================
- AWS IoT Core
    - Topic MQTT + Certs
    - Rules para routera mensajes
- AWS S3
    - Para recibir los mensajes routeados desde MQTT
    - Para storage data temporal Athena
    - Para storage de terraform
- AWS Glue Catalog
    - Database
    - Table de mensajes MQTT
- AWS App Runner: 
    - Para dApp
- AWS Dynamo:
    - Collections:
        - ABI
        - Expenses
        - Contracts Registry
- AWS Systems Manager:
     - Parameters store:
        - Alchemy project id
        - Wallet account address
        - Blockchain URLs
- AWS Secret Manager: 
    - Secret: Para key del wallet


==========================================================
Blockchain services
==========================================================
- Ethereum/Sepolia: no setup
- Etherscan: no setup
- Chainlist: no setup
- Alchemy:
    - Proyecto: Para el acceso a redes Ethereum
- Metamask:
    - Wallet + Account: Para fondear operaciones
- Google Blockchain Faucets


==========================================================
Dev tools
==========================================================
- Truffle
- Solidity
- Docker + Docker Compose
- Ganache
- Node.js
- Web3.js
- ESP-IDF


==========================================================
Actividades
==========================================================
- Configuracion de credenciales AWS en la maquina local.
- Creacion de componentes AWS
- Modificacion de codigo ESP32 para incluir conexion segura MQTT
- Despliegue de certificados MQTT de AWS IoT Core en ESP32
- Configuracion de AWS Secret (private key) para escritura blockchain
- Despliegue de firmware ESP32

- Despliegue de Smart Contracts y dApp
- Despliegue de data pipeline
- Ejecucion del contenedor con terraform montando credenciales AWS
