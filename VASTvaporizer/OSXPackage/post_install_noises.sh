#!/usr/bin/env bash
echo Post install VAST Dynamics Software Vaporizer2 plugin

mkdir -p ~/Documents/Vaporizer2/Noises/Factory
if [[ $DSTROOT == "" ]] 
then
    cp -frv /Applications/Vaporizer2/Noises/* ~/Documents/Vaporizer2/Noises/Factory
else
    cp -frv "$DSTROOT"/Noises/* ~/Documents/Vaporizer2/Noises/Factory
fi