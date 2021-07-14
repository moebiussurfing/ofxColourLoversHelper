ofxColourLoversHelper
------------------------------------

# Overview
**ofxColourLoversHelper** is an **openFrameworks** helper addon to handle **ColourLovers** palettes.
 
The core of this addons is https://github.com/borg/ofxColourLovers.  
Thanks to **@borg**!  
I just added some methods to help integrate into my projects/addons, and also switched from *ofxUI* to *ofxImGui* user control.

<!-- ## Screenshot -->
<!-- ![image](/readme_images/Capture_ofxImGui.PNG?raw=true "image")   -->
## Screencast
![image](/readme_images/ofxColourLoversHelper.gif?raw=true "gif")

## Features
- Implemented GUI with **ImGui**.  

## Usage
- Look the example.

## Dependencies
Add to your project using ```Project Generator```:  
* [ofxImGui](https://github.com/Daandelange/ofxImGui/tree/ofParameters-Helpers-Test)
* [ofxSurfingHelpers](https://github.com/moebiussurfing/ofxSurfingHelpers)  
* ofxPoco
* ofxSSLManager
* ofxIO
* ofxMediaType
* ofxNetworkUtils
* ofxHTTP
* ofxSurfingHelpers
* ofxSurfingImGui
* ofxXmlSettings
Already included into ```OF_ADDON/libs/```. No need to add:  
* [ofxColourLovers](https://github.com/borg/ofxColourLovers)  

## TODO
- Fix singleton/threading on API search.

## Tested systems
- **Windows10** / **VS2017** / **OF ~0.11**

## Author
Addon by **@moebiusSurfing**  
*(ManuMolina). 2021.*  

Thanks to **@borg**, **@arturoc**, **@MacFurax** and **@Daandelange** for the repositories.  

## License
*MIT License.*
