<h1 align="center">
	Minishell
</h1>
<p align="center">
	Reproduce some core features of a shell
</p>

## 📊 How it works ?
The project aims to reproduce a bunch of shell functionalities, so you need to be well informed of what is a shell and how to use one.  
The subject ask us to :
Wait for user input with readline, handle the command input and launch the corresponding executable based on the PATH environnement variable.
Chain commands with pipes.  
Handle environement variables (`$...`) which should expand correclty to their values and `$?`.  
Handle ctrl-C, ctrl-D and ctrl-\ like bash does.  
Implement the basics redirections `<`, `>` and also `<<` (heredoc) and `>>`.  
Handles quotes and double quotes but no other special characters as `;` or `\`.  
And finally reproduce some built-ins : echo, cd, pwd, export, unset, env and exit.  
  
To achieve this goal with my teammate [github.com/BenjaminSuger](https://github.com/BenjaminSuger/) we need to process the user's input to transform it into a datastructure (it is the "parsing" part, that I did) then analyse this datastructure and execute the command (the "exec" part that my teammate did).  
  
MY PART - LEXING/PARSING/EXPAND :  
First, come the lexing (or tokenizing) part. We need to scan the readline input and divide the line into what we call tokens. So each word or symbol is divided into his own token, for example in our project we tokenized into words, infile, here doc, outfile, append, pipe and end. These tokens are contained in a linked list that is given to the parser.  
The parser will then analyse the tokens and verify that they're correct. After that (or in our case in the same time) the parser will transform these tokens into the data structure we chose. The best option for a shell datastruct like this is to do an AST (more details at the bottom) but because we knew we didn't want to do the bonuses, we chose to do a simple linked list of commands.  
And finally in the expand phase we go through the command linked list we just did and dequote all args that will be given to execve later and also expand all environnement variable (so everything that starts with a dollarsign and that is present in the env) as well as $? that will expand into the exit code of the last command executed.  
  
HIS PART - ENV/EXEC/BUILTINS :  
You take the idea from the previous project Pipex, which are fork, pipe, file descriptor management.....  
With the goal to follow as much as possible the bash behaviour we have to follow certain steps:   
*managing the here document  
*go through each commands, we are going to check if redirections are valids and execute the commands if it's right.  
The main challenge of execution is to manage file descriptors where some informations are stored. The problem arise when you fork you inherit those fd and had to close them everywhere.  
We have several cases of redirections on each commands; the input can be from a previous command with pipe, from an input file with the < symbol or from heredocument with <<.
The output can be in the terminal, append to a file or truncate to a file.  
We also set up with a global variable, which is the only one allow in the subject, the exit code. Each command produce an exit code that you can check with "echo $?" in your terminal. This exit code can be different in various situations.  
The signals we manage are CTRL+D, CTRL+C and CTRL+\ (informations from the subject). There is a little catch, CTRL+D is not a signal but sent only EOF (End Of Files).  
CTRL+\ we have only to make sure it does not crash our minishell.  
This version on branch main is the one we submitted for evalutation and we got 101% ! Yoohoo... but the project is not perfect.. hum hum sigpipe in heredocs hum hum.  
  
## 🖐️ How to use ? 
Git clone the repo, make and you're done !\
Some usage examples are shown above, and feel free to try every command you want.. but these commands must have an executable that we can find on the $PATH (you can try `whereis cmd` to see if the command is valid for pipex).  
