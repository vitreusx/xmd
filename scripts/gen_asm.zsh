#!/usr/bin/env zsh
echo "Generating assembly files..."
for name in $(fd . -e o); do
  objdump -drlwC -Mintel "$name" >"${name}.s"
done