#!/usr/bin/env bash
echo Post install VAST Dynamics Software Vaporizer2 plugin

mkdir -p ~/Documents/Vaporizer2/Tables/Factory

if [[ $DSTROOT == "" ]] 
then
    cp -frv /Applications/Vaporizer2/Tables/* ~/Documents/Vaporizer2/Tables/Factory
else
    cp -frv "$DSTROOT"/Tables/* ~/Documents/Vaporizer2/Tables/Factory
fi