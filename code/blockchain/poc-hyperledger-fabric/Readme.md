#
# Generacion de los Artefactos necesarios en cryto-config con cryptogen
#

docker run --rm \
  -v $(pwd)/config:/config \
  -v $(pwd)/artifacts:/artifacts \
  hyperledger/fabric-tools:2.5 \
  cryptogen generate --config=/config/crypto-config.yml --output=/artifacts/crypto-config


#
# Generar el Bloque Génesis con configtxgen
#


docker run --rm \
  -v $(pwd)/config:/config \
  -v $(pwd)/artifacts:/artifacts \
  hyperledger/fabric-tools:2.5 \
  configtxgen -profile OrdererGenesis -channelID system-channel -outputBlock /artifacts/genesis.block -configPath /config
