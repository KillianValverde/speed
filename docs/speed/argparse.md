# `argparse` Module

## Overview

The `argparse` module  in the `speed` library provides utilities to write user-friendly 
command-line interfaces. The program defines what arguments it requires, and argparse will figure 
out how to parse those out of argc and argv. The argparse module also automatically generates help, 
usage messages and issues errors when users give the program invalid arguments.

## Table of Contents

- [Quick Start](#quick-start)
  - [Simple arguments](#simple-arguments)
  - [Arguments with values](#arguments-with-values)
  - [Arguments without keys](#arguments-without-keys)
  - [Help arguments](#help-arguments)
  - [Version arguments](#version-arguments)
  - [Arguments constraints](#arguments-constraints)
  - [Help menus](#help-menus)
  - [Arguments parsing](#arguments-parsing)
- [Argument parser configuration (work in progress)](#argument-parser-configuration)
- [Acknowledgement](#acknowledgement)

## Quick Start

This section aims to present a superficial overview of how to use `argparse`. The next 
sections will delve deeper into each aspect. 

To start using argparse in your project, simply include the Speed library header.

```cpp
#include <speed/speed.hpp>
```

Now create an `arg_parser` object. The argument parser takes the name of your project as a 
parameter; if you don't specify any, it will try to get it from parsing argv during its execution.

```cpp
speed::argparse::arg_parser ap("project_name");
```

Now it is time to add arguments and information to the parser.

### Simple arguments

The argument parser provides different methods to add arguments depending on what you want to do.
To add a simple argument that you will be able to check its program call presence, simply add a 
`key_arg`.

```cpp
ap.add_key_arg("--yes", "-y")
        .description("Assume yes as the answer to all prompts.")
        .store_presence(&skip);
```

You can add as many keys as you wish; each one of them will be assigned to the added argument and 
could be used by the user to set the flag or by the programmer to reference the argument within 
the parser. 

Additionally, each key specified in the example above has a different prefix. The 
string that the argument parser will consider as a prefix can be modified at any moment, and we 
will cover that later. For now, it is important to know that by default `--` is considered as the 
long prefix and `-` is the short prefix.

Along with that, when adding any kind of argument, a special argument setter object is returned by 
the function. This object can be used to add additional properties to the argument, like for 
example a help description.

### Arguments with values

If you want to add an argument that will have values associated, you will need to use a 
`key_value_arg`. This object has all the features of the `key_arg` but additionally has others 
that allow it to get and parse associated values.

```cpp
ap.add_key_value_arg("--start-number", "-n")
        .description("Start number used.")
        .values_names("INTEGER")
        .store_into(&start_nr);
```

The `key_value_arg` knows which type to use for validity checks based on the value used to store 
the result of the parsing. If no value is specified, there will not be any type verification.

Additionally, the `key_value_arg` will, by default, have the minimum and maximum number of 
required values set to one. You can change this property by using the 
`minmax_values(size_t, size_t)` method.

Another important method to mention is `mandatory(bool)`. Setting this property will allow you 
to specify whether you want the argument to always be present during a program call.

Let's check another, slightly more complex example of adding a `key_value_arg`.

```cpp
ap.add_key_value_arg("--names", "-nm")
        .description("Names to set.")
        .values_names("STRING")
        .minmax_values(1, ~0ull)
        .store_into(&nms)
        .mandatory(true);
```

In this example, we are adding a mandatory `key_value_arg` that can take from one to 
`max_ull` values, and we are storing the results into a vector of strings called `nms`.

### Arguments without keys

The `keyless_arg` can be used to specify an argument without any key and with an associated value. 
This can be very useful, for example, for specifying an output file or an input directory.

```cpp
ap.add_keyless_arg("DIR")
        .description("Directory in which perform the operation.")
        .store_into(&dir);
```

The `speed` library also defines a set of paths classes that inherit from `std::filesystem::path` 
to allow the specification of the permissions associated with the kind of file that the program 
expects.

As an example, feel free to use a `speed::filesystem::rx_directory_path` to create a path object 
that the parser will check if the specified path is a readable, executable directory. The `speed` 
library defines a type for each one of the permissions combinations for regular files and 
directories. There is another particular kind of path called 
`speed::filesystem::output_regular_file_path` that allows checking if the file exists with write 
permissions, and if the file doesn't exist, if it can be created.

### Help arguments

The `help_arg` is used to print help information. It works exactly like a `key_value_arg` but 
with additional features allowing it to be associated with a `help_menu`. If the `help_arg` is 
not explicitly associated with a `help_menu`, it will be associated with the default `help_menu`, 
which has an empty string as its ID.

A `help_arg` object, by default, has a minimum and maximum expected value count of zero. The 
reason it can have values will be explained outside the quick start section, and it is related to 
the association between the `help_arg` and the additional `help_menu` objects.

```cpp     
ap.add_help_arg("--help", "-h")
        .description("Display this help and exit.");
```

### Version arguments

To print the version information of your program, you can use a `version_arg`. This simple object 
inherits from the `key_arg` and has an associated string that it will print if found during the 
program call.

```cpp
ap.add_version_arg("-v", "--version")
        .version_information("v1.0.0");
```

In order to be aligned with the GPLv3 licence, there is a method that facilitates the writting 
of starderized version information.

```cpp
ap.add_version_arg("-v", "--version")
        .gplv3_version_information("v1.0.0", "2024", "John Doe");
```

### Arguments constraints

The `argument_constraint` is the base of a set of classes that allow specifying specific 
dependencies between arguments. Currently, there are just two kinds of constraints, the 
`at_least_one_found` and the `mutually_exclusive`.

```cpp
ap.add_key_arg("-c")
        .description("Compile in C language.");

ap.add_key_arg("-cpp")
        .description("Compile in C++ language.");

ap.add_key_arg("-py")
        .description("Interpret in Python language.");

ap.add_constraint_one_or_more("-c", "-cpp", "-py");
ap.add_constraint_mutually_exclusive("-c", "-cpp", "-py");
```

### Help menus

The `help_menu` is an object that will store a reference to arguments in order to print the help 
information. If no `help_menu` is added to the parser, a default one with an empty ID will be used. 
The `help_menu` objects are created on demand, and arguments can reference them even if they haven't
been explicitly added to the parser.

Generally, a help_menu object will be created explicitly to add a description and an epilogue for 
the help information printing. If, during the creation of the `help_menu`, you don't give it any 
ID, you will be creating and configuring the default `help_menu`.

```cpp
ap.add_help_menu()
        .description("Count keyboard hits.")
        .epilogue("Example: $ count_kbhit -n 0 -s 2 -p 0.073");
```

You can create as many `help_menu` objects as you want, but you will need to explicitly associate 
each argument to the new `help_menu` objects. Otherwise, the argument will be associated with the 
default `help_menu`.

### Arguments parsing

Once all the configuration has been done, the last thing to do will be parsing the arguments.

```cpp
ap.parse_args(argc, argv);
```

During this process, arguments will have their values set, type validity checks will be performed, 
and if errors occur, they will be displayed and the process terminated.

Furthermore, help and version information will be displayed if `help_arg` or `version_arg` objects 
are found.

On the other hand, if you didn't specify a presence holder, a value holder, or a vector holder for 
the argument values, you will be able to use the `arg_parser` get methods to retrieve and convert 
the desired values.

```cpp
bool all = ap.was_found("-a");
auto secs = ap.get_front_as<std::uint64_t>("-s", 0);
```

## Argument parser configuration

(work in progress)

## Acknowledgement

The implementation of this argument parser has been influenced by many projects and people.

I want to thank the programmers who worked on the Python module
<a href="https://docs.python.org/3/library/argparse.html">argparse</a>, as discovering and using 
this library in 2015 inspired me to start this project.

I also want to specially thank <a href="https://github.com/p-ranav">p-ranav</a> for their work on 
their argument parser, as discovering their project gave me many ideas and insights into how to 
make a good argument parser.