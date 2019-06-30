# [Xflashcard](https://github.com/h5p9sl/xflashcard)

Xflashcard is a lightweight flashcard studying program.

Please refer to [example.txt](https://github.com/h5p9sl/xflashcard/blob/424d7addc11f0d47022731e9e7247aaae03bae8d/flashcards/example.txt) for an example of how to create flashcard files.

## Building
### Linux
Please make sure you have the following packages installed to your system:
`make gcc git`.
If you are unsure, run the following command to install them (Ubuntu):
`sudo apt-get install make gcc git`.
Then you can build the program using the following commands:
```sh
git clone https://github.com/h5p9sl/xflashcard
cd xflashcard
make clean all
```

### Windows
Windows support is mediocre. You may run into unforseen issues.

Please make sure you have the following software installed to your system:
+ [MinGW.org Compiler Collection (GCC)](https://osdn.net/projects/mingw/downloads/68260/mingw-get-setup.exe/)
+ [Git for Windows](https://github.com/git-for-windows/git/releases/latest)

Then you can build the program using the following commands:
```bat
git clone https://github.com/h5p9sl/xflashcard
cd xflashcard
mingw32-make clean all
```
