#!/bin/bash

#Installation Script
read -p "Install Fuzzy-Server? [yes/no] " installation
if [ $installation == yes ]
then
  clear
  echo "Installing..."
  echo "--------------------------------------------------------------------------------"
  sleep 1
  apt-get update
  sleep 1
  echo "--------------------------------------------------------------------------------"
  apt-get install figlet -y
  sleep 1
  echo "--------------------------------------------------------------------------------"
  apt-get install spike -y
  sleep 1
  echo "--------------------------------------------------------------------------------"
  echo "Adding The Finishing Touches..."
  sleep 1
  chmod 777 fuzzy-server
  echo "--------------------------------------------------------------------------------"
  echo "Done!"
else
  exit
fi
