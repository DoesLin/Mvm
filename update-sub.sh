#!/bin/bash

endScriptError() {
    kdialog --title "Error" --error "An error occurred. See the console for details"
    qdbus $dbusRef close 2> /dev/null > /dev/null
    exit 1
}

if [[ -z "$(which kdialog)" ]]; then
    echo "kdialog not found."
    exit 1
fi

if [[ -z "$(which wget)" ]]; then
    echo "wget not found."
    exit 1
fi

dbusRef=$(kdialog --geometry 600x100 --title "Updating project's files..." --progressbar "Creating a backup...                                                                                       " 4)
qdbus $dbusRef setLabelText "Creating a backup..." 2> /dev/null > /dev/null
dir="../$(basename $(pwd))-$(date +%F#%T|tr ':' '-')"

if [[ -d "$dir" ]]; then
    echo "Can not create backup ! $dir already exists."
    endScriptError
fi

echo "Backuping to $dir..."
mkdir "$dir"
cp -r . "$dir/" || (echo "Can not create backup !"; endScriptError)
rmdir "$dir" 2>/dev/null >/dev/null

arch=$(uname -m)
rm -rf update
mkdir -p update
qdbus $dbusRef setLabelText "Downloading files..." 2> /dev/null > /dev/null

qdbus $dbusRef Set "" value 1 2> /dev/null > /dev/null
wget -q https://secure.bocqfamily.fr/enseignement/c/base-$arch.tar.gz -O update/base.tar.gz || endScriptError
qdbus $dbusRef Set "" value 2 2> /dev/null > /dev/null
wget -q https://secure.bocqfamily.fr/enseignement/c/doc.tar.gz -O update/doc.tar.gz 2>&1 || endScriptError
qdbus $dbusRef Set "" value 3 2> /dev/null > /dev/null
wget -q https://secure.bocqfamily.fr/enseignement/c/user-$arch.tar.gz -O update/user.tar.gz || endScriptError

qdbus $dbusRef setLabelText "Extracting files..." 2> /dev/null > /dev/null
qdbus $dbusRef Set "" value 4 2> /dev/null > /dev/null
tar -xzf update/base.tar.gz || endScriptError
tar -xzf update/doc.tar.gz || endScriptError
tar -xzkf update/user.tar.gz 2> /dev/null

rm -rf update
qdbus $dbusRef close 2> /dev/null > /dev/null
exit 0
