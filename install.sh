#!/bin/bash
# 
# install.sh: Script to install mkcproj
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

printf "\n#######################\n"
printf "# Installing mkcproj #\n"
printf "#######################\n"

# Creating the documentation directory
mkdir -pv /usr/share/doc/mkcproj

# Install the binary of software and yout autocomplete script
cp -rvf ./bin/mkcproj /usr/bin
cp -rvf ./_mkcproj_complete.sh /usr/share/bash-completion/completions

# Installing the configuration file of the software
cp -rvf ./mkcproj.conf /etc

# Installing the documentation
cp -rvf ./man/mkcproj.1.gz /usr/share/man/man1
cp -rvf ./doc/* /usr/share/doc/mkcproj
cp -rvf ./LICENSE.gz /usr/share/doc/mkcproj
cp -rvf ./AUTHORS.gz /usr/share/doc/mkcproj
cp -rvf ./NEWS.gz /usr/share/doc/mkcproj
cp -rvf ./README.gz /usr/share/doc/mkcproj

printf "mkcproj was installed successfuly!"

