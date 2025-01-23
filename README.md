# Text-Renderer
 OpenGL Text Renderer

Uses Array textures for 

Textbox Functionality:
- Takes in a string,
- Goes through the string and substituting any values that are writtin within a {} and replacing them using texttable,
- Goes through the string again while knowing the font's size; determining how long each line should be (while wrapping appropriately) in comparison to the textbox size
- if the text's width and height are too big for the textbox, switch to a smaller font and try the previous step again.
- Once the text can fit inside the textbox or you have reached the smallest font with whichever font you will generate the vertices and render the text.

A few cases I want you to keep in mind:
- If word is too long to fit in textbox's width you can either word wrap (add a - and go to the next line)
- If at smallest font and even while word wrapping you cannot fit the whole text, cut it off with elipses