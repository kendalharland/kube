#!/bin/bash

pushd "$1"

for f in *.fbx; do
  [ -f "$f" ] || continue
  base="${f%.*}"
  assimp export "$f" "${base}.obj"
done

popd
