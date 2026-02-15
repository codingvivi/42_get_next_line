build *args:
    premake5 gmake
    make {{args}}
    rsync -av --include-from='.turnin-include' --exclude='*' . turnin/
    mv -f turnin/README.md turnin/src/README.md
