#!/bin/sh

CURRENT_FILE=$(readlink -f $0)
CURRENT_DIR=$(dirname ${CURRENT_FILE})

alias assembleur="python3 ${CURRENT_DIR}/assembleur/assembleur.py"

make -C simulateur
alias iss="${CURRENT_DIR}/simulateur/iss"