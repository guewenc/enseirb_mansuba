# Mansuba - Team 17115 (n°9)

First year project in engineering school in the Computer Science department of ENSEIRB-MATMECA. Project link : <https://www.labri.fr/perso/renault/working/teaching/projets/2022-23-S5-Mansuba.php>

## Getting Started

These instructions will allow you to run the project on your local machine for development and testing purposes.

## Installing and run

```bash
make
./project
```

## Run tests

The tests related to the project are present in the `tst` folder.

```bash
make test
```

## Usage

5 options are possible at runtime:

- an optional parameter `-s` to initialise the random generator.
- an optional `-t` parameter to set the type of win (`s` declares a simple win, `c` a complex win).
- an optional `-m` parameter to set the maximum number of rounds.
- an optional `-c` parameter to get the configuration of a special game. When this parameter is used, none of the others specified above are usable.
- an optional `-h` or `-u` parameter to get usage. Only print usage then program stop.

Example of use:

```bash
./project -s 156 -m 10 -t c
./project -t c
./project -c 1
./project -u
```

## Documentation

A Doxygen configuration file is present at the root of the project. Link to the Doxygen project: <https://github.com/doxygen/doxygen>.
Prerequisite `bison` package `>= 2.7`. Change the `DOT_PATH` variable in the `dgenerate` file to the location of your `dot` package. You can find it with the `which dot` command.

To compile the documentation, follow these instructions:

```bash
make clean doxygen
```

All generated documentation is present in the `doc` folder at the root of the project.

## Contributing

- Guewen COUSSEAU (<gcousseau@ipb.fr>)
- Samuel LANDEAU (<slandeau@ipb.fr>)
