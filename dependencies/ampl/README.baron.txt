Solver baron is a global nonlinear solver based on the BARON solver by
Nikolaos V. Sahinidis and co-authors.  See

        http://archimedes.cheme.cmu.edu/?q=baron
        http://www.minlp.com/

For much more detail on BARON, including pointers to papers, see the
accompanying baron_manual.pdf.

It is most convenient to invoke baron with AMPL's solve command:

        ampl: option solver baron;
        ampl: solve;

but baron can also be run separately, with invocation syntax

        baron stub [-AMPL] [keywd=[value] ...]

in which stub comes from AMPL's write command.  For example,

        > ampl -obfoo foo.mod foo.dat
        > baron foo

demonstrates running baron separately; the ampl invocation writes file
foo.nl (stub = "foo"), which baron reads.

Command-line arguments to baron either have the form

        keywd
or
        keywd=value

where keywd is one of the keywords described below.  Alternatively,
you can invoke baron the way AMPL's solve command does, i.e.,

        baron stub -AMPL [keywd=value ...]

where stub was specified in

        ampl -obstub ...
or
        ampl -ogstub...

Such an invocation causes baron to read from stub.nl and to write stub.sol.

-----------------
Controlling baron
-----------------

Baron reads keywords and values from the environment (shell) variable
baron_options and from the command line.  Execute

        baron -?

or (if your shell requires ? to be quoted)

        baron '-?'

for a summary of baron usage and

        baron -=

(or baron '-=') for a summary of keywords peculiar to the AMPL/BARON
driver "baron".

------------------
Sample Invocations
------------------

  If you're using AMPL, just say

        option solver baron;
        solve;

  If you've executed, say,

        ampl -objunk junk.model junk.data

then you could say

        baron junk epsr=1e-4 lsolver=minos

to force baron to use relative tolerance .0001 between the function
value at the solution returned and a lower bound thereon and to use
local solver minos.  (This assumes a license for the AMPL/MINOS solver
"minos" is available.)  With the Bourne shell, either of the invocations

        baron_options='epsr=1e-4 lsolver=minos' baron junk
or
        baron_options='epsr=1e-4 lsolver=minos'
        export baron_options
        baron junk

would have the same effect; within AMPL, specifying

        option baron_options 'epsr=1e-4 lsolver=minos', solver baron;
        solve;

would also have this effect.


-----------------------
solve_result_num values
=======================

Here is a table of solve_result_num values that "baron" can return to
an AMPL session, along with the text that appears in the associated
solve_message.

        Value   Message

        0       optimal within tolerances
        100     numerical difficulties but possibly optimal
	150	feasible but insufficient progress
        200     infeasible
        201     infeasible, IIS found
        202     infeasible, IS found, possibly not irreducible
        203     infeasible, IIS sought but not found
        300     unbounded
        400     node limit reached
        401     iteration limit reached
        402     CPU time limit reached
        500     licensing error
        501     numerical difficulties
        502     interrupted (Control-C)
        503     too little memory
        504     terminated by BARON
        505     BARON syntax error (should not happen)
        506     operation not supported by BARON
        507     Interrupted by Control-C

----------
INSTALLING
==========

On Linux systems, libbaron*.so (where the values of "*" depends on the
current version of BARON) needs to appear in the current directory
when baron itself appears there, or in one of the standard places
(specified by /etc/ld.so.conf on some systems), or in a directory
named in $LD_LIBRARY_PATH.  An alternative is to add a short shell
script, such as

        #!/bin/sh
        LD_LIBRARY_PATH=/usr/local/lib
        export LD_LIBRARY_PATH
        exec /usr/local/bin/baronx "$@"

to a directory in your usual $PATH (and mark the script executable
with, e.g., "chmod +x baron").  The above script assumes that the
true "baron" binary has been moved to /usr/local/bin/baronx and that
the libbaron*.so file has been moved to /usr/local/lib.

MacOSX systems are similar to Linux systems, but with libbaron*.dylib
in place of libbaron*.so and with DYLD_LIBRARY_PATH in place of
LD_LIBRARY_PATH.  On MacOSX systems, it suffices for the
libbaron*.dylib file to appear in the same directory as baron, at
least when that directory is current when baron is invoked.

On MS Windows systems, xpress.exe and the relevant baron-*.dll must
appear somewhere in your usual search $PATH (or in the current
directory).

Questions about this stuff?  Contact support@ampl.com .


----------------------------------
keyword summary: "baron -=" output
==================================

barstats   Report detailed Baron statistics.  No is value expected.

deltaa     Absolute tolerance used when deltaterm=1 is specified.
           Default = Infinity.

deltar     Relative tolerance used when deltaterm=1 is specified.
           Default = 1.

deltat     Used to specifiy ndeltat, which is used when deltaterm=1 is
           specified.  If deltat > 0, then ndelta = deltat.  If deltat < 0,
           then ndeltat = -deltat times the CPU time for root processing.
           If deltat = 0, then deltaterm = 0 is assumed.  Default = -100.

deltaterm  Whether to check for "insufficient progress", which is
           detected if the objective has not improved by more than
           min(deltaa, deltar*abs(objective)) after ndelta seconds.
           See the descriptions of deltaa, deltar, deltat.

epsa       BARON's EpsA convergence tolerance (default 1e-6).  BARON stops if
           the current function value f satisfies abs(f - L) <= epsa,
           where L is the currently best available bound on f.

epsr       BARON's EpsR convergence tolerance (default 1e-6).  BARON stops if
           the current function value f satisfies abs(f - L) <= abs(L*epsr),
           where L is the currently best available bound on f.

filter     Allow BARON to use the FilterSD solver.  No value is expected.
           Deprecated:  use nlpsol=... instead.

iisfind    Whether to find and return an IIS (irreducible infeasible set of
           variables and constraints) if the problem is infeasible:
                0 = no (default)
                1 = yes, using a fast heuristic
                2 = yes, using a deletion filtering algorithm
                3 = yes, using an addition filtering algorithm
                4 = yes, using an addition-deletion filtering algorithm
                5 = yes, using a depth-first search algorithm.
           IIS details are returned in suffix .iis, which assumes one of the
           values "non" for variables and constraints not in the IIS; "low"
           for variables or inequality constraint bodies whose lower bounds
           are in the IIS; "upp" for variables and inequality constraint
           bodies whose upper bounds are in the IIS; and "fix" for equality
           constraints that are in the IIS.

iisint     Whether to include integer variables in an IIS (see iisfind):
                0 = no
                1 = yes (default).
           Binary variables are always excluded.

iisorder   How to order constraints when seeking an IIS (see iisfind):
                -1 = automatic choice
                 1 = problem order (as in .nl file)
                 2 = ascending order by degree
                 3 = descending order by degree
                >= 4 = random order with seed iis_order.

keepsol    Keep BARON's solution files.  No value is expected.

lpsolver   Choice of LP solver, which matters mainly when there are integer
           variables:  one of cbc (default), cplex, or xpress.  The last two
           must be suitably licensed to be used.

lsolmsg    Show solution messages for lsolver.  No value is expected.

lsolver    Local nonlinear solver that Baron should call.
           The local solver should have an AMPL interface and, if needed,
           its own license.  Default:  use a builtin local solver.

maxiter    Maximum number of branch-and-reduce iterations; -1 (the default)
           means no limit; 0 forces BARON to stop after root-node processing.

maxtime    Maximum CPU seconds allowed (default 1000); -1 means no limit.

nlpsol     Local nonlinear solvers BARON is allowed to use: sum (mod 16) of
                1 ==> IPOPT (builtin)
                2 ==> FilterSD (builtin)
                4 ==> FilterSQP (builtin)
                8 ==> lsolver (if lsolver=... is specified)
           Default 0 ==> allow all.

numsol     Number of near optimal solutions to find.
           Default = 1; values > 1 imply keepsol and cause suffix .numsol
           on the objective and problem to be returned.

objbound   Return suffixes .obj_lb and .obj_ub on the problem and objective
           with Baron's final lower and upper bounds on the objective value.
           No value is expected.

objno      objective number: 1 = first (default).

optfile    Name of BARON option file (not required).  If given, the file should
           contain name-value pairs, one per line, with the name and value
           separated by a blank, a colon, or an equal sign, possibly surrounded
           by white space.  The names and possible values are summarized in
           section 6 of the BARON user manual (baron_manual.pdf).  Empty lines
           and lines that start with # are ignored.

outlev     Whether to chatter: 0 ==> no (default), 1 ==> yes.

prfreq     Report progress every prfreq nodes (default 1e6).

prloc      Whether to report local searches: 0 ==> no (default), 1 = yes.

problem    Problem name printed in logfile.

prtime     Report progress every prtime seconds (default 30).

scratch    Directory for temporary files; will be removed unless keepsol
           is specified.

sumfile    Name of summary file; default = none (not written).

threads    Maximum threads to use (default 1) when there are integer variables.

trace      Name of Baron "trace" file; none if not specified.

version    Single-word phrase:  show the current version.

wantsol    solution report without -AMPL: sum of
                1 ==> write .sol file
                2 ==> print primal variable values
                4 ==> print dual variable values
                8 ==> do not print solution message
