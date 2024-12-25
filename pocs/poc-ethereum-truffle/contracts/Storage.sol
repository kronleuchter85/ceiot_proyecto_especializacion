pragma solidity ^0.8.0;

contract Storage {
    uint256 private data;


    constructor(uint _data) {
        require(_data > 0, "Data must be greater than 0");
        data = _data;
    }

    function set(uint256 x) public {
        data = x;
    }

    function get() public view returns (uint256) {
        return data;
    }
}