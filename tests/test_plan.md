
# Mini Shell (msh) - Test Plan

**Student Name:** INDAKWA PAULINE APPHIE  
**Registration Number:** SCT212-0062/2024  
**Programming Language:** C  
**Testing Environment:** Ubuntu Linux (WSL)

## 1. Purpose of Testing

The purpose of this test plan is to verify that the Mini Shell performs the required operations correctly. The tests cover command execution, argument handling, built-in commands, error handling, background processes, output redirection and signal handling.

## 2. Test Cases and Results

| No. | Test Input | Expected Behaviour | Actual Result | Status |
|---|---|---|---|---|
| 1 | Press Enter without typing a command | A new prompt appears without an error | Tested successfully; a new prompt appeared | PASS |
| 2 | `echo hello world` | Displays the text and returns status 0 | Tested successfully; displayed `hello world` | PASS |
| 3 | `ls /nonexistent` | Displays an error from ls and returns a non-zero status | Tested successfully; ls displayed a missing directory error and shell continued to 2 |PASS |
| 4 | `nosuchcommand` | Displays an error message and returns status 127 | Tested successfully; error displayed and shell continued | PASS |
| 5 | `cd /tmp` followed by `pwd` | Displays `/tmp` | Tested successfully; displayed `/tmp` | PASS |
| 6 | `sleep 3 &` | Returns the prompt immediately and displays the process ID | Tested successfully; process ID displayed and shell returned to prompt | PASS |
| 7 | `echo hi > f.txt` followed by `cat f.txt` | Creates the file and displays `hi` | Tested successfully; displayed `hi` | PASS |
| 8 | `sleep 100` followed by Ctrl+C | Stops sleep, keeps shell running and displays status 130 | Tested successfully; shell remained open | PASS |
| 9 | Ctrl+D at the prompt | Exits the shell cleanly | Tested successfully; shell exited | PASS |

## 3. Additional Tests

| No. | Test Input | Expected Behaviour | Actual Result | Status |
|---|---|---|---|---|
| 10 | `help` | Displays available built-in commands | Tested successfully; help information displayed | PASS |
| 11 | `cd` followed by `pwd` | Changes to the user's home directory | Tested successfully; cd without argument changed to home directory |PASS |
| 12 | `echo first > f.txt` followed by `echo second > f.txt` and `cat f.txt` | Replaces the previous file contents with `second` | Tested successfully; the second command replaced the previous file contents with "second" |PASS |

## 4. Testing Summary

All planned tests were executed successfully. The tests confirmed that the Mini Shell handles blank input, external commands, invalid commands, invalid directories, built-in commands, background processes, output redirection, Ctrl+C and Ctrl+D as expected.
