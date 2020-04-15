# Coronapoker 

Highly efficient Texas Hold'em simulation framework, with AI created during the great COVID-19 epidemic, by JWang925 and Kai.


Step 1: Create your own class, derived from base class Player. 
        See RandomPlayer for example.

Step 2: Setup your problem, see main.cpp for an example.

Step 3: Compile and run ./xxx >log 2>&1 ; or ./xxx >/run/shm/log 2>&1 or ./xxx >/dev/null


Known limitation:
        Only headsup is functional. For multi-way to work, need to have better pot size raise computation and side pot functionality.
        Each hand must start with 100bb.