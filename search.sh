#!/bin/bash
search_term="$1"
directory="${2:-.}"
exclude_patterns="${3:-remove|./tags}"

grep "$search_term" $(find "$directory" -type f) | grep -v -E "$exclude_patterns"
