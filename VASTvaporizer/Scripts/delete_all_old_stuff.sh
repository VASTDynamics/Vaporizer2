#!/bin/bash

sudo rm -R /Library/Audio/Plug-Ins/Components/VAST*
sudo rm -R /Library/Audio/Plug-Ins/VST/VAST*
sudo rm -R /Library/Audio/Plug-Ins/VST3/VAST*
sudo rm -R /Library/Audio/Plug-Ins/LV2/VAST*
sudo rm -R "/Library/Application Support/Avid/Audio/Plug-Ins/VAST*"

sudo chmod 777 /Library/Audio/Plug-Ins/VST
sudo chmod 777 /Library/Audio/Plug-Ins/VST3
sudo chmod 777 /Library/Audio/Plug-Ins/Components
sudo chmod 777 /Library/Audio/LV2/Components
sudo chmod 777 "/Library/Application Support/Avid/Audio/Plug-Ins"
