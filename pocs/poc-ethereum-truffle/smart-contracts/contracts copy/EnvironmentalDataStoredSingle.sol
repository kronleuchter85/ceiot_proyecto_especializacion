// SPDX-License-Identifier: MIT
pragma solidity ^0.8.0;

contract EnvironmentalDataStoredSingle {
    // Estructura para almacenar los datos ambientales
    struct EnvironmentalReading {
        uint256 timestamp; // Marca de tiempo
        int256 temperature; // Temperatura en grados Celsius
        uint256 pressure;   // Presión en hPa
        uint256 humidity;   // Humedad relativa en porcentaje
        uint256 luminosity; // Luminosidad en lux
    }

    // Arreglo dinámico para almacenar el historial de lecturas
    EnvironmentalReading public reading;

    // Evento para notificar que se registró una nueva lectura
    event NewReading(
        uint256 indexed timestamp,
        int256 temperature,
        uint256 pressure,
        uint256 humidity,
        uint256 luminosity
    );

    // Función para registrar una nueva lectura
    function recordReading(
        int256 _temperature,
        uint256 _pressure,
        uint256 _humidity,
        uint256 _luminosity
    ) public {
        // Crear una nueva lectura
        EnvironmentalReading memory newReading = EnvironmentalReading({
            timestamp: block.timestamp,
            temperature: _temperature,
            pressure: _pressure,
            humidity: _humidity,
            luminosity: _luminosity
        });

        reading = newReading;

        // Emitir el evento
        emit NewReading(block.timestamp, _temperature, _pressure, _humidity, _luminosity);
    }


    function getReading() public view returns (EnvironmentalReading memory) {
        return reading;
    }

}
