# huffman-coding-core
huffman-coding-core-system

# Description
ハフマン法による圧縮

this is the core system of my huffman coding project

# How to use

圧縮、解凍したいファイルをehuf.exeにドラッグドロップするだけで勝手に圧縮、解凍してくれます

もしファイルが.ehufファイルでなかったときは同じディレクトリに圧縮したファイルを"元の名前+.ehuf"の名前で保存します.

もしファイルが.ehufであったときはファイルを解凍してそのファイルを同じディレクトリに保存します、命名の規則については後述します.

just drag&drop your file to main.exe
if the file was .ehuf, it extracts the file and save it to the same directry.
if the file was not .ehuf, it compresses the file and save it to the same directry with name made by original name and .ehuf extens

## rule of naming file

the compressed file should be like this 

{name}.{extension}.huff

when the application extracts the file, it adds "1" to the last of {name}, So the file name is going to be like this.

{name}1.{extension}

# Other things

This document might be hard to read in English (for both Japanese and English speaker) because of my weak English skill.
If you found any improvement point in my document please contact [me](https://twitter.com/erurami3250)

# License
This software is released under the MIT license.
