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

To achieve this goal with my teammate [github.com/BenjaminSuger](https://github.com/BenjaminSuger/) 
## 🖐️ How to use ? 
Git clone the repo, make and you're done !\
Some usage examples are shown above, and feel free to try every command you want.. but these commands must have an executable that we can find on the $PATH (you can try `whereis cmd` to see if the command is valid for pipex).  
