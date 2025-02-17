terraform {
  backend "s3" {
    bucket         = "ceiot-iac-terraform"  # Nombre de tu bucket S3
    key            = "terraform.tfstate"  # Ruta del archivo de estado en el bucket S3
    region         = "eu-west-2"  # Región de AWS
    encrypt        = true  # Encriptación del estado en el bucket S3
    acl            = "private"  # ACL del bucket (opcional)
  }
}
