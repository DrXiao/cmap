# Documents of cmap
There have two documents:
* ```cmap.md```
* ```red-black-tree.md```

Because of same mistakes when drawing the images, if you click the two markdown files to view the content on Github and with black theme, the images may be invisible.
Suggest to view the pdf files (converted from markdown files) or download the source and view the markdown files by other tools.

For example, you can download a Python module called ```grip``` and it helps you to convert the markdown files to html and open the browser to view the entire content.
```
$ pip3 install grip
$ grip -b red-black-tree.md
```

Or,
* by ```pandoc``` command
	* ```$ pandoc -V geometry:"top=2cm, bottom=1.5cm, left=2cm, right=2cm" red-black-tree.md -o red-black-tree.pdf```
* Visual Studio Code (VSCode), a famous text editor designed by Microsoft, also can find the extension, called ```Markdown Preview Enhanced```, to skim the markdown files.
* HackMD, a collaborative platform, can also browse the content by importing the markdown files.
