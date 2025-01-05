FROM node:16

# Crear un directorio de trabajo
WORKDIR /app

# Instalar Truffle globalmente
RUN npm install -g truffle

# Copiar archivos del proyecto
COPY . .

# Exponer el puerto de Truffle Dashboard (24012)
EXPOSE 24012

# Define el comando predeterminado para iniciar una shell
CMD ["bash"]