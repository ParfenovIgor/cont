# Cont

## Author

Igor Parfenov

Contact: [@Igor_Parfenov](https://t.me/Igor_Parfenov)

## Overview

This is tool for containarization on Linux OS. It supports pulling images for [Hub Docker](hub.docker.com)

Tool was built on Ubuntu 22.

Execute `cont` to get usage information.

## Usage

Usage: cont [command]

Commands:
* `list` List all local containers
* `setup <name> <size>` Create disk file for mounting with given name and size in Mb
* `remove <name>` Remove disk file with given name
* `pull <name> <image>` Pull given docker image into given disk file name
* `run <name> <command>` Run given command in local container with given name

## Example

Example for setting `alpine` OS:

```
sudo ./cont setup alpine 100
sudo ./cont pull alpine alpine
sudo ./cont run alpine /bin/ash
```

To setup DNS, you can enter `echo "nameserver 8.8.8.8" > /etc/resolv.conf`

## Build

Enter `make` to build the tool.
