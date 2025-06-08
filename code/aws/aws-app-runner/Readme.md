docker build -t aws-app-runner .

aws ecr get-login-password --region eu-west-2 --profile dev | docker login --username AWS --password-stdin 307634926156.dkr.ecr.eu-west-2.amazonaws.com

docker build --file Dockerfile.dapp -t ceit/dapp .

docker tag aws-app-runner 307634926156.dkr.ecr.eu-west-2.amazonaws.com/ceit/aws-app-runner

docker push 307634926156.dkr.ecr.eu-west-2.amazonaws.com/ceit/aws-app-runner