#!/bin/bash
# 
# uninstall.sh: Script to uninstall mkcproj
# 
# Written by Gustavo Bacagine <gustavo.bacagine@protonmail.com>
# 
# Date: dd/mm/aaaa

# Checking if user is root
if [[ $EUID -ne 0 ]]; then
  # Checking if the terminal suport colored text
  if [[ test -n $(tput colors) && test $(tput colors) -ge 8]]; then
    printf "\033[1;31mE:\033[m This script must be run as root\n"
  fi
  
  print "E: This script must be run as root\n"

  exit 1
fi

if test -f /usr/bin/mkcproj; then
  printf "#########################\n"
  printf "# Uninstalling mkcproj #\n"
  printf "#########################\n"

  # Creating the documentation directory
  rm -rv /usr/share/doc/mkcproj

  # Install the binary of software and yout autocomplete script
  rm -rvf /usr/bin/mkcproj
  rm -rvf /usr/share/bash-completion/completions/_mkcproj_complete.sh 

  # Installing the configuration file of the software
  rm -rvf /etc/mkcproj.conf

  # Installing the documentation
  cp -rvf /usr/share/man/man1/mkcproj.1.gz
  cp -rvf /usr/share/doc/mkcproj

  printf "\nmkcproj was uninstalled successfuly!\n"
elif
  # Checking if the terminal suport colored text
  if [[ test -n $(tput colors) && test $(tput colors) -ge 8]]; then
    printf "\033[1;31mE:\033[m mkcproj is not installed!\n"
  elif
    printf "E: mkcproj is not installed!\n"
  fi

  printf "Nothing to do\n"

  exit 1
fi

