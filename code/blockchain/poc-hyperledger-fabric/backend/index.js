const { Gateway, Wallets } = require('fabric-network');
const path = require('path');
const fs = require('fs');

const main = async () => {
  try {
    const ccpPath = path.resolve(__dirname, '../config/connection.json');
    const ccp = JSON.parse(fs.readFileSync(ccpPath, 'utf8'));

    const walletPath = path.join(__dirname, 'wallet');
    const wallet = await Wallets.newFileSystemWallet(walletPath);

    const gateway = new Gateway();
    await gateway.connect(ccp, {
      wallet,
      identity: 'admin',
      discovery: { enabled: true, asLocalhost: true },
    });

    const network = await gateway.getNetwork('mychannel');
    const contract = network.getContract('mychaincode');

    console.log('Submitting transaction...');
    const result = await contract.submitTransaction('createAsset', 'asset1', '100');
    console.log(`Transaction result: ${result.toString()}`);

    await gateway.disconnect();
  } catch (error) {
    console.error(`Error: ${error}`);
    process.exit(1);
  }
};

main();