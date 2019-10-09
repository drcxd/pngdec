# pngdec

This is a very simple image decoding program which may extract a hidden image(if ther is any)
from a normal image.

# How does it work

Actually it is really simple. Since png uses 4bytes to represent RGBA, if we modify the least significant
bit of the red color byte, human eyes can not detect such a subtle change. Then we could use the least
significant bit of the red byte of every pixel to store our information. We could use bit 1 to represent
black and bit 0 to represent white. So we could obtain a black-white image of the same dimension with the
original one.

# Why I do this

One day I saw a news about one game which was going to released on steam. It was *Catherine*, which is
developed by ATLUS and published by SEGA. The news said that SEGA had published several posts which contain
mysterious pictures of sheep. Some one said that these pictures contained hidden messages. The news lead
me to an [website](https://osric.com/chris/steganography/decode.html) which provides service of decoding
such encrypted message. I was thinking: why couldn't I implement one of my own?

So I searched online the pnd file format and how this message was encrypted. And then the result is this
little program!
