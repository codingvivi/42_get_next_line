release:
    just build-dist
    @printf "\n"
    gh release create --fail-on-no-commits

build-dist:
    @printf "Copying files to dist/"
    rsync -av --mkpath --include-from='.dist-include' --exclude='*' . dist/
    rsync -av --mkpath README.md dist/src/README.md
    @printf "\n"
    ln -sfrv dist/src/README.md dist/README.md

build-make:
    premake5 gmake
    @printf "\n"
    premake5 ecc
