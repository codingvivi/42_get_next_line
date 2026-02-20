build-release:
    rm -rfv dist/
    just build-dist
    @printf "\n"
    tar -czf lrain-get_next_line.tar.gz -C dist/ . 

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

fclean:
    rm -rfv obj/
    rm -rfv bin/
    rm -rfv dist/
    rm -fv Makefile
    rm -fv *.make
    rm -fv compile_commands.json

re:
    just fclean
    just build-make
    make
    just build-dist
