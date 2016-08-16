# Unix Shell
Unix Shell - Programmed in C

<p>
Executable framework for a Linux shell programmed in C on a x64amd processor running Ubuntu. </br>
The command prompt is similar to unix allowing the user to execute any Linux command.
</p>
<p>
<ul>
<li>User is promted with "command?:".</li>
<li>the shell will exit when the “exit” command is issued.</li>
<li>the shell is bash-syntax compatible.</li>
</ul>
Shell function and information:
<ul>
<li>Handles basic commands, such as ls -l, pwd, and others.</li>
<li>Handles a single pipe such as (ls -l | wc -w)</li>
<li>mssh shell read input from a file called .msshrc and executes each line in this file as a
command. This makes the .msshrc file a batch file. (BASH reads from .bashrc)</li>
<li>Implemented aliasing, i.e. define an alias and undefine it.</li>
<li>The .msshrc should be able to have predefined aliases. For example, it would contain
alias LA=”ls -a"</li>
<li>Implemented path extension feature, e.g., PATH=$PATH:/data/myprogs. Typically, this is
executed when you log in, which means that it would be in the .msshrc file.</li>
<li>Implemented a "S_history" mechanism which will allow to scroll though the last N shell commands
where N=HISTORYCOUNT is an environment variable in .msshrc. In .msshrc (if it exists) will
be at least two environment variables HISTCOUNT and HISTFILECOUNT. If .msshrc does not
exist, the HISTCOUNT will be 100 and HISTFILECOUNT will be 1000 by default. The S_history is stored in a file called
.msshrc_history.</li>
<li>Implemented CD (change directory) command</li>
</ul>
<p>
GENERAL FLOW</br>
• The program starts up and checks for:</br>
o .msshrc – if it exists reads in the variables – file format is below – doesn’t exist then</br>
default HISTCOUNT and HISTFILECOUNT are used</br>
o .mssh_history is loaded if it exists</br>
o prompt is displayed</br>
o each command is handled appropriately</br>
o upon exit .mssh_history is written from the executed commands</br>
.run_msshrc file format – if exists (guaranteed to have HISTCOUNT and HISTFILECOUNT</br>
HISTCOUNT=value</br>
HISTFILECOUNT=value</br>
Alias1</br>
Alias2</br>
…</br>
AliasN</br></br>
PATH=$PATH:/data/myprogs</br>
</p>
