terraform {
  backend "s3" {
    bucket         = "ceiot-iac-terraform" 
    key            = "terraform.tfstate"  
    region         = "eu-west-2" 
    encrypt        = true  
    acl            = "private"  
  }
}
