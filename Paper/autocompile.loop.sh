#!/bin/bash

PDF_FILE="$1"
COMPILE_TARGET="$2"
BIBCOMPILE_TARGET="$3"

ECHECK=$(ps aux | grep "evince Out/${PDF_FILE}" | awk '{print $2}')
if [ "$ECHECK" ]; then
	EPID="$ECHECK"
else
	evince Out/${PDF_FILE} &
	EPID="$!"
fi

OLD=$(find . -name "*.tex" -exec cat '{}' 2>/dev/null \; | md5sum | grep -o "^[^ ]*")
OLDCITE=$(find . -name "*.tex" -exec cat '{}' 2>/dev/null \; | grep -o "\\\cite{[^}]*}" | md5sum | grep -o "^[^ ]*")
while true; do
	ERUN=`ps aux | awk '{print $2}' | grep "^$EPID$"`
	if [ ! "$ERUN" ]; then
	evince Out/${PDF_FILE} &
	EPID="$!"
	fi
	CUR=$(find . -name "*.tex" -exec cat '{}' 2>/dev/null \; | md5sum | grep -o "^[^ ]*")
	CITE=$(find . -name "*.tex" -exec cat '{}' 2>/dev/null \; | grep -o "\\\cite{[^}]*}" | md5sum | grep -o "^[^ ]*")
	if [[ "$CUR" != "$OLD" ]]; then
	if [[ "${CITE}" == "${OLDCITE}" ]]; then
		echo "Starting target COMPILE"
		make ${COMPILE_TARGET}
		echo "Target COMPILE completed"
	else
		echo "Starting target BIBCOMPILE"
		make ${BIBCOMPILE_TARGET}
		echo "Target BIBCOMPILE completed"
	fi
	OLD=$CUR
	OLDCITE=${CITE}
	date
	fi
	sleep 2s
done
