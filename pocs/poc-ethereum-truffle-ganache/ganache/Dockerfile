# Usa una imagen base de Node.js
FROM node:16

# Instala Ganache CLI
RUN npm install -g ganache

# Expone el puerto por defecto de Ganache (7545)
EXPOSE 7545

# Ejecuta Ganache CLI por defecto al iniciar el contenedor
CMD ["ganache", "--host", "0.0.0.0"]