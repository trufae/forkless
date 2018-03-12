forkless
========

This library aims to provide an alternative to fork() for
systems that don't support fork (windows and ios)

I'm targeting iOS only because windows requires much more
abstractions to get this working, and there's already cygwin
who does that.

On iOS, you can't execve or fork for sandbox reasons, and
having the ability to have a full unix environment (pipes,
processes, ...) is desireable on that environment.

Tested on x86-64 only.

--pancake
