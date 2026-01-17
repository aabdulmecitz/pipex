*This project has been created as part of the 42 curriculum by aozkaya.*

# 🚀 Pipex

## 📝 Description
Pipex is a foundational project at 42 that challenges students to re-implement the UNIX pipe mechanism (`|`). The program mimics the behavior of the shell, handling data flow between commands and file redirections with precision.

### Key Goals:
- **Redirection**: Master `dup2` and `fork` to handle custom STDIN/STDOUT.
- **Process Management**: Synchronize child processes and handle execution failures gracefully.
- **Pipe Logic**: Create a seamless communication channel between two or more system commands.

---

## 🛠 Instructions

### Compilation
The project includes a `Makefile` that supports both mandatory and bonus features without unnecessary relinking.

| Component | Command | Output |
| :--- | :--- | :--- |
| **Mandatory** | `make` | `./pipex` |
| **Bonus** | `make bonus` | `./pipex_bonus` |
| **Cleanup** | `make fclean` | Removes executables & objects |
| **Rebuild** | `make bonus_re` | Full bonus recompile |

### Execution Examples

#### 1. Mandatory Part
Simulates: `< infile cmd1 | cmd2 > outfile`
```bash
./pipex infile "ls -l" "wc -l" outfile
```

#### 2. Bonus: Multiple Pipes
Simulates: `< infile cmd1 | cmd2 | cmd3 | ... | cmdn > outfile`
```bash
./pipex_bonus infile "cat" "grep pattern" "wc -l" outfile
```

#### 3. Bonus: Here_doc
Simulates: `cmd << LIMITER | cmd1 >> outfile`
```bash
./pipex_bonus here_doc LIMITER "cat" "wc -l" outfile
```

---

## 📚 Resources

### References
- [Pipe (Unix) - Wikipedia](https://en.wikipedia.org/wiki/Pipe_(Unix))
- [Code Vault - Youtube](
    https://youtu.be/Mqb2dVRe0uo?si=-RoazM1HC9QZbpdR , 
    https://youtu.be/6xbLgZpOBi8?si=D-qv4VUc4MAgV3I3 , 
    https://youtu.be/2hba3etpoJg?si=w1uqzfm-gpgkwBsV , 
    https://youtu.be/OVFEWSP7n8c?si=68I5axncIvbDSCJ4 , 
    https://youtu.be/NkfIUo_Qq4c?si=4aKi8q1-bwacSC8Q)
- [Wait and Waitpid - Manual](https://man7.org/linux/man-pages/man2/wait.2.html)

### AI Usage Disclosure
Artificial Intelligence (Antigravity/Gemini) was used during this project as a **learning companion**. My primary goal was to use AI to deepen my understanding of UNIX mechanisms. It helped me:
- Better understand the interaction between `fork`, `pipe`, and `dup2`.
- Learn how to structure iterative loops for complex pipelines in the bonus section.
- Identify best practices for process synchronization and file descriptor management.
- Ensure the codebase follows the 42 Norme through interactive feedback.
