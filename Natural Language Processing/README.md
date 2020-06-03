Read the TSV train and dev files created from the break dataset after they have been converted from CSVs into TSVs by preprocess_examples.py
Read the JSON train and dev files created by the Biaffine Parser
Merge the heads of the sentences from JSON files into TSV files, so the original sentence a word's head after every word, and the target remains the same.
Output a TSV file of the meged sentence and the original target
Option to create same word heads or random heads for baseline
To run - change file paths if needed, control heads being from JSON, same word or random using same_word_heads and is_random heads variables.
