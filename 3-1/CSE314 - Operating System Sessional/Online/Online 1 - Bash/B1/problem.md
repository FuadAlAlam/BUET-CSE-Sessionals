<div align="center">

**January 2026 CSE 314**

**Online Assignment on Bash Scripting**

Time: 30 minutes

Subsection B1

</div>

You are working as a data analyst for a team playing in the 2026 FIFA World Cup Final in New York. A hacker has infiltrated your servers and scrambled the extensions of all tactical analysis files in the `input_dir` directory. The files now look like random system garbage. You know the genuine tactical files are all plain text (ASCII format). You need to write a script to identify these text files, ignore the fake extensions, and copy them to a secure `output_dir`.

### Command

```bash
./online-2205XXX.sh input_dir output_dir
```

### Output

1. Identify and copy only the **ASCII text files** from `input_dir` into `output_dir`. Hint: `man file`
2. Rename the copied files based on the **number of lines** they contain, in **ascending order**.
   - The file with the fewest lines → `0_original_filename.ext`
   - The next → `1_original_filename.ext`, and so on.
   - Keep the original extension.

### Sample Input (`input_dir/`)

```
input_dir/
├── formation.abc          ← ASCII text, 1 line
├── tactical_data.xyz      ← ASCII text, 3 lines
├── player_stats.dll       ← ASCII text, 7 lines
├── image_cache.sys        ← Binary (not ASCII) — IGNORE
└── metadata.bin           ← Binary (not ASCII) — IGNORE
```

### Sample Output (`output_dir/`)

```
output_dir/
├── 0_formation.abc
├── 1_tactical_data.xyz
└── 2_player_stats.dll
```


