variable "bucket_name" {
  description = "The name of the S3 bucket"
  type        = string
  default     = "terraform-bucket-319283ujdaosd"
}

variable "region" {
  description = "AWS Region"
  type        = string
  default     = "us-west-2"
}