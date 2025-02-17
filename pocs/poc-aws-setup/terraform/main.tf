provider "aws" {
  region = "us-west-2"
}

resource "aws_s3_bucket" "iot_data_bucket" {
  bucket = "iot-data-bucket-1490i109id0adaa"
}