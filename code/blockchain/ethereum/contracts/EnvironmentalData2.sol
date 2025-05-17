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
        string date,
        string time,
        string deviceId,
        string geoLat,
        string geoLong,
        string readingType,
        string value
    );

    function recordReading(
        string memory _date,
        string memory _time,
        string memory _deviceId,
        string memory _geoLat,
        string memory _geoLong,
        string memory _readingType,
        string memory _value
    ) public {
        emit NewReading(_deviceId, _readingType, _value, _geoLat, _geoLong, _date, _time);
    }
}
