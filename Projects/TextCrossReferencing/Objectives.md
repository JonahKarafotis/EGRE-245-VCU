# Text Cross Referencing: Objectives and Plan

Source: `proj4.pdf`, Programming Project #4, EGRE246.

## What the program must do

Write a C program that accepts an input file path on the command line, reads the text one line at a time, and prints an alphabetical cross-reference of its words followed by statistics.

- Print each unique word and the line numbers on which it appears. The first line is line 1.
- Count every word occurrence, including repeated words on the same line.
- Print a line number only once for a given word. The sample has `hi hi hi` on line 1, but `hi` references line 1 only once.
- Count all input lines, including blank lines and lines containing only punctuation or numbers. Count a final nonempty line even if it has no trailing newline.
- Preserve capitalization: `Fun` and `fun` are different words. Sort using `strcmp`, which puts uppercase before lowercase for the sample's letters.
- Handle up to 10,000 unique words.
- Print an error message and exit if the input file does not exist.

## Answers to the questions in my first draft

### Does the file need to end in .dat?

No filename-extension restriction is stated. `cref.dat` is the example input filename; `.dat` is a generic data-file extension. Open the supplied path as a text stream. Do not reject a readable text file merely because its name lacks `.dat`. Checking for arbitrary binary-file formats is outside the stated requirements.

Also check that the user supplied the expected command-line argument and handle any failure to open the file with a useful error.

### What counts as a word?

There is no dictionary lookup or spelling check. Process strings of letters as words, even if they are not dictionary words. Ignore spaces, punctuation, and numbers when identifying words.

The sample establishes that a hyphen separates words: `mom-Fun` produces `mom` and `Fun`. A practical parsing choice is to treat every nonletter as a separator, so `hi!` produces `hi` and `-13` produces no word. The PDF does not explicitly illustrate embedded digits or apostrophes; under this choice, `abc123def` becomes `abc` and `def`, and `can't` becomes `can` and `t`. These are design assumptions to confirm with the instructor if needed.

### What must be stored for each word?

My original word-and-count structure is not enough: each word also needs a list of line numbers. A per-word occurrence count is optional because the required output shows a single total word count, not a count beside each word.

## Implementation constraints

- Read and process one complete line at a time; do not store the entire input file.
- Assume a maximum input line length of 200 characters. Plan a buffer of 202 bytes to allow 200 text characters, a newline, and the terminating null character.
- Get file input working before implementing string processing.
- Use the singly-linked list module `llist` to create and maintain each word's line-number list. The PDF points to Lab7 and Lab8 as sources for its implementation.
- Do not use global variables. Preprocessor constants such as `#define MAX_WORDS 10000` are allowed.
- Divide the program into appropriate functions.
- Match the sample output, including a word field 15 characters wide. Longer words may extend beyond that width.
- Submit a single source file through Canvas, documented according to class expectations. If working with a partner, include both names and submit one copy.

`fgets`, `strtok`, and `qsort` are recommendations, not required library choices. The `llist` requirement is mandatory.

## Proposed data structure

Use a table with one entry per unique word. Each entry owns a copy of the word and a separate `llist` of line numbers.

Conceptually, each entry contains:

| Field | Purpose |
| --- | --- |
| `word` | Null-terminated word text, preserving case |
| `references` | An `llist` containing distinct line numbers in increasing order |
| `last_line` | Last line recorded for this word, initially 0; prevents duplicate references |

The exact C declaration for `references` depends on the Lab7/Lab8 module's types and initialization rules. Review that module before writing the final structure; those lab files are not currently in this project folder.

For `word`, a fixed `char word[201]` can hold the longest possible word under the line-length limit. Unlike the original `char Word[]` sketch, it has an explicit capacity. A dynamically allocated string is another option.

A 10,000-entry table with fixed-size word fields is large, so allocate the table on the heap and check allocation success. Keep the pointer and counters local to `main` and pass them to helper functions.

Keep three overall counters:

- `unique_word_count`: number of occupied table entries.
- `total_word_count`: incremented for every parsed word, including duplicates.
- `line_count`: incremented for every successfully read input line.

## Implementation steps

1. **Read the file correctly.**
   - Validate the command-line argument and open the file.
   - Read with `fgets`, counting lines and temporarily displaying them to verify input.
   - Verify blank lines and the final line without a newline. Remove temporary display output before submission.
   - Check for a read error and close the file.

2. **Split each line into words.**
   - One approach is to replace nonletters with spaces, then use `strtok` with whitespace delimiters. This also handles numbers without needing a long punctuation delimiter list.
   - If using `isalpha`, pass each character as `(unsigned char)` to avoid invalid negative character arguments.
   - Process each token immediately. Copy a new word into its table entry before the line buffer is reused; do not keep pointers into that buffer.

3. **Build the unique-word table and count occurrences.**
   - Increment the total word count for every token.
   - Search the existing entries using `strcmp` for exact, case-sensitive equality.
   - If the word is new, initialize its entry and its `llist`, then increment the unique count.
   - Check capacity before adding a new entry. A duplicate remains valid when the table already contains 10,000 unique words.
   - A linear search is a simple starting choice; optimize only if performance requires it.

4. **Record line references through llist.**
   - When `last_line` differs from the current line, append that line number and update `last_line`.
   - When the word repeats on the same line, increment the total word count but do not add another reference.
   - Reading in order and appending references keeps the line numbers increasing. Check how the lab module supports insertion at the end.

5. **Sort and print.**
   - Sort the occupied table entries using a comparator based on `strcmp`; `qsort` is a suitable choice.
   - Print the project/name banner, column headings, separators, words, comma-separated line numbers, and statistics as shown below.
   - Use a left-aligned minimum width of 15 for the word field, followed by the sample's separating space. Do not truncate longer words.

6. **Clean up and submit.**
   - Free every line-number list using the module's cleanup operation, then free the table and any separately allocated strings.
   - Handle allocation failures without silently producing an incomplete result.
   - Combine the required `llist` implementation with the program in the single submitted source file, following any class conventions.
   - Add the required class documentation and author name(s).

## Suggested function responsibilities

- `main`: validate input, coordinate processing, and handle exit status.
- A line-processing function: identify tokens and update totals.
- A word-lookup function: find an existing table entry.
- A reference-update function: add a line only when it is new for that word, using `llist`.
- A comparison function: support sorting with `strcmp`.
- An output function: print the table and statistics.
- A cleanup function: release lists and other allocated memory.

Pass required state as arguments rather than using global variables. Function signatures should be settled after reviewing the actual `llist` interface.

## Sample acceptance check

The PDF's input contains a blank fourth line. Preserve it when recreating the sample:

```text
hi hi hi mom
mom dad
fun

Fun mom-Fun
. ? -
E hi! Fun???!!!@
dad -13
mom
```

The meaningful output must match this, with my own name in the banner:

```text
Proj. #4 - <my name>

Word            References
--------------- -------------
E               7
Fun             5,7
dad             2,8
fun             3
hi              1,7
mom             1,2,5,9

Total unique words: 6
Total words: 15
Total lines: 9
```

`Fun` occurs twice on line 5 but records that line once. `hi` occurs three times on line 1 but records that line once. All of those occurrences contribute to the total of 15 words.

## Verification checklist

- [ ] Match the PDF sample's ordering, references, totals, and formatting.
- [ ] Report missing arguments and nonexistent/unreadable files clearly.
- [ ] Handle empty input with zero words, unique words, and lines.
- [ ] Count blank and punctuation-only lines without creating words.
- [ ] Count repeated words on one line while recording that line only once.
- [ ] Record the same word on multiple lines in increasing order.
- [ ] Keep uppercase and lowercase spellings separate.
- [ ] Handle a final line without a newline and a 200-character input line.
- [ ] Support 10,000 unique words and detect capacity overflow before writing past the table.
- [ ] Release all allocated memory and build with compiler warnings enabled.

## Next action

Review the Lab7/Lab8 `llist` module to identify its list type and its initialization, insertion, traversal, and cleanup functions. Then implement and verify file input before starting the word parser, as required by the PDF.
