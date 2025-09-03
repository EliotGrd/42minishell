<h1 align="center">
	Minishell
</h1>
<p align="center">
	A prelude to minishell, reproduct pipes and basic exec
</p>

## 📊 How it works ?
The objective of the exercise is to reproduce how pipes in a shell works.  
I often use bash as an example on how things in pipex (or later in minishell) should work, as it is a bit less complexe than zsh.
To achieve this project you'll need to understand what a pipe is and how to correcty use execve() to execute commands.
You need to take some time to understand the functions execve(), pipe(), fork(), dup2() and access() before anything else so you have a better idea of where to start and what to use where.  
After a parsing phase that will fill the structures correctly, the exec loop can be run to execute command by command the program, each command getting as input the output of the previous one.  
The program when run with `./pipex infile "cmd0" "cmd1" outfile` should behave like `< infile cmd0 | cmd1 > outfile`.
  
With the bonus, you also need to be able to handle multiple pipes so that `./pipex infile cmd cmd1 cmd2 cmd3 cmd4 cmd5 ... outfile` works.
You also need to implement a sort of heredoc mode where instead of infile you manually enter the input of the first command like `./pipex here_doc LIMITER cmd0 cmd1 outfile`. Dont forget that on heredoc mode you shoud append in the outfile instead of repace what was inside.  
  
I passed this project with the maximum grade of 125%

## 🖐️ How to use ? 
Git clone the repo, make and you're done !\
Some usage examples are shown above, and feel free to try every command you want.. but these commands must have an executable that we can find on the $PATH (you can try `whereis cmd` to see if the command is valid for pipex).  
