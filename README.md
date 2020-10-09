# texteditor
 Work-in-progress, terminal-based text editor written in C. 

## Usage

Full documentation will (hopefully) come when this program becomes usable. For now:
- Start the editor by typing `make run`; to open a file, type `make run FILENAME=path/to/file`.
- To re-compile the program without running it, simply type `make`.
- Use the arrow keys to navigate the cursor.
- To save a file, simply hit `F1`.
- To close the editor, type `Ctrl+C`.

Disclaimer: I am not responsible should this program corrupt any of your files (the chance of this happening is almost zero, though you can examine the source code if you like.)

## Bugs

There are a few bugs and weird behavior that has to be fixed before this has any chance of being used as an actual text editor. Examples:
- Attempts to open large-ish files (above 2000 chars or so) will fail
- Program *always* segfaults when closed with Ctrl+C
- Screen doesn't scroll correctly

## Contributing

For the 5 people that are ever going to look at this, do feel free to contribute, suggest changes, or just play around with the code.