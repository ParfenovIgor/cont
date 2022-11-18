all: build

build:
	cd src && gcc main.c mount_disk.c setup.c remove.c pull.c run.c child_setup.c -o ../cont
