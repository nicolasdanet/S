
# Spaghettis

Yet another fork of [Pure Data](http://msp.ucsd.edu/) with [JUCE](https://github.com/juce-framework/JUCE).

## Platforms

Developed on Ubuntu 22.04, macOS Big Sur.
        
## Dependencies

JUCE ( 7.0.12 ) is required at `~/JUCE`.

## Build

- Open a terminal and run the `S/Build.sh` script.
- Resulting binary is inside the `Spaghettis/Builds/` folder.

## Warning

- Spaghettis uses new filename extensions ***pdpatch*** and ***pdhelp***.

## Roadmap

Next features planed:

- Improve compatibility with [Pure Data](http://msp.ucsd.edu/) (e.g. the clone object).
- Make the application embeddable (ala [libpd](https://github.com/libpd/libpd) and [plugdata](https://github.com/plugdata-team/plugdata)).
- Support plainly dynamic patching with scripts (Lua / Python...).
- Add something like [Gem](https://gem.iem.at/) based on a portable framework (OF / Cinder...).

## Residuals

**macOS**

    ~/Library/Application Support/Spaghettis/
    ~/Library/Preferences/org.puredata.spaghettis.plist

**Linux**

    ~/.config/spaghettis/
    
## Credits

- Pure Data by [Miller Puckette & others](http://msp.ucsd.edu/Software/pd-README.txt).
- TinyExpr by [Lewis Van Winkle](https://github.com/codeplea/tinyexpr).
- Mersenne Twister by [T. Nishimura & M. Matsumoto](http://www.math.sci.hiroshima-u.ac.jp/~m-mat).
- FFT by [Takuya Ooura](http://www.kurims.kyoto-u.ac.jp/~ooura/fft.html).
- Icons by [Google](https://github.com/google/material-design-icons).

#### Licensed  under the [AGPLv3](https://www.gnu.org/licenses/agpl-3.0.en.html).
