# Mighty Shell!

A not-so-serious attempt at writing a shell, for the sake of learning the ins and outs of UNIX job control.

I decided to write this shell in C because it lets me work directly with the system and library calls exposed by Linux.

Still very much a work in progress.

Also, I'm not shooting for complete POSIX compliance or anything like that. Keep reading for the planned specs.

## Specs
### Do want
- [x] implement a repl
- [x] read a simple command, launch a foreground job to completion
- [x] implement job control
- [ ] launch job to background (e.g. '\<cmd> \<args> &')
- [ ] run scripts (non-interactive shell)
- [ ] export variables to the environment (export builtin)
- [ ] pipes
- [ ] basic I/O redirection ('>', '>>', '<', but no here documents)
- [ ] advanced I/O fd duplication (e.g. '2>&1')

### Might want
- Better parsing. Honestly, I'd love to delve into proper lexing & parsing, but I might choose to do that in a separate project. I found a cool book on programming a LISP interpreter, so I might do that first and come back to this project armed with better knowledge.
- Variable expansion, as in "echo $PATH"
- Use GNU Readline library. This is the lib that Bash uses, and it's my understanding that it was basically developed alongside Bash. Using it should allow for a much happier command line experience
- History. Useful for obvious reasons.
- Tab completion. Also a very nice thing to have.

### Don't want
- While I mentioned better parsing above, I'm not sure I will ever try to make this thing fully standards compliant.

## Running the shell
So far, the shell only runs in interactive non-login mode. It is not standards-compliant, so do not try to set it as your login shell using `chsh`.

### Make
The included Makefile is not very portable, unfortunately. I mostly develop in Linux and macOS. I use `gcc`, so the makefile expects that. Of course, you could easily compile using `clang` or your compiler of choice. The following paragraphs only apply to the use of the included makefile.

If you are running macOS, I recommend installing [Homebrew gcc-12](https://formulae.brew.sh/formula/gcc). The makefile should pick up on the fact that you are running Darwin (macOS) and set `CC` to "gcc-12". You'll just need to make sure that command is on your `PATH`.

Running
```
$ make
```
will build a binary named `main`, which can then be run like so:
```
$ ./main
```

To clean up, run
```
$ make clean
```

If you'd like to build an optimized binary, set `BUILD_PROFILE` to "release", as in:
```
$ BUILD_PROFILE=release make
```

## Contributing

Since this is a learning project, I'm hacking on this myself for the time being, but I'm always open to suggestions or ideas.

aubrey.nicoll@gmail.com

## Resources
- [Write a Shell in C](https://brennan.io/2015/01/16/write-a-shell-in-c/) by Stephen Brennan
- [The Bourne-Again Shell](http://www.aosabook.org/en/bash.html) by Chet Ramey
- [The GNU C Library Reference Manual](https://www.gnu.org/software/libc/manual/html_node/index.html)
- Bryant, Randal, and David O'Hallaron. Computer Systems: A Programmer's Perspective.
- Kerrisk, Michael. The Linux Programming Interface.
- [The Linux Man Pages Project](https://man7.org/linux/man-pages/index.html) maintained by Michael Kerrisk
- Stevens, W. Richard, and Stephen Rago. Advanced Programming in the UNIX Environment.
