// SPDX-License-Identifier: MIT
pragma solidity ^0.8.0;

contract EnvironmentalData2 {
    struct EnvironmentalReading {
        string deviceId;
        string readingType; // 'type' es palabra reservada en Solidity
        string value;
        string geoLat;
        string geoLong;
        string date;
        string time;
    }

    // Evento para nueva lectura
    event NewReading(
        string deviceId,
        string readingType,
        string value,
        string geoLat,
        string geoLong,
        string date,
        string time
    );

    function recordReading(
        string memory _deviceId,
        string memory _readingType,
        string memory _value,
        string memory _geoLat,
        string memory _geoLong,
        string memory _date,
        string memory _time
    ) public {
        emit NewReading(_deviceId, _readingType, _value, _geoLat, _geoLong, _date, _time);
    }
}
