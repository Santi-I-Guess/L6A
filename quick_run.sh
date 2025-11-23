#!/usr/bin/env bash
if [[ $1 == "a" ]]; then
    printf -- "-1\n1\n" | ./L6A
elif [[ $1 == "l" ]]; then
    printf -- "-1\n2\n" | ./L6A
fi
