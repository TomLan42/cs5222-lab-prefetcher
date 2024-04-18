# Use Ubuntu as the base image
FROM ubuntu:latest

# Set the working directory
WORKDIR /root

# Update package lists and install build essentials and other necessary dependencies
RUN apt-get update && \
    apt-get install -y \
    build-essential \
    g++ \
    vim \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

# Expose a volume
VOLUME /cs5222-lab-prefetcher

# Start a bash shell by default
CMD ["/bin/bash"]
