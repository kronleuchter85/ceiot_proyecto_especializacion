const Storage = artifacts.require("Storage");

module.exports = function (deployer) {
    console.log("Iniciando el despliegue de Storage");
    deployer.deploy(Storage, 42)
        .then(() => console.log("Despliegue exitoso"))
        .catch((err) => console.error("Error en el despliegue:", err));
};