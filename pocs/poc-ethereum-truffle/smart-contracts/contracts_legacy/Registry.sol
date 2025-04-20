// SPDX-License-Identifier: MIT
pragma solidity ^0.8.0;

contract Registry {

    address public owner;
    
    mapping(string => address) public contracts; // Relaciona un nombre con la dirección del contrato



    struct RegistryEntry {
        string contractName;
        address contractAddress;
    }

    RegistryEntry[] entries;

    event ContractRegistered(string name, address contractAddress);

    modifier onlyOwner() {
        require(msg.sender == owner, "Only the owner can perform this action");
        _;
    }

    constructor() {
        owner = msg.sender; // El que despliegue será el propietario
    }

    function registerContract(string calldata name, address contractAddress) external onlyOwner {
        require(contractAddress != address(0), "Invalid contract address");
        contracts[name] = contractAddress;
     
        RegistryEntry memory newEntry = RegistryEntry({contractName:name , contractAddress: contractAddress});
        entries.push(newEntry);

        emit ContractRegistered(name, contractAddress);
    }

    function getContract(string calldata name) external view returns (address) {
        return contracts[name];
    }


    function getAllEntries() external view returns(RegistryEntry[] memory){
        return entries;
    }

}