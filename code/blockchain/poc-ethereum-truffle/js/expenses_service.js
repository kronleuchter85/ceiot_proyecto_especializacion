const {Web3} = require("web3");
const axios = require("axios");

const ETH_USD_RATE_URL = 'https://api.coingecko.com/api/v3/simple/price?ids=ethereum&vs_currencies=usd';

class ExpenseService {


    constructor(providerUrl){
        
        if (ExpenseService.instance) {
            return ExpenseService.instance;
        }
        
        this.web3 = new Web3(providerUrl);
        this.expenses = new Array();
        this.setETHPriceInUSD();

        ExpenseService.instance = this;
    }


     setETHPriceInUSD() {
        try {
            const response = axios.get(ETH_USD_RATE_URL).then( response => {
                this.ethUsdRate = response.data.ethereum.usd;
                }
            );
        } catch (error) {
            console.error('Error al obtener el precio de ETH:', error);
        }
    }

    addExpenseInformation(expenseInfo){
        this.expenses.push(expenseInfo);
    }

    getExpenseInformation(contractName , methodName , gasUsed , gasPrice){
        const gasCostInETH = BigInt(gasUsed) * BigInt(gasPrice);
        const gasCostInETHFormatted = this.web3.utils.fromWei(gasCostInETH, 'ether');
        const gasCostInUSD = parseFloat(gasCostInETHFormatted) * this.ethUsdRate;
        const expenseInfo = {
            'operation':`${contractName}.${methodName}`,
            'gasUsed': gasUsed,
            'costEth': gasCostInETHFormatted,
            'eth/usd': this.ethUsdRate,
            'costUSD': gasCostInUSD
        };
        return expenseInfo;
    }

    getExpenses(){
        return this.expenses;
    }

}




module.exports = ExpenseService;