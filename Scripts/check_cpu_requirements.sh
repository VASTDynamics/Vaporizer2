#!/bin/bash

CPUReq=$(sysctl machdep.cpu)
#echo $CPUReq

CPUReqNeon=$(sysctl -a | grep "hw.optional.neon")
#echo $CPUReq

AVXSUPPORT=false
SSE2SUPPORT=false
NEONSUPPORT=false
if grep -q -I AVX <<< $CPUReq; then
  echo "AVX supported"
  AVXSUPPORT=true
else
 echo "AVX not supported"
fi

if grep -q -I SSE2 <<< $CPUReq; then
  echo "SSE2 supported"
  SSE2SUPPORT=true
else
  echo "SSE2 not supported"
fi

if grep -q -I "1" <<< $CPUReqNeon; then
  echo "Neon supported"
  NEONSUPPORT=true
else
  echo "Neon not yet supported"
fi

if (([ "$AVXSUPPORT" == "false" ])  && ([ "$SSE2SUPPORT" == "true" ])); then

Result=$(osascript<<-EOF

tell application id "com.apple.systemevents"
   set myMsg to "The installer has detected that your system does not support AVX or only NEON vector extensions (that are not yet natively supported) but SSE2 support is available. " & return & "Please install the SSE2 compatibility versions from the custom installation options and remove the others." & return & "Do you want to continue as selected anyway?"
   set theResponse to display dialog myMsg buttons {"Stop", "Continue"} default button 2 
end tell

if button returned of theResponse is "Stop" then
   return "Stop"
else 
   return "Continue"
end if

EOF)

if ([ "$Result" == "Stop" ]); then
  echo "Stopping"
  exit 1
fi

fi


if (([ "$AVXSUPPORT" == "false" ]) && ([ "$SSE2SUPPORT" == "false" ])); then

Result=$(osascript<<-EOF

tell application id "com.apple.systemevents"
   set myMsg to "The installer has detected that your system neither supports AVX nor NEON nor SSE2 vector extensions. " & return & "The plugin will unfortunately not run on your computer." & return & "Do you want to continue as selected anyway?"
   set theResponse to display dialog myMsg buttons {"Stop", "Continue"} default button 2 
end tell

if button returned of theResponse is "Stop" then
   return "Stop"
else 
   return "Continue"
end if

EOF)

if ([ "$Result" == "Stop" ]); then
  echo "Stopping"
  exit 1
fi

fi

exit 0

