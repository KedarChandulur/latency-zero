# latency-zero
Low-latency HFT simulator in C/C++

### Started with the Network folder:
- Done with the UDP so far.
- Currently working on the TCP.

## Setup:
- To setup basic dependencies, you need to run bootstrap script. This will install all the necessary dependencies for the project.
- Before executing the bootstrap.sh script, make sure it has execution permissions. Use chmod to achieve that.

```bash
chmod +x scripts/bootstrap.sh
bash scripts/bootstrap.sh
bash scripts/foldersetup.sh
```

## Build:
- To build the project, you can use make command. This will compile all the necessary files and create an executable.
- Make should be called at the root of the project directory

```bash
make
```

- To clean the generated files use the below command

```bash
make clean
```

## Run:
- To run the binaries/executables use the below command

```bash
# Inside the terminal
./bin/name_of_executable

# Replace name_of_executable with the name of your executable. For example, if you have a binary named `udp_receiver`, you can run it using:
./bin/udp_receiver

# Same applies to udp_sender
./bin/udp_sender
```

#### For Additional Notes:
Please Refer to [GIT_NOTES.md](https://raw.githubusercontent.com/KedarChandulur/latency-zero/refs/heads/Master/GIT_NOTES.md)
