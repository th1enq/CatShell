# Unix/Linux Shell Project

## Project 1: - UNIX Shell

(Based on Chapter 3: Operating System Concepts, Silberschatz et al., 2018)

## Introduction

This project is a simple Unix/Linux shell implementation in C. It supports executing commands, handling input/output redirection, and background process execution.

## Features

- Execute built-in and external commands
- Input and output redirection (`>`, `>>`, `<`)
- Support for piping (`|`)
- Background execution (`&`)
- Error handling for invalid commands

## Installation

### Prerequisites

- GCC compiler
- Make utility
- Unix/Linux-based operating system

### Steps

1. Clone the repository:
   
   ```sh
   git clone https://github.com/th1enq/CatShell.git
   cd CatShell
   ```

2. Compile the project using Makefile:
   
   ```sh
   make run
   ```

3. ## Usage

Once the shell starts, you can execute commands just like in a standard Unix shell. Examples:

- Running a command:
  
  ```sh
  ls -l
  ```

- Redirecting output:
  
  ```sh
  ls > output.txt
  ```

- Running a background process:
  
  ```sh
  sleep 10 &
  ```

- Using pipes:
  
  ```sh
  ls | grep .c
  ```


