# About
AA+ is a C++ implementation for the algorithms as presented in the book "Astronomical Algorithms" by Jean Meeus.
The class framework is done by PJ Naughter.
It also has implementations in other languages, like [JavaScript (aa-js)](https://github.com/onekiloparsec/aa-js), [C# (AASharp)](https://github.com/jsauve/AASharp) and [Swift (SwiftAA)](https://github.com/onekiloparsec/SwiftAA).
However, always keeping updated with AA+ is difficult.

Thanks to Microsoft's C++/CLI technology and Naughter's excellent coding style.
These enable us to compile C++ code into a library that can be used by the dotnet ecosystem with minimal changes.

This project translates native C++ code into C++/CLI based on some simple regexs.
Therefore, if the AA+ library is updated in the future, everyone can compile the latest AA+ dotnet library immediately with minimal modification/translation cost.

Currently, the AAPlusSharp corresponds to version 2.52 of AA+.

# Build steps
1. Download aaplus.zip from http://www.naughter.com/aa.html.
2. Extract aaplus.zip and place all files in the input folder.
3. Run AAPCpp2Cli with proper input/output path. Typically `./aaplus` and `./AAPlusSharp`.
4. Build AAPlusSharp.