'use strict';

const { Contract } = require('fabric-contract-api');

class MyChaincode extends Contract {
  async initLedger(ctx) {
    console.log('Ledger initialized');
    return 'Ledger initialized';
  }

  async createAsset(ctx, assetId, value) {
    const asset = {
      assetId,
      value,
    };
    await ctx.stub.putState(assetId, Buffer.from(JSON.stringify(asset)));
    return asset;
  }

  async readAsset(ctx, assetId) {
    const assetBytes = await ctx.stub.getState(assetId);
    if (!assetBytes || assetBytes.length === 0) {
      throw new Error(`Asset ${assetId} not found`);
    }
    return JSON.parse(assetBytes.toString());
  }
}

module.exports = MyChaincode;