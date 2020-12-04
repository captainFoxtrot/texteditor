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

I've cleaned up (some) of the code and fixed the majority of the bugs. However, a few remain persistent:
- When provided with no args (e.g. start with new file), the program does not display the contents of the code correctly.
- Newlines at the end of the file cause any number of the following to happen:
    - The last line is duplicated on the display, but not in memory
    - The last line cannot be deleted at all

## Contributing

For the 5 people that are ever going to look at this, do feel free to contribute, suggest changes, or just play around with the code.