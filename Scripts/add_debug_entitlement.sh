#! /bin/bash
# Simple Utility Script for allowing debug of hardened macOS apps.
# This is useful mostly for plug-in developer that would like keep developing without turning SIP off.
# Credit for idea goes to (McMartin): https://forum.juce.com/t/apple-gatekeeper-notarised-distributables/29952/57?u=ttg
app_path=$1

if [ -z "$app_path" ];
then
    echo "You need to specify app to re-codesign!"
    exit 0
fi

# make sure you use a proper Developer ID. 
# you'd might need to make this not ambigious if multiple ids exists.
dev_id="Developer ID Application"
entitlements_plist=/tmp/debug_entitlements.plist
echo "Grabbing entitlements from app..."
codesign -d --entitlements :- "$app_path" >> $entitlements_plist || { exit 1; }
echo "Patch entitlements (if missing)..."
/usr/libexec/PlistBuddy -c "Add :com.apple.security.cs.disable-library-validation bool true" $entitlements_plist
/usr/libexec/PlistBuddy -c "Add :com.apple.security.cs.allow-unsigned-executable-memory bool true" $entitlements_plist
/usr/libexec/PlistBuddy -c "Add :com.apple.security.get-task-allow bool true" $entitlements_plist
echo "Re-applying entitlements (if missing)..."
codesign --force --options runtime --sign - --entitlements $entitlements_plist "$app_path" || { echo "codesign failed!"; }
echo "Removing temporary plist..."
rm $entitlements_plist
