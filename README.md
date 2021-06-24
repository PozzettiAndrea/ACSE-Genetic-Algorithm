<!--
*** Thanks for checking out the Best-README-Template. If you have a suggestion
*** that would make this better, please fork the repo and create a pull request
*** or simply open an issue with the tag "enhancement".
*** Thanks again! Now go create something AMAZING! :D
-->



<!-- PROJECT SHIELDS -->
<!--
*** I'm using markdown "reference style" links for readability.
*** Reference links are enclosed in brackets [ ] instead of parentheses ( ).
*** See the bottom of this document for the declaration of the reference variables
*** for contributors-url, forks-url, etc. This is an optional, concise syntax you may use.
*** https://www.markdownguide.org/basic-syntax/#reference-style-links
-->



<!-- PROJECT LOGO -->
<br />
<p align="center">
  <a href="https://github.com/othneildrew/Best-README-Template">
    <img src="readme_source/logo_imperial_college_london.png" alt="Logo" width="270" height="100">
  </a>

  <h3 align="center">gormanium-rush-fluorite</h3>



<!-- TABLE OF CONTENTS -->
<details open="open">
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#about-the-project">About The Project</a>
      <ul>
        <li><a href="#built-with">Built With</a></li>
      </ul>
    </li>
    <li>
      <a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#prerequisites">Prerequisites</a></li>
        <li><a href="#installation">Installation</a></li>
      </ul>
    </li>
    <li><a href="#usage">Usage</a></li>
    <li><a href="#contributing">Contributing</a></li>
    <li><a href="#license">License</a></li>
    <li><a href="#contact">Contact</a></li>
    <li><a href="#acknowledgements">Acknowledgements</a></li>
  </ol>
</details>



<!-- ABOUT THE PROJECT -->
## About The Project

This program is to find a optimal circuit to get max performance by using genetic algorithm, and then generate a graph to show the result.
In this program, you can compile and run it cross platform (Ubuntu and Windows).

### Built With

* [Visual Studio Code](https://code.visualstudio.com/)
* [CMake](https://cmake.org/)
* [GNU make](https://www.gnu.org/software/make/)
* [mingw-w64](http://mingw-w64.org/doku.php)
* [python](https://www.python.org/)
* [Anaconda](https://www.anaconda.com/products/individual)


<!-- GETTING STARTED -->
## Getting Started

To compile and run our program, you should follow the instruction below.

### Prerequisites

You should install these tools before you compile this program

1. On Ubuntu
* pip
  ```sh
  sudo apt-get install pip
  ```

* cmake
  ```sh
  sudo apt-get install cmake
  ```

* make
  ```sh
  sudo apt-get install make
  ```

* cpplint
  ```sh
  pip install cpplint
  ```
  
* flake8
  ```sh
  pip install flake8
  ```
  
* Anaconda
  ```sh
  sudo wget https://repo.continuum.io/archive/Anaconda3-5.3.1-Linux-x86_64.sh
  chmod +x Anaconda3-5.3.1-Linux-x86_64.sh
  bash Anaconda3-5.3.1-Linux-x86_64.sh
  ```

2. On Windows
* pip
  ```sh
  you can find installation and usage information in this link: https://pypi.org/project/pip/#downloads
  ```
* CMake
  ```sh
  you can install CMake here: https://cmake.org/download/
  ```

* mingw-w64
  ```sh
  you can download mingw-w64 here: http://mingw-w64.org/doku.php/download
  ```

* cpplint
  ```sh
  pip install cpplint
  ```
  
* Anaconda
  ```sh
  you can install Anaconda here: https://www.anaconda.com/products/individual
  ```
  
* flake8
  ```sh
  pip install flake8
  ```


### Installation

1. git clone our repository:
   ```sh
   git clone git@github.com:acse-2020/acse-4-gormanium-rush-fluorite.git
   ```
3. Compile this program
   
   On windows:
   ```sh
   cmake -G   "MinGW Makefiles" .
   mingw32-make all
   ```

   On Ubuntu
   ```sh
   cmake .
   make
   ```
   after use CMake to generate the Makefile, you can just use command `make` every time you modify the code.
   "CMAKE ONCE, MAKE EVEVRYWHERE"


<!-- USAGE EXAMPLES -->
## Usage

The software will ask users to input how many times to run, any generations, crossover and mutation probability, whether users wants to print and write to txt or not by clear instructions guided on the terminal.
And it will return the best vector with fitness we found for you automatcially.

On Windows
```sh
Genetic_Algorithm.exe
```


On Ubuntu
```sh
./Genetic_Algorithm.exe
```

## visualization

The example python script requires a working python installation of Graphviz. For conda users a working environment can be 

```
conda env create -f environment.yml
conda activate acse-4-p3
```

or you can (conda) install the packages yourself
```
conda install graphviz python-graphviz
```
The output from previous algorithm should be written in vector.txt, whose content looks like this:
```
0 4 3 2 0 5 4 4 6 2 1
```

After those set-up things, you can run this to test from command line:
```
python3 drawCircuit.py inputFileName outputFileName
```

For example:
```
python3 drawCircuit.py vector.txt myOutput.png
```
You will have the visulization of the units in file `myOutput.png`. Cheers!🍺


## Test
### CODE TEST
After you use CMake and make to compile this project successfully, you can use this command below to test

and it will run on Windows or Ubuntu successfully

```sh
make runtests
```

### LINTING TEST
1. CPP files
```sh
cpplint --verbose=3 filename.cpp
```

2. Python files
```sh
flake8 filename.py
```

<!-- LICENSE -->
## License

Distributed under the MIT License. See `LICENSE` for more information.



<!-- CONTACT -->
## Contact

Pozzetti, Andrea - andrea.pozzetti20@imperial.ac.uk

Zhang, Zhijin - zhijin.zhang20@imperial.ac.uk

He, Tianxu - tianxu.he19@imperial.ac.uk

Wang, Yikang - yikang.wang20@imperial.ac.uk

Ge, Zeena - zihui.ge20@imperial.ac.uk

Lyu, Shiqi - shiqi.lyu20@imperial.ac.uk

Ilia, Aikaterini - aikaterini.ilia20@imperial.ac.uk

lin, yushen - yushen.lin20@imperial.ac.uk


Project Link: [acse-4-gormanium-rush-fluorite](https://github.com/acse-2020/acse-4-gormanium-rush-fluorite)



<!-- ACKNOWLEDGEMENTS -->
## Acknowledgements
* [Best-README-Template](https://github.com/othneildrew/Best-README-Template)
