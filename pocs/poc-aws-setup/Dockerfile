FROM hashicorp/terraform:latest

WORKDIR /workspace

COPY terraform/* /workspace/

entrypoint ["terraform", "apply"]
# CMD ["terraform", "-version"]