#! /bin/bash

read -n1 -p "Do you want to build server (y/n)?"
if [ $REPLY = 'y' ]
then
	if (make -C ../Server)
	then
		echo "Server build OK"
		mv ./../Server/server ./
	else
		echo "Server build failed"
		read
	fi
fi

read -n1 -p "Do you want to build Client (y/n)?"
if [ $REPLY = 'y' ]
then
	if (make -C ../Clients)
	then
		echo "Client build OK"
		mv ./../Clients/rc1 ./
	else
		echo "Client build failed"
		read
	fi
fi


read -n1 -p "Do you want to build Vendor (y/n)?"
if [ $REPLY = 'y' ]
then
	if (make -C ../Vendors)
	then
		echo "Vendor build OK"
		mv ./../Vendors/vn1 ./
	else
		echo "Vendor build failed"
		read
	fi
fi

read -n1 -p "Do you want to run APP (y/n)?"
if [ $REPLY = 'y' ]
then
	./server
fi


