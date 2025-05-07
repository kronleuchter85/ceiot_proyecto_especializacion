provider "aws" {
  region = "eu-west-2"
}


#
# S3 
#
variable "s3_bucket_name" {
  default = "ceiot-exploratory-robot"
}
variable "s3_data_prefix" {
  default = "readings"  
}

resource "random_id" "bucket_id" {
  byte_length = 4
}
# resource "aws_s3_bucket" "iot_data_bucket" {
#   bucket = "iot-data-bucket-${random_id.bucket_id.hex}"
# }


#
# Glue
#
resource "aws_glue_catalog_database" "glue_database" {
  name = "ceit_landing"
}

resource "aws_glue_catalog_table" "landing_readings_table" {
  name          = "readings"
  database_name = aws_glue_catalog_database.glue_database.name

  table_type    = "EXTERNAL_TABLE"

  parameters = {
    "classification" = "json"
    "EXTERNAL"       = "TRUE"
  }

  storage_descriptor {
    location      = "s3://${var.s3_bucket_name}/${var.s3_data_prefix}"
    input_format  = "org.apache.hadoop.mapred.TextInputFormat"
    output_format = "org.apache.hadoop.hive.ql.io.HiveIgnoreKeyTextOutputFormat"

    ser_de_info {
      name                  = "JsonSerDe"
      serialization_library = "org.apache.hive.hcatalog.data.JsonSerDe"
    }

    columns {
      name = "deviceId"
      type = "string"
    }

    columns {
      name = "type"
      type = "string"
    }

    columns {
      name = "value"
      type = "string"
    }

    columns {
      name = "location"
      type = "string"
    }

    columns {
      name = "time"
      type = "string"
    }
  }
}


#
# Secret
#

variable "wallet_private_key" {
  description = "Private key de la wallet blockchain"
  type        = string
  sensitive   = true
}

resource "aws_secretsmanager_secret" "wallet_secret" {
  name        = "wallet-private-key"
  description = "Private key de la wallet blockchain"
}

resource "aws_secretsmanager_secret_version" "wallet_secret_version" {
  secret_id     = aws_secretsmanager_secret.wallet_secret.id
  secret_string = jsonencode({
    private_key = var.wallet_private_key
  })

}