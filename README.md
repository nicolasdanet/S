
# S

This a WIP version of [Spaghettis](https://github.com/Spaghettis/Spaghettis) that aims to use [JUCE](https://github.com/juce-framework/JUCE) for the GUI.

## Platforms

Tested on Ubuntu 20.04, macOS Big Sur (Intel only), and Raspbian Buster (RPI4).
        
## Dependencies

The latest JUCE (6.0.8) is required at `~/JUCE`.

## Build

- Open a terminal and run the `S/Build.sh` script.
- Resulting binary is inside the `Spaghettis/Builds/` folder.

## Comments

- Spaghettis uses new filename extensions ***pdpatch*** and ***pdhelp***.
- You can open [test patches](https://github.com/nicolasdanet/S/tree/master/Spaghettis/Modules/T/stuff) for a quick look.

## Residuals

**macOS**

    ~/Library/Application Support/Spaghettis/
    ~/Library/Preferences/org.puredata.spaghettis.plist

**Linux**

    ~/.config/spaghettis/

## Credits

- Pure Data by [Miller Puckette & others](http://msp.ucsd.edu/Software/pd-README.txt)
- Belle by [Andi Brae](https://github.com/andibrae/belle)
- TinyExpr by [Lewis Van Winkle](https://github.com/codeplea/tinyexpr)
- Mersenne Twister by [T. Nishimura & M. Matsumoto](http://www.math.sci.hiroshima-u.ac.jp/~m-mat)
- FFT by [Takuya Ooura](http://www.kurims.kyoto-u.ac.jp/~ooura/fft.html)

#### Licensed  under the [GPL-3.0 license](https://opensource.org/licenses/GPL-3.0).
